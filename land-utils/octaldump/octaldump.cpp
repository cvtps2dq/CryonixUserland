//
// Created by cv2 on 02.02.2025.
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <bitset>

void print_usage() {
    std::cout << "Usage: cryonix_od [OPTIONS] FILE\n"
              << "Display file contents in different formats (octal, hexadecimal, ASCII).\n\n"
              << "  -A, --address     Display addresses\n"
              << "  -t, --type        Set display type (o for octal, x for hexadecimal, a for ASCII)\n"
              << "  -c, --cols        Set number of columns per output line (default: 8)\n"
              << "  -h, --help        Display this help message\n";
}

void print_file_in_oct_format(std::ifstream& file, int cols) {
    char byte;
    int column_count = 0;
    while (file.get(byte)) {
        std::cout << std::setw(3) << std::oct << (static_cast<int>(byte) & 0xFF) << " ";
        if (++column_count % cols == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

void print_file_in_hex_format(std::ifstream& file, int cols) {
    char byte;
    int column_count = 0;
    while (file.get(byte)) {
        std::cout << std::setw(2) << std::hex << std::setfill('0') << (static_cast<int>(byte) & 0xFF) << " ";
        if (++column_count % cols == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

void print_file_in_ascii_format(std::ifstream& file, int cols) {
    char byte;
    int column_count = 0;
    while (file.get(byte)) {
        if (std::isprint(byte)) {
            std::cout << byte << " ";
        } else {
            std::cout << ". ";
        }
        if (++column_count % cols == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

void display_file_content(const std::string& file_name, const std::string& type, int cols) {
    std::ifstream file(file_name, std::ios::binary);

    if (!file) {
        std::cerr << "\033[1;31mError: Could not open file\033[0m\n";
        return;
    }

    if (type == "o") {
        print_file_in_oct_format(file, cols);
    } else if (type == "x") {
        print_file_in_hex_format(file, cols);
    } else if (type == "a") {
        print_file_in_ascii_format(file, cols);
    } else {
        std::cerr << "\033[1;31mError: Invalid type specified\033[0m\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }

    std::string type = "o"; // Default type (octal)
    int cols = 8; // Default number of columns

    // Parse flags and arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-t" || arg == "--type") {
            if (i + 1 < argc) {
                type = argv[++i];
            } else {
                std::cerr << "\033[1;31mError: Missing argument for -t option\033[0m\n";
                return 1;
            }
        } else if (arg == "-c" || arg == "--cols") {
            if (i + 1 < argc) {
                try {
                    cols = std::stoi(argv[++i]);
                } catch (const std::exception& e) {
                    std::cerr << "\033[1;31mError: Invalid number of columns\033[0m\n";
                    return 1;
                }
            } else {
                std::cerr << "\033[1;31mError: Missing argument for -c option\033[0m\n";
                return 1;
            }
        } else if (arg == "-A" || arg == "--address") {
            // Option for address is supported for future enhancements
            // No specific implementation in the current version
        } else if (arg == "-h" || arg == "--help") {
            print_usage();
            return 0;
        }
    }

    std::string file_name = argv[argc - 1];
    display_file_content(file_name, type, cols);

    return 0;
}