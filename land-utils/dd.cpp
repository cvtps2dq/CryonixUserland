#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <chrono>

// datad - data-definition utility from UNIX inside Cryonix Userland
// optimized for POSIX system calls.

#define DEFAULT_BLOCK_SIZE 512

// ANSI Colors
namespace Colors {
    constexpr auto RESET = "\033[0m";
    constexpr auto GREEN = "\033[32m";
    constexpr auto RED = "\033[31m";
    constexpr auto CYAN = "\033[36m";
    constexpr auto YELLOW = "\033[33m";
}

// Configuration structure
struct Config {
    std::string input = "/dev/stdin";
    std::string output = "/dev/stdout";
    size_t block_size = 4096; // Default 4KB
    size_t count = 0; // 0 means read until EOF
    bool progress = true;
};

// Parse size (supports K, M, G suffixes)
size_t parse_size(const std::string& s) {
    size_t num = 0, i = 0;
    for (; i < s.size() && std::isdigit(s[i]); ++i) {
        num = num * 10 + (s[i] - '0');
    }
    if (i < s.size()) {
        const char suffix = static_cast<char>(std::toupper(s[i]));
        if (suffix == 'K') return num * 1024;
        if (suffix == 'M') return num * 1024 * 1024;
        if (suffix == 'G') return num * 1024 * 1024 * 1024;
        throw std::invalid_argument("Invalid size suffix");
    }
    return num;
}

// Parse command-line arguments
Config parse_args(const int argc, char** argv) {
    Config cfg;
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " from=<input file> to=<output file> (optional)block_size=<size> " << std::endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i < argc; ++i) {
        if (std::string arg = argv[i]; arg.find("from=") == 0) cfg.input = arg.substr(5);
        else if (arg.find("to=") == 0) cfg.output = arg.substr(3);
        else if (arg.find("block_size=") == 0) cfg.block_size = parse_size(arg.substr(11));
        else if (arg.find("count=") == 0) cfg.count = std::stoull(arg.substr(6));
        else if (arg == "status=off") cfg.progress = false;
        else throw std::invalid_argument("Unknown argument: " + arg);
    }
    return cfg;
}

void print_progress(const size_t copied, const size_t total_size, const std::chrono::steady_clock::time_point start_time) {
    const double percent = total_size > 0 ? static_cast<double>(copied) * 100.0 / static_cast<double>(total_size) : 0;
    const auto elapsed = std::chrono::steady_clock::now() - start_time;
    const double seconds_elapsed = std::chrono::duration<double>(elapsed).count();
    const double speed = seconds_elapsed > 0 ? static_cast<double>(copied) / seconds_elapsed : 0;
    const double eta = speed > 0 ? static_cast<double>(total_size - copied) / speed : 0;

    std::cout
    << Colors::GREEN << "\rProgress: " << percent << "% "
    << Colors::CYAN << "| Speed: " << speed / (1024 * 1024)<< " MB/s "
    << Colors::YELLOW << "| ETA: " << eta << "s      "
    << Colors::RESET << std::flush;
}

int write_job(const Config& cfg) {
    const char* input_file = cfg.input.c_str();
    const char* output_file = cfg.output.c_str();
    const size_t block_size = cfg.block_size;

    const int in_fd = open(input_file, O_RDONLY);
    if (in_fd < 0) {
        perror("Error opening input file");
        return 1;
    }

    const int out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (out_fd < 0) {
        perror("Error opening output file");
        close(in_fd);
        return 1;
    }

    struct stat file_stat{};
    if (fstat(in_fd, &file_stat) < 0) {
        perror("Error getting file size");
        close(in_fd);
        close(out_fd);
        return 1;
    }

    const size_t total_size = file_stat.st_size;
    size_t copied = 0;
    const auto buffer = new char[block_size];

    const auto start_time = std::chrono::steady_clock::now();
    ssize_t bytes_read;

    while ((bytes_read = read(in_fd, buffer, block_size)) > 0) {
        const ssize_t bytes_written = write(out_fd, buffer, bytes_read);
        if (bytes_written < 0) {
            perror("Error writing to output file");
            delete[] buffer;
            close(in_fd);
            close(out_fd);
            return 1;
        }
        copied += bytes_written;
        if (cfg.progress == true) print_progress(copied, total_size, start_time);
    }

    delete[] buffer;
    close(in_fd);
    close(out_fd);
    std::cout << "\nCopy complete!" << std::endl;
    return 0;
}

int main(const int argc, char *argv[]) {

    std::cout << "Data-Definition Utility. Cryonix Team, 2025\n";
    try {
        const Config cfg = parse_args(argc, argv);
        write_job(cfg);
    } catch (const std::exception& e) {
        std::cerr << Colors::RED << "Error: " << e.what() << Colors::RESET << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
