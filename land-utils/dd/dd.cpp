#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <chrono>
#include <cstring>

#define DEFAULT_BLOCK_SIZE 512

void print_progress(size_t copied, size_t total_size, std::chrono::steady_clock::time_point start_time) {
    float percent = (total_size > 0) ? (copied * 100.0 / total_size) : 0;
    auto elapsed = std::chrono::steady_clock::now() - start_time;
    double seconds_elapsed = std::chrono::duration<double>(elapsed).count();
    double speed = (seconds_elapsed > 0) ? (copied / seconds_elapsed) : 0;
    double eta = (speed > 0) ? ((total_size - copied) / speed) : 0;

    std::cout << "\rProgress: " << percent << "% | Speed: " << speed / (1024 * 1024) << " MB/s | ETA: " << eta << "s      " << std::flush;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file> [block size]" << std::endl;
        return 1;
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];
    size_t block_size = (argc > 3) ? std::stoi(argv[3]) : DEFAULT_BLOCK_SIZE;

    int in_fd = open(input_file, O_RDONLY);
    if (in_fd < 0) {
        perror("Error opening input file");
        return 1;
    }

    int out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (out_fd < 0) {
        perror("Error opening output file");
        close(in_fd);
        return 1;
    }

    struct stat file_stat;
    if (fstat(in_fd, &file_stat) < 0) {
        perror("Error getting file size");
        close(in_fd);
        close(out_fd);
        return 1;
    }

    size_t total_size = file_stat.st_size;
    size_t copied = 0;
    char *buffer = new char[block_size];

    auto start_time = std::chrono::steady_clock::now();
    ssize_t bytes_read;

    while ((bytes_read = read(in_fd, buffer, block_size)) > 0) {
        ssize_t bytes_written = write(out_fd, buffer, bytes_read);
        if (bytes_written < 0) {
            perror("Error writing to output file");
            delete[] buffer;
            close(in_fd);
            close(out_fd);
            return 1;
        }
        copied += bytes_written;
        print_progress(copied, total_size, start_time);
    }

    delete[] buffer;
    close(in_fd);
    close(out_fd);
    std::cout << "\nCopy complete!" << std::endl;
    return 0;
}
