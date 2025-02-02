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
            std::cout << Colors::BLUE
            << "opening input file: " << cfg.input << Colors::RESET << std::endl;
            if (!infile) throw std::runtime_error("input file error");
            in = &infile;
        }

        if (cfg.output != "stdout") {
            outfile.open(cfg.output, std::ios::binary | std::ios::trunc);
            std::cout << Colors::BLUE
            << "opening output file: " << cfg.output << Colors::RESET << std::endl;
            if (!outfile) throw std::runtime_error("output file error");
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

    void display_loop() const {
        using namespace std::chrono;
        constexpr auto update_interval = 500ms;

        auto last_block_count = blocks_processed.load();
        auto last_time = steady_clock::now();

        while (running) {
            std::this_thread::sleep_for(update_interval);

            auto now = steady_clock::now();
            const auto current_blocks = blocks_processed.load();

            const double elapsed = duration<double>(now - last_time).count();
            const double total_elapsed = duration<double>(now - start_time).count();

            const size_t blocks_diff = current_blocks - last_block_count;
            const double speed = static_cast<double>(blocks_diff * block_size) / (elapsed * 1024 * 1024); // MB/s

            std::cerr << "\r" << Colors::CYAN << "[STATUS] " << Colors::RESET
                      << Colors::GREEN << "blocks: " << current_blocks << " " << Colors::RESET
                      << Colors::YELLOW << "speed: " << std::fixed << std::setprecision(2)
                      << speed << " MB/s" << Colors::RESET
                      << Colors::MAGENTA << "total elapsed: " << total_elapsed << Colors::RESET
                      << std::string(10, ' '); // Clear any leftover characters

            last_block_count = current_blocks;
            last_time = now;
        }
    }

public:
    size_t block_size;

    explicit StatusIndicator(const size_t bs) : block_size(bs) {}

    void start() {
        start_time = std::chrono::steady_clock::now();
        running = true;
        status_thread = std::thread(&StatusIndicator::display_loop, this);
    }

    void increment(const size_t count = 1) {
        blocks_processed += count;
    }

    void stop() {
        running = false;
        if (status_thread.joinable()) {
            status_thread.join();
        }

        const auto end_time = std::chrono::steady_clock::now();
        const double total_seconds = std::chrono::duration<double>(end_time - start_time).count();

        std::cerr << "\n" << Colors::GREEN << "[COMPLETE] " << Colors::RESET
                  << "Total time: " << std::fixed << std::setprecision(2) << total_seconds << "s\n";
    }
};

size_t parse_size(const std::string& s) {
    size_t num = 0;
    size_t i = 0;
    bool digits = false;

    // parse numeric part
    for (; i < s.size() && std::isdigit(static_cast<unsigned char>(s[i])); ++i) {
        num = num * 10 + (s[i] - '0');
        digits = true;
    }

    if (!digits) throw std::invalid_argument("no digits found");

    // parse suffix with proper character handling
    char suffix = '\0';
    if (i < s.size()) {
        suffix = static_cast<char>(std::toupper(static_cast<unsigned char>(s[i++])));
        if (i < s.size() && suffix == 'B') { // Handle 'B' suffix (MB, GB)
            suffix = static_cast<char>(std::toupper(static_cast<unsigned char>(s[i])));
            i++;
        }
    }

    if (i != s.size()) throw std::invalid_argument("invalid characters");

    switch(suffix) {
        case 'K': return num * 1024;
        case 'M': return num * 1024 * 1024;
        case 'G': return num * 1024 * 1024 * 1024;
        case '\0': return num;
        default: throw std::invalid_argument("Invalid suffix");
    }
}

Config parse_args(const int argc, char** argv) {
    Config cfg;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        try {
            if (arg.find("from=") == 0) {
                cfg.input = arg.substr(5);
            } else if (arg.find("to=") == 0) {
                cfg.output = arg.substr(3);
            } else if (arg.find("block_size=") == 0) {
                cfg.block_size = parse_size(arg.substr(3));
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

void transfer_data(const FileWrapper& files, const Config& cfg) {
    std::vector<char> buffer(cfg.block_size);

    StatusIndicator status(cfg.block_size);
    if (cfg.progress) {
        status.start();
    }

    try {
        size_t total_blocks = 0;
        while (true) {
            if (cfg.count != 0 && total_blocks >= cfg.count) break;

            files.input().read(buffer.data(), static_cast<long>(cfg.block_size));

            if (const auto bytes_read = files.input().gcount(); bytes_read > 0) {
                files.output().write(buffer.data(), bytes_read);
                if (!files.output().good()) {
                    throw std::runtime_error("Write error");
                }
                total_blocks++;
                if (cfg.progress) {
                    status.increment();
                }
            }

            if (files.input().eof()) break;
            if (!files.input().good()) {
                throw std::runtime_error("Read error");
            }
        }

        if (cfg.progress) {
            status.stop();
        }

        std::cerr << Colors::GREEN << total_blocks << "+0 records in\n" << Colors::RESET
                  << Colors::BLUE << total_blocks << "+0 records out\n" << Colors::RESET
                  << Colors::YELLOW << total_blocks * cfg.block_size
                  << " bytes transferred\n" << Colors::RESET;
    } catch (...) {
        if (cfg.progress) {
            status.stop();
        }
        throw;
    }
}

int main(int argc, char** argv) {
    std::cout << "cryonix data-definition v1.0" << std::endl;
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