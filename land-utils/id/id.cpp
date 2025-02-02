//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <vector>
#include <sstream>

// ANSI Color Codes for Beautiful Output
#define RESET  "\033[0m"
#define BOLD   "\033[1m"
#define GREEN  "\033[32m"
#define BLUE   "\033[34m"
#define CYAN   "\033[36m"
#define YELLOW "\033[33m"

void print_usage() {
    std::cout << "Usage: cryonix_id [--color]\n"
              << "Display user and group IDs.\n"
              << "  --color   Show output with beautiful formatting.\n";
}

// Function to get group names
std::vector<std::string> get_groups(uid_t uid, gid_t gid) {
    int ngroups = 0;
    getgrouplist(getpwuid(uid)->pw_name, gid, nullptr, &ngroups);

    std::vector<gid_t> groups(ngroups);
    getgrouplist(getpwuid(uid)->pw_name, gid, groups.data(), &ngroups);

    std::vector<std::string> group_names;
    for (gid_t g : groups) {
        struct group* grp = getgrgid(g);
        if (grp) {
            group_names.push_back(std::string(grp->gr_name) + " (" + std::to_string(g) + ")");
        }
    }
    return group_names;
}

void display_id_info(bool color) {
    uid_t uid = getuid();
    gid_t gid = getgid();

    struct passwd* pwd = getpwuid(uid);
    struct group* grp = getgrgid(gid);

    if (!pwd || !grp) {
        std::cerr << "Error: Failed to retrieve user or group information.\n";
        exit(1);
    }

    // Get groups
    std::vector<std::string> groups = get_groups(uid, gid);

    // Build output
    std::stringstream output;

    if (color) {
        output << BOLD << GREEN << "User ID   : " << RESET << CYAN << pwd->pw_name << " (" << uid << ")" << RESET << "\n"
               << BOLD << GREEN << "Group ID  : " << RESET << CYAN << grp->gr_name << " (" << gid << ")" << RESET << "\n"
               << BOLD << YELLOW << "Groups    : " << RESET;

        for (size_t i = 0; i < groups.size(); ++i) {
            output << BLUE << groups[i] << RESET;
            if (i != groups.size() - 1) output << ", ";
        }
    } else {
        output << "uid=" << uid << "(" << pwd->pw_name << ") "
               << "gid=" << gid << "(" << grp->gr_name << ") "
               << "groups=";
        for (size_t i = 0; i < groups.size(); ++i) {
            output << groups[i];
            if (i != groups.size() - 1) output << ",";
        }
    }

    std::cout << output.str() << std::endl;
}

int main(int argc, char* argv[]) {
    bool color = false;

    // Parse arguments
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--color") {
            color = true;
        } else {
            print_usage();
            return 1;
        }
    }

    // Display ID information
    display_id_info(color);

    return 0;
}