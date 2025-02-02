//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <filesystem>

void print_usage() {
    std::cout << "Usage: cryonix_link SOURCE TARGET\n"
              << "Create a hard link from SOURCE to TARGET.\n";
}

void create_hard_link(const std::string& source, const std::string& target) {
    try {
        std::filesystem::create_hard_link(source, target);
        std::cout << "\033[1;32mSuccessfully linked " << source << " -> " << target << "\033[0m\n";
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "\033[1;31mError: " << e.what() << "\033[0m\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        print_usage();
        return 1;
    }

    std::string source = argv[1];
    std::string target = argv[2];

    create_hard_link(source, target);
    return 0;
}