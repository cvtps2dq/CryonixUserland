//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <filesystem>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void print_usage() {
    std::cout << "Usage: mkfifo FIFO_PATH\n"
              << "Create a named pipe (FIFO) at FIFO_PATH.\n";
}

void create_fifo(const std::string& fifo_path) {
    try {
        if (mkfifo(fifo_path.c_str(), 0666) == -1) {
            perror("Error");
            std::cerr << "\033[1;31mError: Unable to create FIFO " << fifo_path << "\033[0m\n";
            return;
        }
        std::cout << "\033[1;32mSuccessfully created FIFO: " << fifo_path << "\033[0m\n";
    } catch (const std::exception& e) {
        std::cerr << "\033[1;31mError: " << e.what() << "\033[0m\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        print_usage();
        return 1;
    }

    std::string fifo_path = argv[1];
    create_fifo(fifo_path);
    return 0;
}