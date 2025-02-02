//
// Created by cv2 on 02.02.2025.
//


#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>

void print_usage() {
    std::cout << "Usage: cryonix_head [OPTIONS] FILE\n"
              << "Display the first part of a file.\n"
              << "Options:\n"
              << "  -n NUM     Output the first NUM lines of the file (default: 10)\n";
}

// Function to read and display the first N lines of a file
void display_head(const std::string& filename, const int num_lines) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Cannot open file '" << filename << "'\n";
        exit(1);
    }

    std::string line;
    int count = 0;

    while (std::getline(file, line) && count < num_lines) {
        std::cout << line << std::endl;
        count++;
    }
}

// Easter egg function to display meme message
void easter_egg() {
    std::cout << "so no head? :(" << std::endl;
}

int main(const int argc) {
    // Check if no arguments were passed
    if (argc == 1) {
        easter_egg();
        return 0;
    }

    int num_lines = 10;  // Default number of lines to display
    std::string filename;

    // Parse command-line options
    for (int i = 1; i < argc; ++i) {
        if (std::string arg = argv[i]; arg == "-n" && i + 1 < argc) {
            if (std::stringstream ss(argv[++i]); !(ss >> num_lines)) {
                std::cerr << "Error: Invalid number for -n option\n";
                return 1;
            }
        } else {
            filename = arg;
        }
    }

    if (filename.empty()) {
        print_usage();
        return 1;
    }

    // Display the first N lines of the file
    display_head(filename, num_lines);

    return 0;
}