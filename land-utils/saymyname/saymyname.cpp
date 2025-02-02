#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <vector>

void print_usage() {
    std::cout << "Usage: cryonix_whoami [-f] [--heisenberg]\n"
              << "Display the current logged-in user's username.\n"
              << "-f: Display full name and groups the user is part of.\n";
}

void saymyname(const bool show_full_info) {
    // Get the current user's UID
    const uid_t uid = getuid();

    // Fetch user information based on UID
    if (const passwd* pw = getpwuid(uid); pw != nullptr) {
        // Print the username associated with the UID
        std::cout << pw->pw_name << std::endl;

        if (show_full_info) {
            // Print the full name (pw_gecos field)
            std::cout << "Full Name: " << pw->pw_gecos << std::endl;

            // Fetch the user's groups
            std::vector<int> groups;
            int ngroups = 0;
            if (getgrouplist(pw->pw_name, static_cast<int>(pw->pw_gid), nullptr, &ngroups) == -1) {
                groups.resize(ngroups);
                if (getgrouplist(pw->pw_name, static_cast<int>(pw->pw_gid), groups.data(), &ngroups) == -1) {
                    std::cerr << "Error: Unable to retrieve groups.\n";
                    return;
                }
            }

            std::cout << "Groups: ";
            for (const int gid : groups) {
                if (const group* gr = getgrgid(gid); gr != nullptr) {
                    std::cout << gr->gr_name << " ";
                }
            }
            std::cout << std::endl;
        }
    } else {
        std::cerr << "Error: Unable to retrieve user information.\n";
    }
}

void heisenberg_egg() {
    std::cout << "You god damn right.\n";
}

int main(const int argc, char* argv[]) {
    bool show_full_info = false;
    bool easter_egg = false;

    // Handle command-line arguments
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-f") {
            show_full_info = true;
        } else if (std::string(argv[i]) == "--heisenberg") {
            easter_egg = true;
        } else {
            print_usage();
            return 1;
        }
    }

    if (easter_egg) {
        heisenberg_egg();  // Print the Easter egg
    } else {
        saymyname(show_full_info);  // Call whoami function to display the requested information
    }

    return 0;
}