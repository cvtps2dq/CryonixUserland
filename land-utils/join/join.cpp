//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

void print_usage() {
    std::cout << "Usage: join [-1 FIELD] [-2 FIELD] FILE1 FILE2\n"
              << "Join two sorted files based on a common field.\n";
}

std::vector<std::string> split_line(const std::string& line) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void join_files(const std::string& file1, int field1, const std::string& file2, int field2) {
    std::ifstream f1(file1), f2(file2);
    if (!f1 || !f2) {
        std::cerr << "\033[1;31mError: Could not open files.\033[0m\n";
        return;
    }

    std::map<std::string, std::vector<std::string>> file1_map;
    std::string line;

    // Read first file into map
    while (std::getline(f1, line)) {
        auto tokens = split_line(line);
        if (tokens.size() < field1) continue;
        file1_map[tokens[field1 - 1]] = tokens;
    }

    // Read second file and join on common field
    while (std::getline(f2, line)) {
        auto tokens = split_line(line);
        if (tokens.size() < field2) continue;
        std::string key = tokens[field2 - 1];

        if (file1_map.contains(key)) {
            // Print joined line with color formatting
            std::cout << "\033[1;34m" << key << "\033[0m ";
            for (size_t i = 0; i < file1_map[key].size(); ++i)
                if (i != field1 - 1) std::cout << file1_map[key][i] << " ";
            for (size_t i = 0; i < tokens.size(); ++i)
                if (i != field2 - 1) std::cout << tokens[i] << " ";
            std::cout << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }

    int field1 = 1, field2 = 1;
    std::string file1, file2;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-1" && i + 1 < argc) field1 = std::stoi(argv[++i]);
        else if (arg == "-2" && i + 1 < argc) field2 = std::stoi(argv[++i]);
        else if (file1.empty()) file1 = arg;
        else file2 = arg;
    }

    if (file1.empty() || file2.empty()) {
        print_usage();
        return 1;
    }

    join_files(file1, field1, file2, field2);
    return 0;
}