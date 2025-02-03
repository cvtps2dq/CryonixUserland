//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

void print_usage() {
    std::cout << "Usage: cryonix_paste [OPTIONS] FILE1 [FILE2 ...]\n"
              << "Merge lines of files side by side.\n\n"
              << "  -d, --delimiter DELIM    Use DELIM instead of TAB for the delimiter\n"
              << "  -h, --help               Display this help message\n";
}

void paste_files(const std::vector<std::string>& files, const std::string& delimiter) {
    std::vector<std::ifstream> file_streams;
    for (const auto& file : files) {
        file_streams.push_back(std::ifstream(file));
        if (!file_streams.back()) {
            std::cerr << "\033[1;31mError: Could not open file " << file << "\033[0m\n";
            return;
        }
    }

    std::vector<std::string> lines(file_streams.size());
    bool done = false;

    while (!done) {
        done = true;
        std::ostringstream output_line;
        for (size_t i = 0; i < file_streams.size(); ++i) {
            if (std::getline(file_streams[i], lines[i])) {
                output_line << lines[i];
                if (i < file_streams.size() - 1) {
                    output_line << delimiter;
                }
                done = false;
            }
        }

        if (!done) {
            std::cout << output_line.str() << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::string delimiter = "\t"; // Default delimiter: Tab

    // Parse flags and arguments
    std::vector<std::string> files;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-d" || arg == "--delimiter") {
            if (i + 1 < argc) {
                delimiter = argv[++i];
            } else {
                std::cerr << "\033[1;31mError: Missing argument for -d option\033[0m\n";
                return 1;
            }
        } else if (arg == "-h" || arg == "--help") {
            print_usage();
            return 0;
        } else {
            files.push_back(arg);
        }
    }

    if (files.empty()) {
        std::cerr << "\033[1;31mError: No input files specified\033[0m\n";
        return 1;
    }

    paste_files(files, delimiter);
    return 0;
}