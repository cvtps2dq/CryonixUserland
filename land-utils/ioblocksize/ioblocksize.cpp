//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <filesystem>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <unistd.h>

namespace fs = std::filesystem;

void print_usage() {
    std::cout << "Usage: cryonix_ioblksize [FILE]...\n"
              << "Show the I/O block size of files or directories.\n";
}

std::string format_size(size_t size) {
    constexpr size_t KB = 1024;
    constexpr size_t MB = KB * 1024;
    constexpr size_t GB = MB * 1024;

    if (size >= GB) return std::to_string(size / GB) + "G";
    if (size >= MB) return std::to_string(size / MB) + "M";
    if (size >= KB) return std::to_string(size / KB) + "K";
    return std::to_string(size) + "B";
}

void get_block_size(const fs::path& path) {
    struct stat file_stat;

    if (stat(path.c_str(), &file_stat) == -1) {
        std::cerr << "\033[1;31mError: Cannot access " << path << "\033[0m\n";
        return;
    }

    // Get file system block size
    struct statfs fs_stat;
    if (statfs(path.c_str(), &fs_stat) == -1) {
        std::cerr << "\033[1;31mError: Unable to get filesystem info for " << path << "\033[0m\n";
        return;
    }

    // Get I/O block size
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "\033[1;31mError: Cannot open " << path << "\033[0m\n";
        return;
    }

    unsigned int block_size;
    if (ioctl(fd, BLKBSZGET, &block_size) == -1) {
        block_size = file_stat.st_blksize;  // Fallback if ioctl fails
    }
    close(fd);

    // Print results
    std::cout << "\033[1;34m" << path.filename() << "\033[0m\n";
    std::cout << "  \033[1;32mOptimal I/O block size: \033[0m" << format_size(file_stat.st_blksize) << "\n";
    std::cout << "  \033[1;36mFilesystem block size:  \033[0m" << format_size(fs_stat.f_bsize) << "\n";
    std::cout << "  \033[1;33mDevice I/O block size:  \033[0m" << format_size(block_size) << "\n";
    std::cout << "--------------------------------------------\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        get_block_size(argv[i]);
    }

    return 0;
}