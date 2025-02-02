//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <filesystem>

void print_usage() {
    std::cout << "Usage: mkdir [-p] DIRECTORY...\n"
              << "Create directories.\n\n"
              << "  -p    Create parent directories as needed\n";
}

void create_directory(const std::string& dir, bool create_parents) {
    try {
        if (create_parents) {
            std::filesystem::create_directories(dir);
            std::cout << "\033[1;32mSuccessfully created directories: " << dir << "\033[0m\n";
        } else {
            std::filesystem::create_directory(dir);
            std::cout << "\033[1;32mSuccessfully created directory: " << dir << "\033[0m\n";
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "\033[1;31mError: " << e.what() << "\033[0m\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    bool create_parents = false;
    std::vector<std::string> dirs;

    // Check flags and gather directories
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-p") {
            create_parents = true;
        } else {
            dirs.push_back(arg);
        }
    }

    if (dirs.empty()) {
        print_usage();
        return 1;
    }

    // Create directories
    for (const auto& dir : dirs) {
        create_directory(dir, create_parents);
    }

    return 0;
}