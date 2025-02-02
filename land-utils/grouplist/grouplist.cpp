//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <iomanip>
#include <sys/types.h>

// Function to print usage instructions
void print_usage() {
    std::cout << "Usage: grouplist [OPTIONS]\n"
              << "Display groups the current user belongs to.\n"
              << "Options:\n"
              << "  --incredible    Display detailed group information with colors and tabs.\n";
}

// Function to print a group in a beautiful way
void print_group_info(const group* grp) {
    // Display group name
    std::cout << "\033[1;32mGroup Name: \033[0m" << std::setw(15) << std::left << grp->gr_name;

    // Display group ID
    std::cout << "\033[1;33mGID: \033[0m" << std::setw(10) << grp->gr_gid << "\n";

    // Display group members
    std::cout << "\033[1;34mMembers: \033[0m";
    if (grp->gr_mem) {
        for (char **member = grp->gr_mem; *member != nullptr; ++member) {
            std::cout << *member;
            if (*(member + 1)) {
                std::cout << ", ";
            }
        }
    } else {
        std::cout << "No members";
    }
    std::cout << "\n";
}

// Function to display basic group names
void display_groups(const std::vector<std::string>& groups, const bool incredible_flag) {
    if (incredible_flag) {
        std::cout << "\033[1;34m========================= Group Info ==========================\033[0m\n";
    }

    for (const auto& group : groups) {
        std::cout << "\033[1;36m" << group << "\033[0m\n"; // Colorize group name
        if (incredible_flag) {
            // Get detailed group info
            if (const struct group* grp = getgrnam(group.c_str()); grp != nullptr) {
                print_group_info(grp);
            } else {
                std::cerr << "Error: Group " << group << " not found.\n";
            }
            std::cout << "\033[1;34m--------------------------------------------------\033[0m\n";
        }
    }

    if (incredible_flag) {
        std::cout << "\033[1;34m=====================================================================\033[0m\n";
    }
}

// Function to get the groups of the current user
std::vector<std::string> get_user_groups(const std::string& username) {
    std::vector<std::string> groups;
    const passwd* pwd = getpwnam(username.c_str());
    if (pwd == nullptr) {
        std::cerr << "Error: User not found.\n";
        exit(1);
    }

    // Get the list of groups for the user
    int ngroups = 0;
    getgrouplist(username.c_str(), pwd->pw_gid, nullptr, &ngroups);
    std::vector<gid_t> group_ids(ngroups);  // Changed to gid_t
    getgrouplist(username.c_str(), pwd->pw_gid, group_ids.data(), &ngroups);

    // Retrieve group names based on group IDs
    for (const gid_t gid : group_ids) {  // Changed to gid_t
        if (group* grp = getgrgid(gid); grp != nullptr) {
            groups.emplace_back(grp->gr_name);
        }
    }

    return groups;
}

int main(const int argc, char** argv) {
    bool incredible_flag = false;

    // Parse command-line options
    for (int i = 1; i < argc; ++i) {
        if (std::string arg = argv[i]; arg == "--incredible") {
            incredible_flag = true;
        } else {
            print_usage();
            return 1;
        }
    }

    // Get current username
    const char* username = getlogin();
    if (username == nullptr) {
        std::cerr << "Error: Unable to retrieve the username.\n";
        return 1;
    }

    // Get the groups for the current user
    const std::vector<std::string> groups = get_user_groups(username);

    // Display the groups and their information
    display_groups(groups, incredible_flag);

    return 0;
}

