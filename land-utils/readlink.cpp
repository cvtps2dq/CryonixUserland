//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>

// Define color codes for ANSI
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

// Function to print the resolved symbolic link with colors
void print_colored_readlink(const std::string& symlink_path, bool color_enabled) {
    char resolved_path[4096]; // Buffer to store the resolved symlink path

    // Get the resolved symbolic link
    ssize_t length = readlink(symlink_path.c_str(), resolved_path, sizeof(resolved_path) - 1);

    if (length == -1) {
        std::cerr << "Error: Unable to resolve symlink for '" << symlink_path << "'\n";
        return;
    }

    // Null-terminate the resolved path
    resolved_path[length] = '\0';

    // If color is enabled, print the resolved path in color
    if (color_enabled) {
        std::cout << BOLD << GREEN << "Resolved Symlink: " << RESET
                  << YELLOW << resolved_path << RESET << std::endl;
    } else {
        std::cout << resolved_path << std::endl;
    }
}

// Function to print the help message
void print_usage() {
    std::cout << "Usage: cryonix_readlink [OPTIONS] SYMLINK\n"
              << "Resolve and print the target of a symbolic link.\n\n"
              << "  -h, --help               Display this help message\n"
              << "  -c, --color              Enable color output (default: enabled)\n"
              << "  -n, --no-color           Disable color output\n";
}

int main(int argc, char* argv[]) {
    bool color_enabled = true;  // Default is to use colors
    bool show_help = false;
    std::string symlink_path;

    // Parse flags and arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            show_help = true;
            break;
        } else if (arg == "-n" || arg == "--no-color") {
            color_enabled = false;
        } else if (arg == "-c" || arg == "--color") {
            color_enabled = true;
        } else {
            symlink_path = arg;  // Capture the symbolic link path
        }
    }

    // Show help if requested
    if (show_help) {
        print_usage();
        return 0;
    }

    // Check if symlink path is provided
    if (symlink_path.empty()) {
        std::cerr << "Error: Missing symbolic link path argument.\n";
        print_usage();
        return 1;
    }

    // Resolve and print the symlink
    print_colored_readlink(symlink_path, color_enabled);

    return 0;
}
