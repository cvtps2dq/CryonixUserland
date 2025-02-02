//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>

void shred_file(const std::string& filename, int passes) {
    std::ofstream file;
    file.open(filename, std::ios::out | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for shredding: " << filename << std::endl;
        return;
    }

    // File size to know how many bytes we need to overwrite
    struct stat file_stat;
    if (stat(filename.c_str(), &file_stat) == -1) {
        std::cerr << "Error: Could not stat file: " << filename << std::endl;
        return;
    }
    size_t file_size = file_stat.st_size;

    // Shred the file by overwriting with random data
    for (int pass = 0; pass < passes; ++pass) {
        std::cout << "Shredding pass " << (pass + 1) << "..." << std::endl;

        file.seekp(0);
        for (size_t i = 0; i < file_size; ++i) {
            char random_byte = rand() % 256;
            file.put(random_byte);
        }
    }

    // Close the file
    file.close();

    // Delete the file
    if (remove(filename.c_str()) != 0) {
        std::cerr << "Error: Could not delete file: " << filename << std::endl;
        return;
    }

    std::cout << "File " << filename << " successfully shredded and deleted." << std::endl;
}

// Function to print the help message
void print_usage() {
    std::cout << "Usage: cryonix_shred [options] file...\n"
              << "Securely overwrite and delete files.\n\n"
              << "Options:\n"
              << "  -n, --passes NUM        Number of overwrite passes (default: 3)\n"
              << "  -f, --force             Ignore warnings and proceed\n"
              << "  -h, --help              Display this help message\n";
}

// Function to check if the file is a dangerous file
bool is_dangerous_file(const std::string& filename) {
    return filename == "/" || filename == "/etc/passwd" || filename == "/etc/shadow";
}

int main(int argc, char* argv[]) {
    int passes = 3;
    bool force = false;
    std::string filename;

    // Parse flags and arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            print_usage();
            return 0;
        } else if (arg == "-n" || arg == "--passes") {
            if (i + 1 < argc) {
                passes = std::stoi(argv[++i]);
            }
        } else if (arg == "-f" || arg == "--force") {
            force = true;
        } else {
            filename = arg;
        }
    }

    if (filename.empty()) {
        std::cerr << "Error: No file specified." << std::endl;
        print_usage();
        return 1;
    }

    // Check for dangerous files
    if (is_dangerous_file(filename) && !force) {
        std::cerr << "Warning: You are attempting to shred a dangerous file: " << filename << std::endl;
        std::cout << "Are you sure you want to proceed? (y/n): ";
        char confirm;
        std::cin >> confirm;
        if (confirm != 'y' && confirm != 'Y') {
            std::cout << "Shredding operation aborted." << std::endl;
            return 1;
        }
    }

    // If the file is extremely sensitive, we add an Easter egg
    if (filename == "/") {
        std::cout << "Warning: You are about to shred the root directory! You are about to cause irreversible data loss!" << std::endl;
        std::cout << "Are you sure you want to do this? It's a bad idea. Just saying. (Type 'yes' to confirm): ";
        std::string confirmation;
        std::cin >> confirmation;
        if (confirmation != "yes") {
            std::cout << "Abort! Abort! It’s a bad idea!" << std::endl;
            return 1;
        } else {
            std::cout << "What are you doing?! Are you a monster?" << std::endl;
        }
    }

    // Proceed to shred the file
    shred_file(filename, passes);

    return 0;
}