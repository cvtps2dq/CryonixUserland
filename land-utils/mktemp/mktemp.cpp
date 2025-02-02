//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <filesystem>
#include <string>
#include <random>
#include <ctime>
#include <fstream>
#include <sstream>

void print_usage() {
    std::cout << "Usage: cryonix_mktemp [OPTIONS] TEMPLATE\n"
              << "Create a unique temporary file or directory from TEMPLATE.\n\n"
              << "  -d    Create a temporary directory instead of a file\n";
}

std::string generate_temp_name(const std::string& template_str) {
    // Generate random string for temporary file/directory
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);  // Hexadecimal characters

    std::string temp_name = template_str;
    for (size_t i = 0; i < temp_name.size(); ++i) {
        if (temp_name[i] == 'X') {
            temp_name[i] = "0123456789abcdef"[dis(gen)];
        }
    }

    return temp_name;
}

void create_temp(const std::string& template_str, bool is_directory) {
    std::string temp_name = generate_temp_name(template_str);

    try {
        if (is_directory) {
            if (std::filesystem::create_directory(temp_name)) {
                std::cout << "\033[1;32mSuccessfully created temporary directory: " << temp_name << "\033[0m\n";
            } else {
                std::cerr << "\033[1;31mError: Unable to create directory " << temp_name << "\033[0m\n";
            }
        } else {
            std::ofstream temp_file(temp_name);
            if (temp_file) {
                std::cout << "\033[1;32mSuccessfully created temporary file: " << temp_name << "\033[0m\n";
            } else {
                std::cerr << "\033[1;31mError: Unable to create file " << temp_name << "\033[0m\n";
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "\033[1;31mError: " << e.what() << "\033[0m\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    bool is_directory = false;
    std::string template_str;

    // Parse flags and arguments
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-d") {
            is_directory = true;
        } else {
            template_str = argv[i];
        }
    }

    if (template_str.empty()) {
        print_usage();
        return 1;
    }

    // Ensure TEMPLATE ends with "XXXXXX"
    if (template_str.find("XXXXXX") == std::string::npos) {
        std::cerr << "\033[1;31mError: TEMPLATE must contain 'XXXXXX' to be replaced with random characters.\033[0m\n";
        return 1;
    }

    create_temp(template_str, is_directory);
    return 0;
}