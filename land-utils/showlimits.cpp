//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>
#include <iomanip>
#include <string>

// Function to print usage instructions
void print_usage() {
    std::cout << "Usage: showlimits [OPTIONS]\n"
              << "Display user resource limits in a beautiful format.\n"
              << "Options:\n"
              << "  -a    Display all resource limits.\n"
              << "  -c    Display core dump size limit.\n"
              << "  -n    Display maximum number of open file descriptors.\n";
}

// Function to print a formatted limit
void print_limit(const std::string& label, const long long value, const std::string& unit = "") {
    std::cout << "  " << std::left << std::setw(40) << label
              << std::right << std::setw(10) << value << " " << unit << "\n";
}

// Function to display resource limits
void display_limits(const bool show_all, const bool show_core, const bool show_open_files) {
    rlimit rl{};

    std::cout << "\033[1;34m=============================== Resource Limits ===============================\033[0m\n";

    // Maximum number of open file descriptors
    if (show_all || show_open_files) {
        if (getrlimit(RLIMIT_NOFILE, &rl) == 0) {
            print_limit("Maximum number of open file descriptors", static_cast<long long>(rl.rlim_cur));
        }
    }

    // Maximum stack size
    if (show_all) {
        if (getrlimit(RLIMIT_STACK, &rl) == 0) {
            print_limit("Maximum stack size (bytes)", static_cast<long long>(rl.rlim_cur));
        }
    }

    // Maximum process size
    if (show_all) {
        if (getrlimit(RLIMIT_DATA, &rl) == 0) {
            print_limit("Maximum process size (bytes)", static_cast<long long>(rl.rlim_cur));
        }
    }

    // Maximum user processes
    if (show_all) {
        if (getrlimit(RLIMIT_NPROC, &rl) == 0) {
            print_limit("Maximum user processes", static_cast<long long>(rl.rlim_cur));
        }
    }

    // Maximum number of files
    if (show_all) {
        if (getrlimit(RLIMIT_FSIZE, &rl) == 0) {
            print_limit("Maximum number of files", static_cast<long long>(rl.rlim_cur));
        }
    }

    // Core dump size
    if (show_all || show_core) {
        if (getrlimit(RLIMIT_CORE, &rl) == 0) {
            print_limit("Core dump size (bytes)", static_cast<long long>(rl.rlim_cur));
        }
    }

    std::cout << "\033[1;34m===============================================================================\033[0m\n";
}

int main(const int argc, char* argv[]) {
    bool show_all = false;
    bool show_core = false;
    bool show_open_files = false;

    // Parsing command-line options
    for (int i = 1; i < argc; ++i) {
        if (std::string arg = argv[i]; arg == "-a") {
            show_all = true;
        } else if (arg == "-c") {
            show_core = true;
        } else if (arg == "-n") {
            show_open_files = true;
        } else {
            print_usage();
            return 1;
        }
    }

    // If no options are specified, show all by default
    if (!show_all && !show_core && !show_open_files) {
        show_all = true;
    }

    // Display the resource limits
    display_limits(show_all, show_core, show_open_files);

    return 0;
}