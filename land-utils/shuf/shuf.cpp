//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <iterator>
#include <sstream>

// Function to shuffle lines of a file or input stream
void shuffle_lines(std::istream& input, size_t num_lines = 0) {
    std::vector<std::string> lines;
    std::string line;

    // Read all lines into a vector
    while (std::getline(input, line)) {
        lines.push_back(line);
    }

    // If num_lines is specified, limit the output
    if (num_lines > 0 && num_lines < lines.size()) {
        lines.resize(num_lines);
    }

    // Shuffle lines using random_shuffle from <algorithm>
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(lines.begin(), lines.end(), g);

    // Output the shuffled lines
    for (const auto& l : lines) {
        std::cout << l << std::endl;
    }
}

// Function to print the usage/help message
void print_usage() {
    std::cout << "Usage: cryonix_shuf [OPTION]... [FILE]...\n"
              << "Shuffle lines of a file or standard input.\n\n"
              << "  -n, --head-count=N     Output at most N lines.\n"
              << "  -h, --help             Display this help message.\n";
}

int main(int argc, char* argv[]) {
    std::string filename;
    size_t num_lines = 0;
    bool is_file = false;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            print_usage();
            return 0;
        } else if (arg == "-n" || arg == "--head-count") {
            if (i + 1 < argc) {
                num_lines = std::stoi(argv[++i]);
            }
        } else {
            filename = arg;
            is_file = true;
        }
    }

    // If no filename is provided, read from stdin
    if (!is_file) {
        shuffle_lines(std::cin, num_lines);
    } else {
        // Open the file and shuffle the lines
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file: " << filename << std::endl;
            return 1;
        }
        shuffle_lines(file, num_lines);
        file.close();
    }

    return 0;
}