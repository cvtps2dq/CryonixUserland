//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <cstdlib>
#include <chrono>
#include <iomanip>
#include <thread>
#include <atomic>
#include <unistd.h>
#include <fcntl.h>

// ANSI color codes
namespace Colors {
    constexpr auto RESET = "\033[0m";
    constexpr auto RED = "\033[31m";
    constexpr auto GREEN = "\033[32m";
    constexpr auto YELLOW = "\033[33m";
    constexpr auto BLUE = "\033[34m";
    constexpr auto MAGENTA = "\033[35m";
    constexpr auto CYAN = "\033[36m";
    constexpr auto WHITE = "\033[37m";
    constexpr auto BOLD = "\033[1m";
}

struct Config {
    std::string input = "stdin";
    std::string output = "stdout";
    size_t block_size = 512;
    size_t count = 0;
    bool progress = false;
};

class FileWrapper {
    std::istream* in = &std::cin;
    std::ostream* out = &std::cout;
    std::ifstream infile;
    std::ofstream outfile;

public:
    explicit FileWrapper(const Config& cfg) {
        if (cfg.input != "stdin") {
            infile.open(cfg.input, std::ios::binary);
            std::cout << Colors::BLUE << "Opening input file: " << cfg.input << Colors::RESET << std::endl;
            if (!infile) throw std::runtime_error("Error opening input file");
            in = &infile;
        }

        if (cfg.output != "stdout") {
            outfile.open(cfg.output, std::ios::binary | std::ios::trunc);
            std::cout << Colors::BLUE << "Opening output file: " << cfg.output << Colors::RESET << std::endl;
            if (!outfile) throw std::runtime_error("Error opening output file");
            out = &outfile;
        }
    }

    std::istream& input() const { return *in; }
    std::ostream& output() const { return *out; }
};

class StatusIndicator {
    std::atomic<bool> running{false};
    std::thread status_thread;
    std::atomic<size_t> blocks_processed{0};
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    size_t block_size;

    void display_loop() const {
        using namespace std::chrono;
        constexpr auto update_interval = 500ms;

        size_t last_block_count = blocks_processed.load();
        auto last_time = steady_clock::now();

        while (running) {
            std::this_thread::sleep_for(update_interval);

            auto now = steady_clock::now();
            size_t current_blocks = blocks_processed.load();

            double elapsed = duration<double>(now - last_time).count();
            double total_elapsed = duration<double>(now - start_time).count();

            size_t blocks_diff = current_blocks - last_block_count;
            double speed = (blocks_diff * block_size) / (elapsed * 1024 * 1024); // MB/s

            std::cerr << "\r" << Colors::CYAN << "[STATUS] " << Colors::RESET
                      << Colors::GREEN << "Blocks: " << current_blocks << " " << Colors::RESET
                      << Colors::YELLOW << "Speed: " << std::fixed << std::setprecision(2)
                      << speed << " MB/s" << Colors::RESET
                      << Colors::MAGENTA << " Total Elapsed: " << total_elapsed << "s " << Colors::RESET
                      << std::string(10, ' '); // Clear extra characters

            last_block_count = current_blocks;
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

    void increment(size_t count = 1) {
        blocks_processed += count;
    }

    void stop() {
        running = false;
        if (status_thread.joinable()) {
            status_thread.join();
        }

        auto end_time = std::chrono::steady_clock::now();
        double total_seconds = std::chrono::duration<double>(end_time - start_time).count();

        std::cerr << "\n" << Colors::GREEN << "[COMPLETE] " << Colors::RESET
                  << "Total time: " << std::fixed << std::setprecision(2) << total_seconds << "s\n";
    }
};

size_t parse_size(const std::string& s) {
    size_t num = 0, i = 0;
    bool digits = false;

    for (; i < s.size() && std::isdigit(static_cast<unsigned char>(s[i])); ++i) {
        num = num * 10 + (s[i] - '0');
        digits = true;
    }

    if (!digits) throw std::invalid_argument("Invalid size format");

    char suffix = (i < s.size()) ? std::toupper(static_cast<unsigned char>(s[i])) : '\0';

    switch(suffix) {
        case 'K': return num * 1024;
        case 'M': return num * 1024 * 1024;
        case 'G': return num * 1024 * 1024 * 1024;
        case '\0': return num;
        default: throw std::invalid_argument("Invalid suffix");
    }
}

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

void fsync_if_needed(const std::string& path) {
    if (path != "stdout") {
        int fd = open(path.c_str(), O_WRONLY);
        if (fd >= 0) {
            fsync(fd);
            close(fd);
        }
    }
}

void transfer_data(const FileWrapper& files, const Config& cfg) {
    std::vector<char> buffer(cfg.block_size);
    StatusIndicator status(cfg.block_size);

    if (cfg.progress) status.start();

    size_t total_blocks = 0;

    while (cfg.count == 0 || total_blocks < cfg.count) {
        files.input().read(buffer.data(), static_cast<std::streamsize>(cfg.block_size));
        std::streamsize bytes_read = files.input().gcount();

        if (bytes_read > 0) {
            files.output().write(buffer.data(), bytes_read);
            files.output().flush();
            fsync_if_needed(cfg.output);
            total_blocks++;
            if (cfg.progress) status.increment();
        }

        if (files.input().eof()) break;
        if (!files.input().good()) throw std::runtime_error("Read error");
    }

    if (cfg.progress) status.stop();

    std::cerr << Colors::GREEN << total_blocks << " records in\n"
              << Colors::BLUE << total_blocks << " records out\n"
              << Colors::YELLOW << total_blocks * cfg.block_size << " bytes transferred\n"
              << Colors::RESET;
}

int main(int argc, char** argv) {
    std::cout << "Cryonix Data-Definition v1.0" << std::endl;

    try {
        Config cfg = parse_args(argc, argv);
        FileWrapper files(cfg);
        transfer_data(files, cfg);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}