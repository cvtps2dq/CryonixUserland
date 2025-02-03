//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void print_usage() {
    std::cout << "Usage: linecompare FILE1 FILE2\n"
              << "Compare two sorted files line by line and output three columns:\n"
              << "1. Lines only in FILE1\n"
              << "2. Lines only in FILE2\n"
              << "3. Lines common to both files\n";
}

void comm_clone(const std::string& file1, const std::string& file2) {
    std::ifstream f1(file1);
    std::ifstream f2(file2);

    if (!f1.is_open()) {
        std::cerr << "Error: Could not open file '" << file1 << "'.\n";
        exit(1);
    }
    if (!f2.is_open()) {
        std::cerr << "Error: Could not open file '" << file2 << "'.\n";
        exit(1);
    }

    std::string line1, line2;
    std::vector<std::string> unique_to_file1, unique_to_file2, common_lines;

    // Read lines from both files
    while (std::getline(f1, line1) && std::getline(f2, line2)) {
        if (line1 < line2) {
            unique_to_file1.push_back(line1);
        } else if (line1 > line2) {
            unique_to_file2.push_back(line2);
        } else {
            common_lines.push_back(line1);
        }
    }

    // Handle remaining lines in file1
    while (std::getline(f1, line1)) {
        unique_to_file1.push_back(line1);
    }

    // Handle remaining lines in file2
    while (std::getline(f2, line2)) {
        unique_to_file2.push_back(line2);
    }

    // Output the results
    for (const auto& line : unique_to_file1) {
        std::cout << line << "\t";
    }
    std::cout << std::endl;

    for (const auto& line : unique_to_file2) {
        std::cout << "\t" << line << "\t";
    }
    std::cout << std::endl;

    for (const auto& line : common_lines) {
        std::cout << "\t\t" << line << std::endl;
    }
}

int main(const int argc, char* argv[]) {
    if (argc != 3) {
        print_usage();
        return 1;
    }

    const std::string file1 = argv[1];
    const std::string file2 = argv[2];

    comm_clone(file1, file2);

    return 0;
}