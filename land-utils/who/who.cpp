//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <utmp.h>

void show_help() {
    std::cout << "Usage: cryonix_who [options]\n"
              << "Options:\n"
              << "  --help            display this help message\n"
              << "  -a, --all         show all logged-in users\n"
              << "  -b, --boot        show system boot time\n"
              << "  -m, --me          show information about the current session\n"
              << "  -q, --quiet       display only the number of users logged in\n";
}

void print_time(time_t timestamp) {
    std::tm* tm_info = std::localtime(&timestamp);
    std::cout << std::put_time(tm_info, "%Y-%m-%d %H:%M:%S");
}

void print_user_info(const struct utmp& entry) {
    std::cout << std::left << std::setw(10) << entry.ut_user
              << std::setw(8) << entry.ut_line
              << std::setw(20);
    print_time(entry.ut_time);
    std::cout << " " << entry.ut_host << std::endl;
}

void print_boot_time() {
    std::ifstream boot_file("/var/run/utmp");
    if (!boot_file.is_open()) {
        std::cerr << "Error: Could not open /var/run/utmp\n";
        return;
    }

    struct utmp entry;
    while (boot_file.read(reinterpret_cast<char*>(&entry), sizeof(entry))) {
        if (entry.ut_type == BOOT_TIME) {
            std::cout << "System boot time: ";
            print_time(entry.ut_time);
            std::cout << std::endl;
            return;
        }
    }
    std::cerr << "Error: Could not find boot time\n";
}

void print_logged_in_users() {
    std::ifstream utmp_file("/var/run/utmp");
    if (!utmp_file.is_open()) {
        std::cerr << "Error: Could not open /var/run/utmp\n";
        return;
    }

    struct utmp entry;
    while (utmp_file.read(reinterpret_cast<char*>(&entry), sizeof(entry))) {
        if (entry.ut_type == USER_PROCESS) {
            print_user_info(entry);
        }
    }
}

void print_current_user_info() {
    struct utmp entry;
    std::string current_user = getlogin();

    if (current_user.empty()) {
        std::cerr << "Error: Could not get current user\n";
        return;
    }

    std::ifstream utmp_file("/var/run/utmp");
    if (!utmp_file.is_open()) {
        std::cerr << "Error: Could not open /var/run/utmp\n";
        return;
    }

    while (utmp_file.read(reinterpret_cast<char*>(&entry), sizeof(entry))) {
        if (entry.ut_type == USER_PROCESS && current_user == entry.ut_user) {
            print_user_info(entry);
            return;
        }
    }
    std::cerr << "Error: Could not find current user session\n";
}

int main(int argc, char* argv[]) {
    bool show_all = false, show_boot = false, show_me = false, show_quiet = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help") {
            show_help();
            return 0;
        } else if (arg == "-a" || arg == "--all") {
            show_all = true;
        } else if (arg == "-b" || arg == "--boot") {
            show_boot = true;
        } else if (arg == "-m" || arg == "--me") {
            show_me = true;
        } else if (arg == "-q" || arg == "--quiet") {
            show_quiet = true;
        } else {
            std::cerr << "Unknown option: " << arg << std::endl;
            show_help();
            return 1;
        }
    }

    if (show_boot) {
        print_boot_time();
    } else if (show_me) {
        print_current_user_info();
    } else if (show_all) {
        print_logged_in_users();
    } else if (show_quiet) {
        int user_count = 0;
        std::ifstream utmp_file("/var/run/utmp");
        if (!utmp_file.is_open()) {
            std::cerr << "Error: Could not open /var/run/utmp\n";
            return 1;
        }

        struct utmp entry;
        while (utmp_file.read(reinterpret_cast<char*>(&entry), sizeof(entry))) {
            if (entry.ut_type == USER_PROCESS) {
                user_count++;
            }
        }

        std::cout << user_count << " user(s) logged in.\n";
    } else {
        print_logged_in_users();
    }

    return 0;
}