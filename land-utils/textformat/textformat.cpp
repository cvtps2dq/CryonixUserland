//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

// Function to print the usage instructions
void print_usage() {
    std::cout << "Usage: textformat [OPTIONS] [FILE]\n"
              << "Reformat text by breaking long lines into shorter ones.\n"
              << "Options:\n"
              << "  -w WIDTH  Set the maximum line width (default 80).\n"
              << "  -p        Remove leading spaces from each line.\n"
              << "  -c        Justify the text.\n";
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

// Function to remove leading spaces from a string
std::string remove_leading_spaces(const std::string& line) {
    const size_t start = line.find_first_not_of(" \t");
    return start == std::string::npos ? "" : line.substr(start);
}

// Function to wrap the text into lines of a given width
std::vector<std::string> wrap_text(const std::string &text, int width, bool remove_spaces) {
    std::vector<std::string> lines;
    std::istringstream ss(text);
    std::string word;
    std::string current_line;

    while (ss >> word) {
        // Remove leading spaces if -p flag is set
        if (remove_spaces) {
            word = remove_leading_spaces(word);
        }

        if (current_line.empty()) {
            current_line = word;
        } else {
            if (current_line.size() + 1 + word.size() <= width) {
                current_line += " " + word;
            } else {
                lines.push_back(current_line);
                current_line = word;
            }
        }
    }

    if (!current_line.empty()) {
        lines.push_back(current_line);
    }

    return lines;
}

// Function to justify the text
std::vector<std::string> justify_text(const std::vector<std::string>& lines, const int width) {
    std::vector<std::string> justified_lines;

    for (const auto& line : lines) {
        if (line.size() < width) {
            // Calculate spaces to insert
            int spaces_needed = width - line.size();
            int space_slots = std::count(line.begin(), line.end(), ' ');

            if (space_slots > 0) {
                const int spaces_per_slot = spaces_needed / space_slots;
                int remaining_spaces = spaces_needed % space_slots;

                std::string justified_line;
                size_t start = 0;
                for (const char i : line) {
                    if (i == ' ') {
                        justified_line.append(spaces_per_slot, ' ');
                        if (remaining_spaces > 0) {
                            justified_line.push_back(' ');
                            --remaining_spaces;
                        }
                    } else {
                        justified_line.push_back(i);
                    }
                }
                justified_lines.push_back(justified_line);
            } else {
                justified_lines.push_back(line);
            }
        } else {
            justified_lines.push_back(line);
        }
    }

    return justified_lines;
}

// Function to format and print the output
void format_text(const std::string& text, const int width, const bool remove_spaces, const bool justify) {
    auto lines = wrap_text(text, width, remove_spaces);

    if (justify) {
        lines = justify_text(lines, width);
    }

    for (const auto& line : lines) {
        std::cout << line << "\n";
    }
}

int main(const int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    int width = 80;        // Default width
    bool remove_spaces = false;  // -p flag
    bool justify = false;  // -c flag
    std::string filename;

    // Parsing command-line options
    for (int i = 1; i < argc; ++i) {
        if (std::string arg = argv[i]; arg == "-w" && i + 1 < argc) {
            width = std::stoi(argv[++i]);
        } else if (arg == "-p") {
            remove_spaces = true;
        } else if (arg == "-c") {
            justify = true;
        } else {
            filename = arg;
        }
    }

    // Read input text
    const std::string input_text = read_input(filename);

    // Format and output the text
    format_text(input_text, width, remove_spaces, justify);

    return 0;
}