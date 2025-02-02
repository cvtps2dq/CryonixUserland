//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

// Function to check if a file exists
bool file_exists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

// Function to check if a file is readable
bool is_readable(const std::string& path) {
    return (access(path.c_str(), R_OK) == 0);
}

// Function to check if a file is writable
bool is_writable(const std::string& path) {
    return (access(path.c_str(), W_OK) == 0);
}

// Function to check if a file is executable
bool is_executable(const std::string& path) {
    return (access(path.c_str(), X_OK) == 0);
}

// Function to compare if two strings are equal
bool compare_strings_equal(const std::string& str1, const std::string& str2) {
    return (str1 == str2);
}

// Function to compare if two strings are not equal
bool compare_strings_not_equal(const std::string& str1, const std::string& str2) {
    return (str1 != str2);
}

// Function to compare if a number is equal
bool compare_numbers_equal(int num1, int num2) {
    return (num1 == num2);
}

// Function to compare if a number is greater than another
bool compare_numbers_greater_than(int num1, int num2) {
    return (num1 > num2);
}

// Function to compare if a number is less than another
bool compare_numbers_less_than(int num1, int num2) {
    return (num1 < num2);
}

// Function to print powerful output with color
void print_result(bool result, const std::string& message) {
    if (result) {
        std::cout << "\033[1;32m[PASS] " << message << "\033[0m\n";  // Green for pass
    } else {
        std::cout << "\033[1;31m[FAIL] " << message << "\033[0m\n";  // Red for fail
    }
}

// Function to print powerful output for numeric comparisons
void print_numeric_result(bool result, const std::string& operation, int num1, int num2) {
    if (result) {
        std::cout << "\033[1;32m[PASS] " << num1 << " " << operation << " " << num2 << "\033[0m\n";  // Green for pass
    } else {
        std::cout << "\033[1;31m[FAIL] " << num1 << " " << operation << " " << num2 << "\033[0m\n";  // Red for fail
    }
}

// Main function to handle test command-like behavior
void process_test_command(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: cryonix_test [CONDITIONS] [FILE] [STRING] [NUMERIC COMPARISONS]\n";
        return;
    }

    // Handling file checks
    if (std::string(argv[1]) == "-e") {
        if (argc < 3) {
            std::cerr << "Error: Missing file argument.\n";
            return;
        }
        std::string path = argv[2];
        print_result(file_exists(path), "File " + path + " exists");
    }
    else if (std::string(argv[1]) == "-r") {
        if (argc < 3) {
            std::cerr << "Error: Missing file argument.\n";
            return;
        }
        std::string path = argv[2];
        print_result(is_readable(path), "File " + path + " is readable");
    }
    else if (std::string(argv[1]) == "-w") {
        if (argc < 3) {
            std::cerr << "Error: Missing file argument.\n";
            return;
        }
        std::string path = argv[2];
        print_result(is_writable(path), "File " + path + " is writable");
    }
    else if (std::string(argv[1]) == "-x") {
        if (argc < 3) {
            std::cerr << "Error: Missing file argument.\n";
            return;
        }
        std::string path = argv[2];
        print_result(is_executable(path), "File " + path + " is executable");
    }
    // String comparison checks
    else if (std::string(argv[1]) == "=") {
        if (argc < 4) {
            std::cerr << "Error: Missing string arguments.\n";
            return;
        }
        std::string str1 = argv[2];
        std::string str2 = argv[3];
        print_result(compare_strings_equal(str1, str2), "Strings are equal: \"" + str1 + "\" == \"" + str2 + "\"");
    }
    else if (std::string(argv[1]) == "!=") {
        if (argc < 4) {
            std::cerr << "Error: Missing string arguments.\n";
            return;
        }
        std::string str1 = argv[2];
        std::string str2 = argv[3];
        print_result(compare_strings_not_equal(str1, str2), "Strings are not equal: \"" + str1 + "\" != \"" + str2 + "\"");
    }
    // Numeric comparison checks
    else if (std::string(argv[1]) == "-eq") {
        if (argc < 4) {
            std::cerr << "Error: Missing numeric arguments.\n";
            return;
        }
        int num1 = std::stoi(argv[2]);
        int num2 = std::stoi(argv[3]);
        print_numeric_result(compare_numbers_equal(num1, num2), "==", num1, num2);
    }
    else if (std::string(argv[1]) == "-gt") {
        if (argc < 4) {
            std::cerr << "Error: Missing numeric arguments.\n";
            return;
        }
        int num1 = std::stoi(argv[2]);
        int num2 = std::stoi(argv[3]);
        print_numeric_result(compare_numbers_greater_than(num1, num2), ">", num1, num2);
    }
    else if (std::string(argv[1]) == "-lt") {
        if (argc < 4) {
            std::cerr << "Error: Missing numeric arguments.\n";
            return;
        }
        int num1 = std::stoi(argv[2]);
        int num2 = std::stoi(argv[3]);
        print_numeric_result(compare_numbers_less_than(num1, num2), "<", num1, num2);
    }
    // Additional flags
    else {
        std::cerr << "Error: Unsupported operation or flag.\n";
    }
}

int main(int argc, char* argv[]) {
    process_test_command(argc, argv);
    return 0;
}