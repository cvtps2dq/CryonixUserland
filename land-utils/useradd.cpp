//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <crypt.h>
#include <shadow.h>
#include <sys/types.h>
#include <cstdlib>
#include <ctime>

void show_help() {
    std::cout << "Usage: cryonix_useradd [options] username\n"
              << "Options:\n"
              << "  -m, --create-home      Create the user's home directory\n"
              << "  -s, --shell SHELL      Set the login shell for the user\n"
              << "  -g, --group GROUP      Set the initial group for the user\n"
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

bool create_home_directory(const std::string& username) {
    std::string home_dir = "/home/" + username;
    if (mkdir(home_dir.c_str(), 0755) == -1) {
        std::cerr << "Error: Unable to create home directory for user " << username << std::endl;
        return false;
    }
    return true;
}

bool add_user_to_passwd(const std::string& username, const std::string& home_dir, const std::string& shell, int uid, int gid) {
    std::ofstream passwd_file("/etc/passwd", std::ios::app);
    if (!passwd_file.is_open()) {
        std::cerr << "Error: Unable to open /etc/passwd" << std::endl;
        return false;
    }

    std::ostringstream user_info;
    user_info << username << ":x:" << uid << ":" << gid << ":" << username << " User::" << home_dir << ":" << shell;
    passwd_file << user_info.str() << std::endl;
    return true;
}

bool add_user_to_shadow(const std::string& username, const std::string& password_hash) {
    std::ofstream shadow_file("/etc/shadow", std::ios::app);
    if (!shadow_file.is_open()) {
        std::cerr << "Error: Unable to open /etc/shadow" << std::endl;
        return false;
    }

    std::ostringstream user_shadow;
    user_shadow << username << ":" << password_hash << "::::::";
    shadow_file << user_shadow.str() << std::endl;
    return true;
}

std::string hash_password(const std::string& password) {
    const char* salt = "$6$random_salt$";  // Example salt (SHA-512)
    struct crypt_data data;
    data.initialized = 0;
    return crypt_r(password.c_str(), salt, &data);
}

bool create_group(const std::string& group_name, int gid) {
    std::ofstream group_file("/etc/group", std::ios::app);
    if (!group_file.is_open()) {
        std::cerr << "Error: Unable to open /etc/group" << std::endl;
        return false;
    }

    group_file << group_name << ":x:" << gid << ":" << group_name << std::endl;
    return true;
}

// Function to generate a random number within a range
int generate_random_id(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Function to check if the UID or GID already exists
bool id_exists(int id, const std::string& file_path, bool is_uid) {
    std::ifstream file(file_path);
    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find(":");
        std::string id_str = line.substr(0, pos);
        int file_id = std::stoi(id_str);
        if (file_id == id) {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    std::string username;
    bool create_home = false;
    std::string shell = "/bin/bash";
    std::string group_name = "users"; // Default group
    bool help_flag = false;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            help_flag = true;
        } else if (arg == "--create-home" || arg == "-m") {
            create_home = true;
        } else if (arg == "--shell" || arg == "-s") {
            if (i + 1 < argc) {
                shell = argv[++i];
            } else {
                std::cerr << "Error: Missing shell argument after --shell" << std::endl;
                return 1;
            }
        } else if (arg == "--group" || arg == "-g") {
            if (i + 1 < argc) {
                group_name = argv[++i];
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

    if (user_exists(username)) {
        std::cerr << "Error: User " << username << " already exists." << std::endl;
        return 1;
    }

    // Generate random UID and GID in the range 1000-9999
    srand(time(0));  // Initialize random seed
    int uid = generate_random_id(1000, 9999);
    int gid = generate_random_id(1000, 9999);

    // Ensure the UID and GID do not already exist
    while (id_exists(uid, "/etc/passwd", true)) {
        uid = generate_random_id(1000, 9999);
    }

    while (id_exists(gid, "/etc/group", false)) {
        gid = generate_random_id(1000, 9999);
    }

    // Create the group if it doesn't exist
    if (!group_exists(group_name)) {
        if (!create_group(group_name, gid)) {
            return 1;
        }
    }

    // Create the user
    std::cout << "Creating user " << username << "..." << std::endl;

    // Create the home directory
    if (create_home) {
        if (!create_home_directory(username)) {
            return 1;
        }
    }

    // Hash the password
    std::string password;
    std::cout << "Set password for " << username << ": ";
    std::getline(std::cin, password);
    std::string password_hash = hash_password(password);

    // Add the user to /etc/passwd
    if (!add_user_to_passwd(username, "/home/" + username, shell, uid, gid)) {
        return 1;
    }

    // Add the user to /etc/shadow
    if (!add_user_to_shadow(username, password_hash)) {
        return 1;
    }

    std::cout << "User " << username << " created successfully!" << std::endl;
    return 0;
}