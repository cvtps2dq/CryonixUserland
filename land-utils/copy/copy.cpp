//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

void print_usage() {
    std::cout << "Usage: cryonix_cp [-R] SOURCE DEST\n"
              << "Copy files or directories from SOURCE to DEST. The -R flag allows copying directories recursively.\n";
}

void copy_file(const fs::path& source, const fs::path& dest) {
    // Open source file for reading
    std::ifstream src(source, std::ios::binary);
    if (!src.is_open()) {
        std::cerr << "Error: Could not open source file '" << source << "'\n";
        exit(1);
    }

    // Open destination file for writing
    std::ofstream dst(dest, std::ios::binary);
    if (!dst.is_open()) {
        std::cerr << "Error: Could not open destination file '" << dest << "'\n";
        exit(1);
    }

    // Copy contents of source to destination
    dst << src.rdbuf();  // Copy the content of the source file to the destination file
    std::cout << "File '" << source << "' copied to '" << dest << "'\n";
}

void copy_directory(const fs::path& source_dir, const fs::path& dest_dir) {
    if (!fs::exists(dest_dir)) {
        fs::create_directory(dest_dir);
        std::cout << "Directory '" << dest_dir << "' created\n";
    }

    for (const auto& entry : fs::directory_iterator(source_dir)) {
        const auto& source_path = entry.path();
        auto dest_path = dest_dir / source_path.filename();

        if (fs::is_directory(source_path)) {
            // Recursively copy subdirectory
            copy_directory(source_path, dest_path);
        } else if (fs::is_regular_file(source_path)) {
            // Copy regular files
            copy_file(source_path, dest_path);
        }
    }
}

void cp_clone(const fs::path& source, const fs::path& dest, bool recursive) {
    if (fs::is_directory(source)) {
        if (recursive) {
            // Copy directory recursively
            copy_directory(source, dest);
        } else {
            std::cerr << "Error: Source is a directory. Use the -R flag to copy directories.\n";
            exit(1);
        }
    } else if (fs::is_regular_file(source)) {
        // Copy a single file
        copy_file(source, dest);
    } else {
        std::cerr << "Error: Source is neither a file nor a directory.\n";
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }

    bool recursive = false;
    std::string source_str;
    std::string dest_str;

    // Parse the arguments
    if (argc == 4 && std::string(argv[1]) == "-R") {
        recursive = true;
        source_str = argv[2];
        dest_str = argv[3];
    } else if (argc == 3) {
        source_str = argv[1];
        dest_str = argv[2];
    } else {
        print_usage();
        return 1;
    }

    fs::path source(source_str);
    fs::path dest(dest_str);

    if (!fs::exists(source)) {
        std::cerr << "Error: Source '" << source << "' does not exist.\n";
        return 1;
    }

    cp_clone(source, dest, recursive);

    return 0;
}