//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <filesystem>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sysmacros.h>

void print_usage() {
    std::cout << "Usage: cryonix_mknod FILE TYPE [MAJOR MINOR]\n"
              << "Create a special file (block or character device).\n\n"
              << "  TYPE    Type of the device: 'b' for block, 'c' for character\n"
              << "  MAJOR   Major device number\n"
              << "  MINOR   Minor device number\n";
}

void create_device(const std::string& file, char type, int major, int minor) {
    mode_t mode = (type == 'b') ? S_IFBLK : S_IFCHR;

    try {
        if (mknod(file.c_str(), mode | 0666, makedev(major, minor)) == -1) {
            perror("Error");
            std::cerr << "\033[1;31mError: Unable to create device " << file << "\033[0m\n";
            return;
        }
        std::cout << "\033[1;32mSuccessfully created " << ((type == 'b') ? "block" : "character")
                  << " device: " << file << "\033[0m\n";
    } catch (const std::exception& e) {
        std::cerr << "\033[1;31mError: " << e.what() << "\033[0m\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4 && argc != 6) {
        print_usage();
        return 1;
    }

    std::string file = argv[1];
    char type = argv[2][0];  // 'b' for block, 'c' for character
    int major = 0, minor = 0;

    if (argc == 6) {
        try {
            major = std::stoi(argv[3]);
            minor = std::stoi(argv[4]);
        } catch (const std::exception& e) {
            std::cerr << "\033[1;31mError: Invalid major or minor device number\033[0m\n";
            return 1;
        }
    }

    create_device(file, type, major, minor);
    return 0;
}