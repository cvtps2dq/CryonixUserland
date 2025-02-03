//
// Created by cv2 on 02.02.2025.


#include <iostream>
#include <sys/stat.h>
#include <limits.h>
#include <cstring>
#include <vector>
#include <filesystem>
#include <algorithm>

void print_usage() {
    std::cout << "Usage: cryonix_pathchk [OPTIONS] PATHS...\n"
              << "Check whether the specified pathnames are valid.\n\n"
              << "  -h, --help               Display this help message\n"
              << "  -p, --print              Print valid paths only\n";
}

bool is_valid_path(const std::string& path) {
    if (path.empty()) {
        return false;
    }

    // Check if the path length exceeds the maximum allowed length
    if (path.length() > PATH_MAX) {
        return false;
    }

    // Check if the path contains invalid characters (you can expand this if necessary)
    for (char c : path) {
        if (c == '\0' || c == '/' || c == '\\') {
            return false;
        }
    }

    return true;
}

void pathchk(const std::vector<std::string>& paths, bool print_valid_only) {
    bool any_invalid = false;

    for (const auto& path : paths) {
        if (is_valid_path(path)) {
            if (print_valid_only) {
                std::cout << "\033[1;32mValid: " << path << "\033[0m\n";
            } else {
                std::cout << "\033[1;32mValid: " << path << "\033[0m\n";
            }
        } else {
            std::cerr << "\033[1;31mInvalid: " << path << "\033[0m\n";
            any_invalid = true;
        }
    }

    if (any_invalid) {
        std::cerr << "\033[1;33mSome paths are invalid.\033[0m\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    bool print_valid_only = false;
    std::vector<std::string> paths;

    // Parse flags and arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            print_usage();
            return 0;
        } else if (arg == "-p" || arg == "--print") {
            print_valid_only = true;
        } else {
            paths.push_back(arg);
        }
    }

    if (paths.empty()) {
        std::cerr << "\033[1;31mError: No paths specified.\033[0m\n";
        return 1;
    }

    pathchk(paths, print_valid_only);
    return 0;
}
