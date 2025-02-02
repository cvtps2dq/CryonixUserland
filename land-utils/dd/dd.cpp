//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <atomic>
#include <thread>
#include <cstring>
#include <sys/stat.h>

// ANSI Colors
namespace Colors {
    constexpr auto RESET = "\033[0m";
    constexpr auto GREEN = "\033[32m";
    constexpr auto YELLOW = "\033[33m";
    constexpr auto CYAN = "\033[36m";
    constexpr auto BLUE = "\033[34m";
}

// Configuration structure
struct Config {
    std::string input = "/dev/stdin";
    std::string output = "/dev/stdout";
    size_t block_size = 4 * 1024 * 1024;  // Default 4MB
    size_t count = 0;  // 0 means read until EOF
    bool progress = false;
};

// Status Indicator (for progress display)
class StatusIndicator {
    std::atomic<bool> running{false};
    std::thread status_thread;
    std::atomic<size_t> bytes_processed{0};
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    size_t block_size;

    void display_loop() const {
        using namespace std::chrono;
        constexpr auto update_interval = 500ms;

        size_t last_bytes = bytes_processed.load();
        auto last_time = steady_clock::now();

        while (running) {
            std::this_thread::sleep_for(update_interval);

            auto now = steady_clock::now();
            size_t current_bytes = bytes_processed.load();

            double elapsed = duration<double>(now - last_time).count();
            double total_elapsed = duration<double>(now - start_time).count();
            size_t bytes_diff = current_bytes - last_bytes;
            double speed = bytes_diff / (elapsed * 1024 * 1024); // MB/s

            std::cerr << "\r" << Colors::CYAN << "[STATUS] " << Colors::RESET
                      << Colors::GREEN << "Transferred: " << current_bytes / (1024 * 1024) << " MB " << Colors::RESET
                      << Colors::YELLOW << "Speed: " << std::fixed << std::setprecision(2)
                      << speed << " MB/s" << Colors::RESET
                      << Colors::BLUE << " Elapsed: " << total_elapsed << "s " << Colors::RESET;

            last_bytes = current_bytes;
            last_time = now;
        }
    }

public:
    explicit StatusIndicator(size_t bs) : block_size(bs) {}

    void start() {
        start_time = std::chrono::steady_clock::now();
        running = true;
        status_thread = std::thread(&StatusIndicator::display_loop, this);
    }

    void add_bytes(size_t bytes) {
        bytes_processed += bytes;
    }

    void stop() {
        running = false;
        if (status_thread.joinable()) status_thread.join();

        auto end_time = std::chrono::steady_clock::now();
        double total_seconds = std::chrono::duration<double>(end_time - start_time).count();

        std::cerr << "\n" << Colors::GREEN << "[COMPLETE] " << Colors::RESET
                  << "Total time: " << std::fixed << std::setprecision(2) << total_seconds << "s\n";
    }
};

// Parse size (supports K, M, G suffixes)
size_t parse_size(const std::string& s) {
    size_t num = 0, i = 0;
    bool digits = false;

    for (; i < s.size() && std::isdigit(static_cast<unsigned char>(s[i])); ++i) {
        num = num * 10 + (s[i] - '0');
        digits = true;
    }

    if (!digits) throw std::invalid_argument("Invalid size format");

    char suffix = (i < s.size()) ? std::toupper(static_cast<unsigned char>(s[i])) : '\0';

    switch (suffix) {
        case 'K': return num * 1024;
        case 'M': return num * 1024 * 1024;
        case 'G': return num * 1024 * 1024 * 1024;
        case '\0': return num;
        default: throw std::invalid_argument("Invalid suffix");
    }
}

// Parse command-line arguments
Config parse_args(int argc, char** argv) {
    Config cfg;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        try {
            if (arg.find("from=") == 0) {
                cfg.input = arg.substr(5);
            } else if (arg.find("to=") == 0) {
                cfg.output = arg.substr(3);
            } else if (arg.find("block_size=") == 0) {
                cfg.block_size = parse_size(arg.substr(11));
            } else if (arg.find("count=") == 0) {
                cfg.count = std::stoull(arg.substr(6));
            } else if (arg == "status=progress") {
                cfg.progress = true;
            } else {
                throw std::invalid_argument("Unknown argument");
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing '" << arg << "': " << e.what() << "\n";
            std::exit(EXIT_FAILURE);
        }
    }

    if (cfg.block_size == 0) {
        std::cerr << "Error: Block size cannot be zero\n";
        std::exit(EXIT_FAILURE);
    }

    return cfg;
}

// Transfer data (core function)
void transfer_data(const Config& cfg) {
    int in_fd = open(cfg.input.c_str(), O_RDONLY);
    int out_fd = open(cfg.output.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (in_fd < 0) throw std::runtime_error("Failed to open input file: " + cfg.input);
    if (out_fd < 0) throw std::runtime_error("Failed to open output file: " + cfg.output);

    std::vector<char> buffer(cfg.block_size);
    StatusIndicator status(cfg.block_size);
    if (cfg.progress) status.start();

    size_t total_bytes = 0;
    size_t blocks_written = 0;

    while (cfg.count == 0 || blocks_written < cfg.count) {
        ssize_t bytes_read = read(in_fd, buffer.data(), cfg.block_size);
        if (bytes_read <= 0) break; // EOF or error

        ssize_t bytes_written = 0;
        while (bytes_written < bytes_read) {
            ssize_t result = write(out_fd, buffer.data() + bytes_written, bytes_read - bytes_written);
            if (result <= 0) throw std::runtime_error("Write error");

            bytes_written += result;
        }

        total_bytes += bytes_read;
        blocks_written++;
        if (cfg.progress) status.add_bytes(bytes_read);
    }

    if (cfg.progress) status.stop();

    std::cerr << Colors::GREEN << blocks_written << " records in\n"
              << Colors::BLUE << blocks_written << " records out\n"
              << Colors::YELLOW << total_bytes << " bytes transferred\n"
              << Colors::RESET;

    close(in_fd);
    close(out_fd);
}

// Main function
int main(int argc, char** argv) {
    std::cout << "Cryonix Data-Definition v2.0 (Optimized)\n";

    try {
        Config cfg = parse_args(argc, argv);
        transfer_data(cfg);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}