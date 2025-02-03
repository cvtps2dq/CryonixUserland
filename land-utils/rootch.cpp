//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>

void print_usage() {
    std::cout << "Usage: rootch NEW_ROOT [COMMAND [ARGUMENTS...]]\n"
              << "Change the root directory to NEW_ROOT and optionally execute a command within it.\n";
}

bool validate_directory(const std::string& dir) {
    struct stat stat_buf{};
    if (stat(dir.c_str(), &stat_buf) == -1) {
        std::cerr << "Error: Directory '" << dir << "' does not exist or is inaccessible.\n";
        return false;
    }
    if (!S_ISDIR(stat_buf.st_mode)) {
        std::cerr << "Error: '" << dir << "' is not a directory.\n";
        return false;
    }
    return true;
}

void chroot_clone(const std::string& new_root, char* const* command) {
    // Validate the new root directory
    if (!validate_directory(new_root)) {
        exit(1);
    }

    // Change the root directory for the calling process
    if (chroot(new_root.c_str()) == -1) {
        std::cerr << "Error: Failed to change root to '" << new_root << "'.\n";
        perror("chroot error");
        exit(1);
    }

    // If a command is provided, execute it inside the new root
    if (command != nullptr) {
        if (execvp(command[0], command) == -1) {
            std::cerr << "Error: Failed to execute command inside new root.\n";
            perror("execvp error");
            exit(1);
        }
    }
}

int main(const int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    const std::string new_root = argv[1];

    // If a command is provided, pass it as an array
    char* const* command = argc > 2 ? &argv[2] : nullptr;

    chroot_clone(new_root, command);

    return 0;
}