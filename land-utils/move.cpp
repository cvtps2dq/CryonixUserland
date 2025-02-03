//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

void print_usage() {
    std::cout << "Usage: move SOURCE DEST\n"
              << "Move or rename files and directories.\n";
}

void move_file(const fs::path& source, const fs::path& dest) {
    try {
        fs::rename(source, dest);  // Move the file to the new location
        std::cout << "File '" << source << "' moved to '" << dest << "'\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << "\n";
        exit(1);
    }
}

void move_directory(const fs::path& source, const fs::path& dest) {
    try {
        // Move the directory to the new location
        fs::rename(source, dest);
        std::cout << "Directory '" << source << "' moved to '" << dest << "'\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << "\n";
        exit(1);
    }
}

void mv_clone(const fs::path& source, const fs::path& dest) {
    if (exists(source)) {
        if (is_directory(source)) {
            // Move directory
            move_directory(source, dest);
        } else if (is_regular_file(source)) {
            // Move file
            move_file(source, dest);
        } else {
            std::cerr << "Error: Source is neither a file nor a directory.\n";
            exit(1);
        }
    } else {
        std::cerr << "Error: Source '" << source << "' does not exist.\n";
        exit(1);
    }
}

int main(const int argc, char* argv[]) {
    if (argc != 3) {
        print_usage();
        return 1;
    }

    const fs::path source(argv[1]);
    const fs::path dest(argv[2]);

    mv_clone(source, dest);

    return 0;
}