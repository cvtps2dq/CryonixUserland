//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

void show_help() {
    std::cout << "Usage: cryonix_wc [options] [file...]\n"
              << "Options:\n"
              << "  -l, --lines        print the number of lines\n"
              << "  -w, --words        print the number of words\n"
              << "  -c, --bytes        print the number of bytes\n"
              << "  --help             display this help message\n";
}

bool is_whitespace(char ch) {
    return std::isspace(static_cast<unsigned char>(ch));
}

void count_file(const std::string& file_path, bool count_lines, bool count_words, bool count_bytes) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << file_path << std::endl;
        return;
    }

    std::string line;
    int line_count = 0, word_count = 0, byte_count = 0;

    while (std::getline(file, line)) {
        line_count++;
        byte_count += line.size();

        std::istringstream word_stream(line);
        std::string word;
        while (word_stream >> word) {
            word_count++;
        }
    }

    if (count_lines) {
        std::cout << line_count << " ";
    }
    if (count_words) {
        std::cout << word_count << " ";
    }
    if (count_bytes) {
        std::cout << byte_count << " ";
    }
    std::cout << file_path << std::endl;
}

void count_stdin(bool count_lines, bool count_words, bool count_bytes) {
    std::string line;
    int line_count = 0, word_count = 0, byte_count = 0;

    while (std::getline(std::cin, line)) {
        line_count++;
        byte_count += line.size();

        std::istringstream word_stream(line);
        std::string word;
        while (word_stream >> word) {
            word_count++;
        }
    }

    if (count_lines) {
        std::cout << line_count << " ";
    }
    if (count_words) {
        std::cout << word_count << " ";
    }
    if (count_bytes) {
        std::cout << byte_count << " ";
    }
    std::cout << "[stdin]" << std::endl;
}

int main(int argc, char* argv[]) {
    bool count_lines = false, count_words = false, count_bytes = false;
    std::vector<std::string> files;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--help" || arg == "-h") {
            show_help();
            return 0;
        } else if (arg == "--lines" || arg == "-l") {
            count_lines = true;
        } else if (arg == "--words" || arg == "-w") {
            count_words = true;
        } else if (arg == "--bytes" || arg == "-c") {
            count_bytes = true;
        } else {
            files.push_back(arg);
        }
    }

    if (!count_lines && !count_words && !count_bytes) {
        count_lines = count_words = count_bytes = true;
    }

    // If no files are given, read from stdin
    if (files.empty()) {
        count_stdin(count_lines, count_words, count_bytes);
    } else {
        // Process each file
        for (const auto& file : files) {
            count_file(file, count_lines, count_words, count_bytes);
        }
    }

    return 0;
}