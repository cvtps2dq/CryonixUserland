//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iterator>

// Function to print the usage instructions
void print_usage() {
    std::cout << "Usage: fold [OPTIONS] [FILE]\n"
              << "Fold each input line to fit within a specified width.\n"
              << "Options:\n"
              << "  -w WIDTH  Set the maximum line width (default 80).\n"
              << "  -s        Break lines at spaces instead of cutting in the middle of words.\n";
}

// Function to read input from a file or stdin
std::string read_input(const std::string& filename = "") {
    std::ostringstream ss;
    if (filename.empty()) {
        ss << std::cin.rdbuf();
    } else {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << "\n";
            exit(1);
        }
        ss << file.rdbuf();
    }
    return ss.str();
}

// Function to fold the text into lines of a specified width
void fold_text(const std::string& text, int width, bool break_at_spaces) {
    std::istringstream ss(text);
    std::string word;
    std::string current_line;

    while (ss >> word) {
        if (break_at_spaces) {
            // Check if the current word will fit in the current line
            if (current_line.length() + word.length() + 1 > width) {
                // Print the current line and start a new one
                std::cout << current_line << "\n";
                current_line = word;
            } else {
                if (!current_line.empty()) {
                    current_line += " ";
                }
                current_line += word;
            }
        } else {
            // Simple break without considering spaces (cut the line)
            if (current_line.length() + word.length() > width) {
                // Print the current line and start a new one
                std::cout << current_line << "\n";
                current_line = word;
            } else {
                current_line += word;
            }
        }
    }

    // Print the last line
    if (!current_line.empty()) {
        std::cout << current_line << "\n";
    }
}

int main(const int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    int width = 80;       // Default width
    bool break_at_spaces = false; // -s flag
    std::string filename;

    // Parsing command-line options
    for (int i = 1; i < argc; ++i) {
        if (std::string arg = argv[i]; arg == "-w" && i + 1 < argc) {
            width = std::stoi(argv[++i]);
        } else if (arg == "-s") {
            break_at_spaces = true;
        } else {
            filename = arg;
        }
    }

    // Read input text
    const std::string input_text = read_input(filename);

    // Fold and print the text
    fold_text(input_text, width, break_at_spaces);

    return 0;
}