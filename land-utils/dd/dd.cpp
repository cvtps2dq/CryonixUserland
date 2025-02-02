#include <iostream>
#include <vector>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <chrono>
#include <iomanip>

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
    size_t block_size = 4 * 1024 * 1024; // Default 4MB
    size_t count = 0; // 0 means read until EOF
    bool progress = false;
};

// Parse size (supports K, M, G suffixes)
size_t parse_size(const std::string& s) {
    size_t num = 0, i = 0;
    for (; i < s.size() && std::isdigit(s[i]); ++i) {
        num = num * 10 + (s[i] - '0');
    }
    if (i < s.size()) {
        char suffix = std::toupper(s[i]);
        if (suffix == 'K') return num * 1024;
        if (suffix == 'M') return num * 1024 * 1024;
        if (suffix == 'G') return num * 1024 * 1024 * 1024;
        throw std::invalid_argument("Invalid size suffix");
    }
    return num;
}

// Parse command-line arguments
Config parse_args(int argc, char** argv) {
    Config cfg;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find("from=") == 0) cfg.input = arg.substr(5);
        else if (arg.find("to=") == 0) cfg.output = arg.substr(3);
        else if (arg.find("block_size=") == 0) cfg.block_size = parse_size(arg.substr(11));
        else if (arg.find("count=") == 0) cfg.count = std::stoull(arg.substr(6));
        else if (arg == "status=progress") cfg.progress = true;
        else throw std::invalid_argument("Unknown argument: " + arg);
    }
    return cfg;
}

// Transfer data with debug info
void transfer_data(const Config& cfg) {
    int in_fd = open(cfg.input.c_str(), O_RDONLY);
    if (in_fd < 0) throw std::runtime_error("Failed to open input: " + cfg.input);

    int out_fd = open(cfg.output.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (out_fd < 0) throw std::runtime_error("Failed to open output: " + cfg.output);

    std::vector<char> buffer(cfg.block_size);
    size_t total_bytes = 0, blocks_written = 0;

    auto start_time = std::chrono::steady_clock::now();
    while (cfg.count == 0 || blocks_written < cfg.count) {
        ssize_t bytes_read = read(in_fd, buffer.data(), cfg.block_size);

        if (bytes_read < 0) {
            std::cerr << Colors::RED << "Read error\n" << Colors::RESET;
            break;
        }
        if (bytes_read == 0) break; // EOF

        std::cerr << Colors::CYAN << "Read " << bytes_read << " bytes\n" << Colors::RESET;

        ssize_t bytes_written = 0;
        while (bytes_written < bytes_read) {
            ssize_t result = write(out_fd, buffer.data() + bytes_written, bytes_read - bytes_written);
            if (result < 0) {
                std::cerr << Colors::RED << "Write error\n" << Colors::RESET;
                break;
            }
            bytes_written += result;
        }

        std::cerr << Colors::GREEN << "Wrote " << bytes_written << " bytes\n" << Colors::RESET;

        total_bytes += bytes_written;
        blocks_written++;
    }

    auto end_time = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(end_time - start_time).count();
    double speed = total_bytes / (elapsed * 1024 * 1024);

    std::cerr << Colors::YELLOW << "Total: " << total_bytes << " bytes, Speed: " << speed << " MB/s\n" << Colors::RESET;

    close(in_fd);
    close(out_fd);
}

int main(int argc, char** argv) {
    std::cout << "Cryonix Data-Definition Debug Mode\n";
    try {
        Config cfg = parse_args(argc, argv);
        transfer_data(cfg);
    } catch (const std::exception& e) {
        std::cerr << Colors::RED << "Error: " << e.what() << Colors::RESET << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}