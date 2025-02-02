//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <fstream>
#include <string>


// Function to handle reading from a file or stdin and processing unique lines
void process_uniq(std::istream &input, bool only_repeated = false, bool skip_first = false) {
    std::string previous_line, current_line;
    bool first_duplicate_found = false;
    while (std::getline(input, current_line)) {
        // Check if the current line is equal to the previous one
        if (current_line == previous_line) {
            // If only repeated flag is set, print the current line
            if (only_repeated) {
                std::cout << current_line << std::endl;
            } else if (skip_first && !first_duplicate_found) {
                // Skip the first occurrence of the duplicate if skip_first is true
                first_duplicate_found = true;
            } else {
                std::cout << current_line << std::endl;
            }
        } else {
            // Print the previous line only if it wasn't skipped
            if (!skip_first || !first_duplicate_found) {
                std::cout << previous_line << std::endl;
            }
            first_duplicate_found = false;
        }
        previous_line = current_line;
    }
    // Print the last line if it wasn't skipped
    if (!skip_first || !first_duplicate_found) {
        std::cout << previous_line << std::endl;
    }
}

int main(int argc, char* argv[]) {
    bool only_repeated = false;
    bool skip_first = false;
    std::ifstream file_input;

    // Parse command-line arguments for options
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--repeated") {
            only_repeated = true;
        } else if (arg == "--skip-first") {
            skip_first = true;
        } else if (arg == "--help") {
            std::cout << "Usage: cryonix_uniq [--repeated] [--skip-first] [file]" << std::endl;
            return 0;
        } else {
            // Assume it's a file path
            file_input.open(arg);
            if (!file_input.is_open()) {
                std::cerr << "Error: Could not open file " << arg << std::endl;
                return 1;
            }
        }
    }

    // If no file was provided, use stdin
    if (!file_input.is_open()) {
        process_uniq(std::cin, only_repeated, skip_first);
    } else {
        process_uniq(file_input, only_repeated, skip_first);
    }

    return 0;
}