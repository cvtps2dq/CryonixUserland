//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <string>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>

void print_usage() {
    std::cout << "Usage: cryonix_pinky [OPTIONS] [USER...]\n"
              << "Display user information.\n\n"
              << "  -h, --help               Display this help message\n"
              << "  -s, --short              Display only a brief summary (username and home)\n";
}

std::string get_idle_time(const std::string& username) {
    // For simplicity, we assume the user is logged in and we are checking their idle time
    // This could be expanded to gather real idle time information from `/proc/` or `w`
    return "0 mins"; // Placeholder for idle time, assuming 0 mins for simplicity.
}

void print_user_info(const std::string& username, bool short_summary) {
    struct passwd* pw = getpwnam(username.c_str());
    if (pw == nullptr) {
        std::cerr << "\033[1;31mError: User " << username << " not found\033[0m\n";
        return;
    }

    std::string idle_time = get_idle_time(username);
    std::string terminal = "tty1";  // Placeholder for terminal (e.g., "tty1" or "pts/0")

    std::cout << "\033[1;32mLogin Name:\033[0m " << pw->pw_name << "\n";
    std::cout << "\033[1;34mTerminal:\033[0m " << terminal << "\n";
    std::cout << "\033[1;35mIdle Time:\033[0m " << idle_time << "\n";
    std::cout << "\033[1;36mHome Directory:\033[0m " << pw->pw_dir << "\n";

    if (!short_summary) {
        // Additional information could be displayed here.
        std::cout << "\033[1;33mShell:\033[0m " << pw->pw_shell << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    bool short_summary = false;
    std::vector<std::string> users;

    // Parse flags and arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            print_usage();
            return 0;
        } else if (arg == "-s" || arg == "--short") {
            short_summary = true;
        } else {
            users.push_back(arg);
        }
    }

    if (users.empty()) {
        // No users specified, default to current user
        users.push_back(getlogin());
    }

    for (const auto& user : users) {
        print_user_info(user, short_summary);
    }

    return 0;
}