//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

void print_usage() {
    std::cout << "Usage: numlines [OPTIONS] FILE\n"
              << "Number the lines of FILE and output them.\n\n"
              << "  -b NUMBERING     Select line numbering type (t, a, n)\n"
              << "  -n NUM           Number starting point\n"
              << "  -s STRING        String to separate line number from content\n";
}

void print_line_numbering(const std::string& line, size_t line_number, const std::string& separator) {
    std::cout << "\033[1;34m" << std::setw(4) << line_number << "\033[0m" << separator << " " << line;
}

void number_lines(const std::string& filename, char numbering_type, size_t start_number, const std::string& separator) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "\033[1;31mError: Unable to open file " << filename << "\033[0m\n";
        return;
    }

    std::string line;
    size_t line_number = start_number;

    while (std::getline(file, line)) {
        switch (numbering_type) {
            case 'a':
                print_line_numbering(line, line_number, separator);
                break;
            case 'n':
                if (!line.empty()) {
                    print_line_numbering(line, line_number, separator);
                    ++line_number;
                } else {
                    std::cout << line; // No numbering for empty lines
                }
                break;
            case 't':
                if (!line.empty()) {
                    print_line_numbering(line, line_number, separator);
                    ++line_number;
                } else {
                    std::cout << line;
                }
                break;
            default:
                print_line_numbering(line, line_number, separator);
                break;
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    char numbering_type = 'a';
    size_t start_number = 1;
    std::string separator = "\t";
    std::string filename;

    // Parse flags and arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-b") {
            if (i + 1 < argc) {
                numbering_type = argv[++i][0];
            } else {
                std::cerr << "\033[1;31mError: Missing argument for -b option\033[0m\n";
                return 1;
            }
        } else if (arg == "-n") {
            if (i + 1 < argc) {
                try {
                    start_number = std::stoi(argv[++i]);
                } catch (const std::exception& e) {
                    std::cerr << "\033[1;31mError: Invalid start number\033[0m\n";
                    return 1;
                }
            } else {
                std::cerr << "\033[1;31mError: Missing argument for -n option\033[0m\n";
                return 1;
            }
        } else if (arg == "-s") {
            if (i + 1 < argc) {
                separator = argv[++i];
            } else {
                std::cerr << "\033[1;31mError: Missing argument for -s option\033[0m\n";
                return 1;
            }
        } else {
            filename = arg;
        }
    }

    if (filename.empty()) {
        std::cerr << "\033[1;31mError: No file specified\033[0m\n";
        return 1;
    }

    number_lines(filename, numbering_type, start_number, separator);
    return 0;
}