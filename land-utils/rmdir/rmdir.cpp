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
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

// Function to remove a directory
void remove_directory(const std::string& path) {
    try {
        if (std::filesystem::exists(path)) {
            // Check if the user is trying to delete the root directory
            if (path == "/") {
                std::cerr << RED << BOLD << "Whoa there! Are you sure you want to delete the root directory?" << RESET << std::endl;
                std::cerr << YELLOW << "This could break everything! You might not be able to turn back!" << RESET << std::endl;
                std::cerr << "Just saying, that's a *really* bad idea..." << RESET << std::endl;
                std::cerr << "Use your brain before proceeding! PLEASE!" << RESET << std::endl;
                return;
            }

            // Perform the directory removal
            std::filesystem::remove(path);
            std::cout << GREEN << "Successfully removed directory: " << path << RESET << std::endl;
        } else {
            std::cerr << RED << "Error: The directory '" << path << "' does not exist." << RESET << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
    }
}

// Function to print the help message
void print_usage() {
    std::cout << "Usage: cryonix_rmdir [OPTIONS] DIRECTORY\n"
              << "Remove empty directories.\n\n"
              << "  -h, --help               Display this help message\n";
}

int main(int argc, char* argv[]) {
    bool show_help = false;
    std::string dir_path;

    // Parse flags and arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            show_help = true;
            break;
        } else {
            dir_path = arg;  // Capture the directory to remove
        }
    }

    // Show help if requested
    if (show_help) {
        print_usage();
        return 0;
    }

    // Check if directory path argument is provided
    if (dir_path.empty()) {
        std::cerr << RED << "Error: Missing directory path argument.\n" << RESET;
        print_usage();
        return 1;
    }

    // Perform the directory removal
    remove_directory(dir_path);

    return 0;
}