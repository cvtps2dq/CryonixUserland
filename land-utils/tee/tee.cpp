//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

void write_to_files(const std::vector<std::string>& files, const std::string& input, bool append) {
    for (const auto& file : files) {
        std::ofstream ofs;
        if (append) {
            ofs.open(file, std::ios::app);
        } else {
            ofs.open(file, std::ios::trunc);
        }

        if (!ofs) {
            std::cerr << "Error: Unable to write to file: " << file << std::endl;
            exit(1);
        }

        ofs << input;
    }
}

void tee_command(bool append, const std::vector<std::string>& files) {
    std::string input;
    while (std::getline(std::cin, input)) {
        // Write to standard output (console)
        std::cout << input << std::endl;

        // Write to the specified files
        write_to_files(files, input, append);
    }
}

int main(int argc, char* argv[]) {
    bool append = false;
    std::vector<std::string> files;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-a") {
            append = true;
        } else {
            files.emplace_back(argv[i]);
        }
    }

    // Check if at least one file is specified
    if (files.empty()) {
        std::cerr << "Usage: cryonix_tee [-a] <file1> <file2> ... " << std::endl;
        return 1;
    }

    // Call the tee function
    tee_command(append, files);

    return 0;
}