//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>


// Define color codes for ANSI
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define YELLOW  "\033[33m"

// Function to print the current directory with colors
void print_colored_pwd(bool color_enabled) {
    char* cwd = getcwd(nullptr, 0); // Get current working directory
    if (cwd == nullptr) {
        std::cerr << "Error: Unable to retrieve the current directory.\n";
        return;
    }

    // If color is enabled, print the current directory in color
    if (color_enabled) {
        std::cout << BOLD << BLUE << "Current Directory: " << RESET
                  << YELLOW << cwd << RESET << std::endl;
    } else {
        std::cout << cwd << std::endl;
    }

    free(cwd); // Free allocated memory for cwd
}

// Function to print the help message
void print_usage() {
    std::cout << "Usage: cryonix_pwd [OPTIONS]\n"
              << "Print the current working directory with optional colors.\n\n"
              << "  -h, --help               Display this help message\n"
              << "  -c, --color              Enable color output (default: enabled)\n"
              << "  -n, --no-color           Disable color output\n";
}

int main(int argc, char* argv[]) {
    bool color_enabled = true;  // Default is to use colors
    bool show_help = false;

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
        }
    }

    // Show help if requested
    if (show_help) {
        print_usage();
        return 0;
    }

    // Print the current directory with the chosen options
    print_colored_pwd(color_enabled);

    return 0;
}