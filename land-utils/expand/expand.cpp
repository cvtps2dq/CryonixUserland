//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <getopt.h>

// Function to print the usage instructions
void print_usage() {
    std::cout << "Usage: expand [-t tab_width] [file...]\n"
              << "Convert tabs to spaces in the input.\n"
              << "  -t tab_width  specify the number of spaces to replace each tab (default is 8)\n";
}

// Function to expand tabs to spaces
std::string expand_tabs(const std::string& line, const int tab_width) {
    std::string result;
    for (const char c : line) {
        if (c == '\t') {
            result.append(tab_width, ' ');
        } else {
            result.push_back(c);
        }
    }
    return result;
}

// Function to process a file or standard input
void process_input(std::istream& input_stream, const int tab_width) {
    std::string line;
    while (std::getline(input_stream, line)) {
        std::cout << expand_tabs(line, tab_width) << "\n";
    }
}

int main(const int argc, char* argv[]) {
    int tab_width = 8;  // Default tab width
    std::vector<std::string> files;

    // Parsing command-line options
    int opt;
    while ((opt = getopt(argc, argv, "t:")) != -1) {
        switch (opt) {
            case 't':
                tab_width = std::stoi(optarg);  // Set tab width
            break;
            default:
                print_usage();
            return 1;
        }
    }

    // Collect the remaining arguments as files
    for (int i = optind; i < argc; ++i) {
        files.emplace_back(argv[i]);
    }

    // If no files are provided, read from standard input
    if (files.empty()) {
        process_input(std::cin, tab_width);
    } else {
        for (const auto& file : files) {
            std::ifstream file_stream(file);
            if (!file_stream) {
                std::cerr << "Error opening file: " << file << "\n";
                return 1;
            }
            process_input(file_stream, tab_width);
        }
    }

    return 0;
}