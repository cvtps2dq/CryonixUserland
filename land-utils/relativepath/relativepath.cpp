//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>

// Define color codes for ANSI
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

// Function to compute the relative path between two paths
std::string get_relative_path(const std::string& from, const std::string& to) {
    std::filesystem::path from_path(from);
    std::filesystem::path to_path(to);

    // Compute the relative path
    std::filesystem::path relative_path = std::filesystem::relative(to_path, from_path);

    return relative_path.string();
}

// Function to print the relative path with optional colors
void print_colored_relpath(const std::string& from, const std::string& to, bool color_enabled) {
    std::string relative_path = get_relative_path(from, to);

    // If color is enabled, print the relative path in color
    if (color_enabled) {
        std::cout << BOLD << GREEN << "Relative Path: " << RESET
                  << YELLOW << relative_path << RESET << std::endl;
    } else {
        std::cout << relative_path << std::endl;
    }
}

// Function to print the help message
void print_usage() {
    std::cout << "Usage: cryonix_relpath [OPTIONS] FROM_PATH TO_PATH\n"
              << "Resolve and print the relative path between two paths.\n\n"
              << "  -h, --help               Display this help message\n"
              << "  -c, --color              Enable color output (default: enabled)\n"
              << "  -n, --no-color           Disable color output\n";
}

int main(int argc, char* argv[]) {
    bool color_enabled = true;  // Default is to use colors
    bool show_help = false;
    std::string from_path;
    std::string to_path;

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
        } else if (from_path.empty()) {
            from_path = arg;  // Capture the 'from' path
        } else {
            to_path = arg;    // Capture the 'to' path
        }
    }

    // Show help if requested
    if (show_help) {
        print_usage();
        return 0;
    }

    // Check if both paths are provided
    if (from_path.empty() || to_path.empty()) {
        std::cerr << "Error: Missing one or both path arguments.\n";
        print_usage();
        return 1;
    }

    // Resolve and print the relative path
    print_colored_relpath(from_path, to_path, color_enabled);

    return 0;
}