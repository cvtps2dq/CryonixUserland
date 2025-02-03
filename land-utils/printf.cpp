//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <string>
#include <cstdarg>
#include <cstdio>
#include <iomanip>
#include <cstdlib>
#include <vector>

void print_usage() {
    std::cout << "Usage: cryonix_printf FORMAT [ARGUMENTS...]\n"
              << "Print formatted output.\n\n"
              << "This is a clone of the standard printf command.\n"
              << "  -h, --help               Display this help message\n";
}

void cryonix_printf(const std::string& format, ...) {
    va_list args;
    va_start(args, format);

    // Print formatted output using std::vprintf, which takes va_list
    vprintf(format.c_str(), args);

    va_end(args);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    bool show_help = false;

    // Parse flags and arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            show_help = true;
            break;
        }
    }

    if (show_help) {
        print_usage();
        return 0;
    }

    // The format string is the first argument
    std::string format = argv[1];
    std::vector<std::string> args(argv + 2, argv + argc);

    // Call our cryonix_printf function with the format and arguments
    cryonix_printf(format, args.data());

    return 0;
}