//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <set>

// Function to get list of currently logged in users
std::set<std::string> get_logged_in_users() {
    std::set<std::string> users;
    std::ifstream wtmp("/var/run/utmp");

    if (!wtmp.is_open()) {
        std::cerr << "Error: Unable to open /var/run/utmp" << std::endl;
        return users;
    }

    std::string line;
    while (std::getline(wtmp, line)) {
        std::istringstream ss(line);
        std::string user;
        ss >> user; // Get the first word which is the username
        if (!user.empty()) {
            users.insert(user);
        }
    }

    return users;
}

// Function to print the users in a single line
void print_users(const std::set<std::string>& users) {
    if (users.empty()) {
        std::cout << "No users currently logged in." << std::endl;
        return;
    }

    for (const auto& user : users) {
        std::cout << user << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    // If the user provides a --help flag, display the usage message
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--help") {
            std::cout << "Usage: cryonix_users [options]" << std::endl;
            std::cout << "Display the list of currently logged in users." << std::endl;
            return 0;
        }
    }

    // Get the list of currently logged in users
    std::set<std::string> users = get_logged_in_users();

    // Print the users in a single line
    print_users(users);

    return 0;
}