//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <string>

// Function to print the usage instructions
void print_usage() {
    std::cout << "Usage: dirname <path>\n"
              << "Return the directory part of the given path.\n";
}

// Function to get the directory part of the path
std::string get_directory(const std::string& path) {
    // Handle the case when the path is just the root directory
    if (path == "/") {
        return "/";
    }

    // Remove trailing slashes
    std::string trimmed_path = path;
    while (!trimmed_path.empty() && trimmed_path.back() == '/') {
        trimmed_path.pop_back();
    }

    // Find the last '/' in the string to separate the directory from the filename
    const size_t pos = trimmed_path.find_last_of('/');

    // If there's no '/' (i.e., the path is just a file name), return "."
    if (pos == std::string::npos) {
        return ".";
    }

    return trimmed_path.substr(0, pos);
}

int main(const int argc, char* argv[]) {
    if (argc != 2) {
        print_usage();
        return 1;
    }

    const std::string path = argv[1];
    std::cout << get_directory(path) << "\n";
    return 0;
}