//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <sys/types.h>
#include <grp.h>

void show_help() {
    std::cout << "Usage: cryonix_groupadd [options] groupname\n"
              << "Options:\n"
              << "  -g, --gid GID          Set the group ID for the new group\n"
              << "  --help                 Display this help message\n";
}

bool group_exists(const std::string& group_name) {
    struct group* grp = getgrnam(group_name.c_str());
    return grp != nullptr;
}

bool add_group_to_group_file(const std::string& group_name, int group_gid) {
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
        std::string existing_group;
        ss >> existing_group;
        if (existing_group == group_name) {
            group_found = true;
            std::cerr << "Error: Group " << group_name << " already exists." << std::endl;
            return false;
        }
        temp_file << line << std::endl;
    }

    if (!group_found) {
        // Create the new group entry
        temp_file << group_name << ":x:" << group_gid << ":\n";
    }

    group_file.close();
    temp_file.close();

    // Replace the original file with the new one
    std::remove("/etc/group");
    std::rename("/etc/group.tmp", "/etc/group");

    return true;
}

int generate_gid() {
    // Generate a random GID in the range 1000-65535
    return 1000 + rand() % 64536;
}

int main(int argc, char* argv[]) {
    std::string group_name;
    int group_gid = -1;
    bool help_flag = false;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            help_flag = true;
        } else if (arg == "--gid" || arg == "-g") {
            if (i + 1 < argc) {
                group_gid = std::stoi(argv[++i]);
            } else {
                std::cerr << "Error: Missing GID argument after --gid" << std::endl;
                return 1;
            }
        } else {
            group_name = arg;
        }
    }

    if (help_flag || group_name.empty()) {
        show_help();
        return 0;
    }

    if (group_exists(group_name)) {
        std::cerr << "Error: Group " << group_name << " already exists." << std::endl;
        return 1;
    }

    // If no GID is specified, generate one
    if (group_gid == -1) {
        group_gid = generate_gid();
    }

    // Add the group to /etc/group
    if (add_group_to_group_file(group_name, group_gid)) {
        std::cout << "Group " << group_name << " with GID " << group_gid << " has been successfully created." << std::endl;
        return 0;
    } else {
        return 1;
    }
}