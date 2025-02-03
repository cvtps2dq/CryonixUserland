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

// Function to print the realpath with colors
void print_colored_realpath(const std::string& path, bool color_enabled) {
    char* real_path = realpath(path.c_str(), nullptr); // Resolve the absolute path

    if (real_path == nullptr) {
        std::cerr << "Error: Unable to resolve path for '" << path << "'\n";
        return;
    }

    // If color is enabled, print the real path in color
    if (color_enabled) {
        std::cout << BOLD << GREEN << "Resolved Absolute Path: " << RESET
                  << YELLOW << real_path << RESET << std::endl;
    } else {
        std::cout << real_path << std::endl;
    }

    free(real_path); // Free the memory allocated by realpath
}

// Function to print the help message
void print_usage() {
    std::cout << "Usage: cryonix_realpath [OPTIONS] PATH\n"
              << "Resolve and print the absolute path of a file or directory.\n\n"
              << "  -h, --help               Display this help message\n"
              << "  -c, --color              Enable color output (default: enabled)\n"
              << "  -n, --no-color           Disable color output\n";
}

int main(int argc, char* argv[]) {
    bool color_enabled = true;  // Default is to use colors
    bool show_help = false;
    std::string path;

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
            path = arg;  // Capture the file or directory path
        }
    }

    // Show help if requested
    if (show_help) {
        print_usage();
        return 0;
    }

    // Check if path argument is provided
    if (path.empty()) {
        std::cerr << "Error: Missing file or directory path argument.\n";
        print_usage();
        return 1;
    }

    // Resolve and print the real path
    print_colored_realpath(path, color_enabled);

    return 0;
}