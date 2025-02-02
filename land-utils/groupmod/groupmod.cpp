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
#include <unistd.h>
#include <cstring>

void show_help() {
    std::cout << "Usage: cryonix_groupmod [options] groupname\n"
              << "Options:\n"
              << "  -n, --new-name NEW_GROUP_NAME   Change the group name to NEW_GROUP_NAME\n"
              << "  -g, --gid GID                   Change the GID of the group\n"
              << "  --help                          Display this help message\n";
}

bool group_exists(const std::string& group_name) {
    struct group* grp = getgrnam(group_name.c_str());
    return grp != nullptr;
}

bool group_exists_by_gid(int gid) {
    struct group* grp;
    setgrent();
    while ((grp = getgrent()) != nullptr) {
        if (grp->gr_gid == gid) {
            endgrent();
            return true;
        }
    }
    endgrent();
    return false;
}

bool modify_group_in_group_file(const std::string& old_group_name, const std::string& new_group_name, int new_gid) {
    std::ifstream group_file("/etc/group");
    std::ofstream temp_file("/etc/group.tmp");
    if (!group_file.is_open() || !temp_file.is_open()) {
        std::cerr << "Error: Unable to open /etc/group" << std::endl;
        return false;
    }

    std::string line;
    bool modified = false;
    while (std::getline(group_file, line)) {
        std::stringstream ss(line);
        std::string group_name;
        int gid;
        std::string rest;

        // Parse the line in /etc/group
        std::getline(ss, group_name, ':');
        std::getline(ss, rest, ':');
        ss >> gid;
        std::getline(ss, rest);

        if (group_name == old_group_name) {
            modified = true;
            // Modify the group name and/or GID
            if (!new_group_name.empty()) {
                group_name = new_group_name;
            }
            if (new_gid != -1) {
                gid = new_gid;
            }
            temp_file << group_name << ":x:" << gid << ":" << rest;
        } else {
            temp_file << line << std::endl;
        }
    }

    group_file.close();
    temp_file.close();

    if (modified) {
        std::remove("/etc/group");
        std::rename("/etc/group.tmp", "/etc/group");
        return true;
    } else {
        std::cerr << "Error: Group " << old_group_name << " not found" << std::endl;
        return false;
    }
}

int generate_random_gid() {
    // Generate a random GID between 1000 and 65535
    return 1000 + rand() % 64536;
}

int main(int argc, char* argv[]) {
    std::string group_name;
    std::string new_group_name;
    int new_gid = -1;
    bool help_flag = false;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            help_flag = true;
        } else if (arg == "--new-name" || arg == "-n") {
            if (i + 1 < argc) {
                new_group_name = argv[++i];
            } else {
                std::cerr << "Error: Missing new group name argument after --new-name" << std::endl;
                return 1;
            }
        } else if (arg == "--gid" || arg == "-g") {
            if (i + 1 < argc) {
                new_gid = std::stoi(argv[++i]);
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

    if (!group_exists(group_name)) {
        std::cerr << "Error: Group " << group_name << " does not exist." << std::endl;
        return 1;
    }

    // If new GID is provided, check if it's already taken
    if (new_gid != -1 && group_exists_by_gid(new_gid)) {
        std::cerr << "Error: GID " << new_gid << " is already assigned to another group." << std::endl;
        return 1;
    }

    // If no new group name is given, retain the original name
    if (new_group_name.empty()) {
        new_group_name = group_name;
    }

    // Modify the group
    if (modify_group_in_group_file(group_name, new_group_name, new_gid)) {
        std::cout << "Group " << group_name << " successfully modified." << std::endl;
        return 0;
    } else {
        return 1;
    }
}