//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <filesystem>

void print_usage() {
    std::cout << "Usage: ln [-s] SOURCE TARGET\n"
              << "Create a hard or symbolic link from SOURCE to TARGET.\n\n"
              << "  -s    Create a symbolic link instead of a hard link\n";
}

void create_link(const std::string& source, const std::string& target, bool symbolic) {
    try {
        if (symbolic) {
            std::filesystem::create_symlink(source, target);
            std::cout << "\033[1;32mSuccessfully created symbolic link " << target << " -> " << source << "\033[0m\n";
        } else {
            std::filesystem::create_hard_link(source, target);
            std::cout << "\033[1;32mSuccessfully created hard link " << target << " -> " << source << "\033[0m\n";
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "\033[1;31mError: " << e.what() << "\033[0m\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        print_usage();
        return 1;
    }

    bool symbolic = false;
    std::string source, target;

    if (argc == 4 && std::string(argv[1]) == "-s") {
        symbolic = true;
        source = argv[2];
        target = argv[3];
    } else if (argc == 3) {
        source = argv[1];
        target = argv[2];
    } else {
        print_usage();
        return 1;
    }

    create_link(source, target, symbolic);
    return 0;
}