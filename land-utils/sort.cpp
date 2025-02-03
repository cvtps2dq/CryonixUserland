//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cctype>
#include <functional>

// Function to ignore case
bool case_insensitive_compare(const std::string& a, const std::string& b) {
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(),
        [](unsigned char ca, unsigned char cb) { return std::tolower(ca) < std::tolower(cb); });
}

// Function to perform numeric comparison
bool numeric_compare(const std::string& a, const std::string& b) {
    double a_num = std::stod(a);
    double b_num = std::stod(b);
    return a_num < b_num;
}

// Function to read input (from a file or stdin)
std::vector<std::string> read_input(const std::string& filename = "") {
    std::vector<std::string> lines;
    std::string line;

    if (filename.empty()) {
        // Read from standard input
        while (std::getline(std::cin, line)) {
            lines.push_back(line);
        }
    } else {
        // Read from a file
        std::ifstream file(filename);
        if (file.is_open()) {
            while (std::getline(file, line)) {
                lines.push_back(line);
            }
        } else {
            std::cerr << "Error: Could not open file: " << filename << std::endl;
            exit(1);
        }
    }

    return lines;
}

// Function to print output
void print_output(const std::vector<std::string>& lines) {
    for (const auto& line : lines) {
        std::cout << line << std::endl;
    }
}

int main(int argc, char* argv[]) {
    bool reverse = false;
    bool numeric = false;
    bool ignore_case = false;
    std::string filename;

    // Parse flags and arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-r") {
            reverse = true;
        } else if (arg == "-n") {
            numeric = true;
        } else if (arg == "-f") {
            ignore_case = true;
        } else {
            filename = arg;  // File input (optional)
        }
    }

    // Read input from file or stdin
    std::vector<std::string> lines = read_input(filename);

    // Sort lines based on specified flags
    if (numeric) {
        std::sort(lines.begin(), lines.end(), numeric_compare);
    } else if (ignore_case) {
        std::sort(lines.begin(), lines.end(), case_insensitive_compare);
    } else {
        std::sort(lines.begin(), lines.end());
    }

    // Reverse the order if -r flag is set
    if (reverse) {
        std::reverse(lines.begin(), lines.end());
    }

    // Print sorted output
    print_output(lines);

    return 0;
}