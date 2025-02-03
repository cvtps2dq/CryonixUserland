//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdlib>

void print_usage() {
    std::cout << "Usage: cryonix_ptx [OPTIONS] [FILE...]\n"
              << "Generate paginated output with formatted headers.\n\n"
              << "  -h, --help               Display this help message\n"
              << "  -n, --lines NUM          Set number of lines per page (default: 20)\n";
}

void print_header(int page_number) {
    std::cout << "\033[1;32mPage " << page_number << "\033[0m\n";
    std::cout << "==============================\n";
}

void print_page(const std::vector<std::string>& lines, int page_number, int lines_per_page) {
    int start_line = (page_number - 1) * lines_per_page;
    int end_line = std::min(start_line + lines_per_page, static_cast<int>(lines.size()));

    print_header(page_number);

    for (int i = start_line; i < end_line; ++i) {
        std::cout << lines[i] << "\n";
    }

    std::cout << "\n[Press Enter to continue]";
}

void paginate_input(std::istream& input_stream, int lines_per_page) {
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(input_stream, line)) {
        lines.push_back(line);
    }

    int total_pages = (lines.size() + lines_per_page - 1) / lines_per_page;
    int current_page = 1;

    while (current_page <= total_pages) {
        print_page(lines, current_page, lines_per_page);

        std::cin.get(); // Wait for user input (press Enter)
        current_page++;
    }
}

int main(int argc, char* argv[]) {
    int lines_per_page = 20;
    bool show_help = false;
    std::string filename;

    // Parse flags and arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            show_help = true;
            break;
        } else if (arg == "-n" || arg == "--lines") {
            if (i + 1 < argc) {
                lines_per_page = std::stoi(argv[++i]);
            } else {
                std::cerr << "Error: --lines requires an argument\n";
                return 1;
            }
        } else {
            filename = arg;
        }
    }

    if (show_help) {
        print_usage();
        return 0;
    }

    if (!filename.empty()) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << "\n";
            return 1;
        }
        paginate_input(file, lines_per_page);
    } else {
        paginate_input(std::cin, lines_per_page);
    }

    return 0;
}