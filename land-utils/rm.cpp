//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <unistd.h>

// Define color codes for ANSI
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

// Function to perform the delete operation
void remove_file_or_directory(const std::string& path, bool force) {
    try {
        if (std::filesystem::exists(path)) {
            // Check if the user is trying to run 'rm -rf /'
            if (path == "/" && !force) {
                std::cerr << RED << BOLD << "WARNING: You are about to do something NASTY!" << RESET << std::endl;
                std::cerr << RED << "This is a dangerous operation. This command will delete rootdir. WITH ALL YOUR DATA ACROSS ALL DEVICES! Are you sure you want to proceed?" << RESET << std::endl;
                std::cerr << "Use the '-please' flag to force this operation." << RESET << std::endl;
                return;
            }

            // Perform the actual deletion
            std::filesystem::remove_all(path); // This will delete both files and directories
            std::cout << GREEN << "Successfully removed: " << path << RESET << std::endl;
        } else {
            std::cerr << RED << "Error: The file or directory '" << path << "' does not exist." << RESET << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
    }
}

// Function to print the help message
void print_usage() {
    std::cout << "Usage: cryonix_rm [OPTIONS] FILE...\n"
              << "Remove (delete) the specified files or directories.\n\n"
              << "  -h, --help               Display this help message\n"
              << "  -please                  Force the command to execute (for dangerous operations like rm -rf /)\n";
}

int main(int argc, char* argv[]) {
    bool force = false;  // Default is not to force the operation
    bool show_help = false;
    std::string path;

    // Parse flags and arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            show_help = true;
            break;
        } else if (arg == "-please") {
            force = true;
        } else {
            path = arg;  // Capture the file or directory to delete
        }
    }

    // Show help if requested
    if (show_help) {
        print_usage();
        return 0;
    }

    // Check if path argument is provided
    if (path.empty()) {
        std::cerr << RED << "Error: Missing file or directory path argument.\n" << RESET;
        print_usage();
        return 1;
    }

    // Perform the delete operation
    remove_file_or_directory(path, force);

    return 0;
}