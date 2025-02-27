//
// Created by cv2 on 03.02.2025.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <bitset>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <cstdlib>
#include <ctime>
#include <sys/stat.h>
#include <algorithm>
#include <crypt.h>

void show_help() {
    std::cout << "Usage: cryonix_usermod [options] username\n"
              << "Options:\n"
              << "  -m, --move-home        Move the user's home directory\n"
              << "  -s, --shell SHELL      Change the login shell for the user\n"
              << "  -u, --uid UID          Change the user ID for the user\n"
              << "  -g, --group GROUP      Change the primary group for the user\n"
              << "  --help                 Display this help message\n";
}

bool user_exists(const std::string& username) {
    struct passwd* pwd = getpwnam(username.c_str());
    return pwd != nullptr;
}

bool group_exists(const std::string& group_name) {
    struct group* grp = getgrnam(group_name.c_str());
    return grp != nullptr;
}

bool update_passwd(const std::string& username, const std::string& new_home, const std::string& new_shell, int new_uid, int new_gid) {
    std::ifstream passwd_file("/etc/passwd");
    std::ofstream temp_file("/etc/passwd.tmp", std::ios::app);
    if (!passwd_file.is_open() || !temp_file.is_open()) {
        std::cerr << "Error: Unable to open /etc/passwd" << std::endl;
        return false;
    }

    std::string line;
    bool user_found = false;
    while (std::getline(passwd_file, line)) {
        std::stringstream ss(line);
        std::string user;
        ss >> user;
        if (user == username) {
            user_found = true;
            std::string new_entry = username + ":x:" + std::to_string(new_uid) + ":" + std::to_string(new_gid) + ":"
                                    + username + " User::" + new_home + ":" + new_shell;
            temp_file << new_entry << std::endl;
        } else {
            temp_file << line << std::endl;
        }
    }

    if (!user_found) {
        std::cerr << "Error: User " << username << " not found in /etc/passwd" << std::endl;
        return false;
    }

    passwd_file.close();
    temp_file.close();
    std::remove("/etc/passwd");
    std::rename("/etc/passwd.tmp", "/etc/passwd");

    return true;
}

bool update_shadow(const std::string& username, const std::string& password_hash) {
    std::ifstream shadow_file("/etc/shadow");
    std::ofstream temp_file("/etc/shadow.tmp", std::ios::app);
    if (!shadow_file.is_open() || !temp_file.is_open()) {
        std::cerr << "Error: Unable to open /etc/shadow" << std::endl;
        return false;
    }

    std::string line;
    bool user_found = false;
    while (std::getline(shadow_file, line)) {
        std::stringstream ss(line);
        std::string user;
        ss >> user;
        if (user == username) {
            user_found = true;
            temp_file << username << ":" << password_hash << "::::::" << std::endl;
        } else {
            temp_file << line << std::endl;
        }
    }

    if (!user_found) {
        std::cerr << "Error: User " << username << " not found in /etc/shadow" << std::endl;
        return false;
    }

    shadow_file.close();
    temp_file.close();
    std::remove("/etc/shadow");
    std::rename("/etc/shadow.tmp", "/etc/shadow");

    return true;
}

bool update_group(const std::string& username, int new_gid) {
    std::ifstream group_file("/etc/group");
    std::ofstream temp_file("/etc/group.tmp", std::ios::app);
    if (!group_file.is_open() || !temp_file.is_open()) {
        std::cerr << "Error: Unable to open /etc/group" << std::endl;
        return false;
    }

    std::string line;
    bool group_found = false;
    while (std::getline(group_file, line)) {
        std::stringstream ss(line);
        std::string group_name;
        int group_gid;
        ss >> group_name;
        ss.ignore(); // skip the colon
        ss >> group_gid;
        if (group_name == username) {
            group_found = true;
            temp_file << group_name << ":x:" << new_gid << ":" << username << std::endl;
        } else {
            temp_file << line << std::endl;
        }
    }

    if (!group_found) {
        std::cerr << "Error: Group for " << username << " not found in /etc/group" << std::endl;
        return false;
    }

    group_file.close();
    temp_file.close();
    std::remove("/etc/group");
    std::rename("/etc/group.tmp", "/etc/group");

    return true;
}

std::string hash_password(const std::string& password) {
    const std::string salt_prefix = "$6$";  // SHA-512
    constexpr size_t salt_size = 3 + 16 + 1;  // 3 (prefix) + 16 chars + null
    char salt[salt_size] = {0};  // Zero-initialize buffer

    const char* salt_chars =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";

    // Copy prefix with bounds checking
    if (salt_prefix.size() >= salt_size) {
        throw std::runtime_error("Salt prefix too long for buffer");
    }

    // Safe copy using standard library with bounds checking
    std::snprintf(salt, salt_size, "%s", salt_prefix.c_str());

    // Generate random bytes
    unsigned char rand_bytes[16];
    FILE* urandom = fopen("/dev/urandom", "rb");
    if (!urandom || fread(rand_bytes, 1, sizeof(rand_bytes), urandom) != sizeof(rand_bytes)) {
        if (urandom) fclose(urandom);
        throw std::runtime_error("Failed to generate random salt");
    }
    fclose(urandom);

    // Fill salt with random characters
    const size_t salt_chars_len = std::strlen(salt_chars);
    for (size_t i = 0; i < 16; ++i) {
        const size_t pos = salt_prefix.length() + i;
        if (pos >= salt_size - 1) break;  // Prevent buffer overflow
        salt[pos] = salt_chars[rand_bytes[i] % salt_chars_len];
    }
    salt[salt_size - 1] = '\0';  // Ensure null termination

    // crypt_r initialization
    struct crypt_data data;
    std::memset(&data, 0, sizeof(data));

    char* result = crypt_r(password.c_str(), salt, &data);
    if (!result) {
        throw std::runtime_error("Password hashing failed");
    }

    return std::string(result);
}

int main(int argc, char* argv[]) {
    std::string username;
    std::string new_home;
    std::string shell = "/bin/bash";
    int new_uid = -1;
    int new_gid = -1;
    bool move_home = false;
    bool help_flag = false;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            help_flag = true;
        } else if (arg == "--move-home" || arg == "-m") {
            move_home = true;
        } else if (arg == "--shell" || arg == "-s") {
            if (i + 1 < argc) {
                shell = argv[++i];
            } else {
                std::cerr << "Error: Missing shell argument after --shell" << std::endl;
                return 1;
            }
        } else if (arg == "--uid" || arg == "-u") {
            if (i + 1 < argc) {
                new_uid = std::stoi(argv[++i]);
            } else {
                std::cerr << "Error: Missing UID argument after --uid" << std::endl;
                return 1;
            }
        } else if (arg == "--group" || arg == "-g") {
            if (i + 1 < argc) {
                std::string group_name = argv[++i];
                if (group_exists(group_name)) {
                    struct group* grp = getgrnam(group_name.c_str());
                    new_gid = grp->gr_gid;
                } else {
                    std::cerr << "Error: Group " << group_name << " not found" << std::endl;
                    return 1;
                }
            } else {
                std::cerr << "Error: Missing group argument after --group" << std::endl;
                return 1;
            }
        } else {
            username = arg;
        }
    }

    if (help_flag || username.empty()) {
        show_help();
        return 0;
    }

    if (!user_exists(username)) {
        std::cerr << "Error: User " << username << " does not exist." << std::endl;
        return 1;
    }

    if (move_home) {
        std::cout << "Move home directory for user " << username << "?\n";
        std::cout << "Enter the new home directory path: ";
        std::getline(std::cin, new_home);
    }

    std::string password;
    std::cout << "Enter the new password for " << username << ": ";
    std::getline(std::cin, password);
    std::string password_hash = hash_password(password);

    // Update /etc/passwd
    if (!update_passwd(username, new_home.empty() ? "/home/" + username : new_home, shell, new_uid == -1 ? 1000 : new_uid, new_gid == -1 ? 1000 : new_gid)) {
        return 1;
    }

    // Update /etc/shadow
    if (!update_shadow(username, password_hash)) {
        return 1;
    }

    // Update /etc/group
    if (new_gid != -1 && !update_group(username, new_gid)) {
        return 1;
    }

    std::cout << "User " << username << " has been successfully modified." << std::endl;
    return 0;
}