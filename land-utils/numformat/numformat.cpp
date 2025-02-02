//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
#include <sstream>

void print_usage() {
    std::cout << "Usage: cryonix_numfmt [OPTIONS] [NUMBERS...]\n"
              << "Convert NUMBERS into human-readable formats.\n\n"
              << "  -k, --kilo       Use 1000 as the base (SI)\n"
              << "  -i, --iec        Use 1024 as the base (IEC)\n"
              << "  -p, --prefix     Prefix for the formatted number\n"
              << "  -s, --suffix     Suffix for the formatted number\n"
              << "  -d, --decimal    Number of decimal places to display\n";
}

std::string format_number(double number, bool is_si, const std::string& suffix, const std::string& prefix, int decimals) {
    const std::string units_si[] = {"", "k", "M", "G", "T", "P", "E"};
    const std::string units_iec[] = {"", "Ki", "Mi", "Gi", "Ti", "Pi", "Ei"};

    int unit_index = 0;
    const std::string* units = is_si ? units_si : units_iec;

    while (number >= (is_si ? 1000.0 : 1024.0) && unit_index < 6) {
        number /= (is_si ? 1000.0 : 1024.0);
        unit_index++;
    }

    std::ostringstream formatted;
    formatted << prefix << std::fixed << std::setprecision(decimals) << number << " " << units[unit_index] << suffix;

    return formatted.str();
}

void process_numbers(int argc, char* argv[], bool is_si, const std::string& suffix, const std::string& prefix, int decimals) {
    if (argc == 1) {
        print_usage();
        return;
    }

    for (int i = 1; i < argc; ++i) {
        try {
            double number = std::stod(argv[i]);
            std::cout << format_number(number, is_si, suffix, prefix, decimals) << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cerr << "\033[1;31mError: Invalid number: " << argv[i] << "\033[0m\n";
        }
    }
}

int main(int argc, char* argv[]) {
    bool is_si = true;
    std::string suffix = "";
    std::string prefix = "";
    int decimals = 2;

    // Parse flags and arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-k" || arg == "--kilo") {
            is_si = true;
        } else if (arg == "-i" || arg == "--iec") {
            is_si = false;
        } else if (arg == "-s" || arg == "--suffix") {
            if (i + 1 < argc) {
                suffix = argv[++i];
            } else {
                std::cerr << "\033[1;31mError: Missing argument for -s option\033[0m\n";
                return 1;
            }
        } else if (arg == "-p" || arg == "--prefix") {
            if (i + 1 < argc) {
                prefix = argv[++i];
            } else {
                std::cerr << "\033[1;31mError: Missing argument for -p option\033[0m\n";
                return 1;
            }
        } else if (arg == "-d" || arg == "--decimal") {
            if (i + 1 < argc) {
                try {
                    decimals = std::stoi(argv[++i]);
                } catch (const std::exception& e) {
                    std::cerr << "\033[1;31mError: Invalid decimal places\033[0m\n";
                    return 1;
                }
            } else {
                std::cerr << "\033[1;31mError: Missing argument for -d option\033[0m\n";
                return 1;
            }
        }
    }

    process_numbers(argc, argv, is_si, suffix, prefix, decimals);
    return 0;
}