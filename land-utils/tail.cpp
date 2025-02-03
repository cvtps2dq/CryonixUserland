//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <fstream>
#include <deque>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <thread>

void print_last_n_lines(const std::string& file_path, int num_lines, bool follow) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << file_path << std::endl;
        exit(1);
    }

    std::deque<std::string> lines;
    std::string line;

    // Read the file and store the lines in a deque
    while (std::getline(file, line)) {
        lines.push_back(line);
        if (lines.size() > num_lines) {
            lines.pop_front();  // Keep only the last `num_lines` lines
        }
    }

    // If follow is enabled, continue monitoring the file for new lines
    if (follow) {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (std::getline(file, line)) {
                lines.push_back(line);
                if (lines.size() > num_lines) {
                    lines.pop_front();
                }
                system("clear");
            }
            // Print the latest lines
            for (const auto& line : lines) {
                std::cout << line << std::endl;
            }
            std::cout.flush();
        }
    } else {
        // Print the last `num_lines` lines
        for (const auto& line : lines) {
            std::cout << line << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    bool follow = false;
    int num_lines = 10;
    std::string file_path;

    if (argc < 2) {
        std::cerr << "Usage: cryonix_tail [-n num_lines] [-f] <file>" << std::endl;
        return 1;
    }

    // Parse command-line arguments
    for (int i = 1; i < argc - 1; ++i) {
        if (std::string(argv[i]) == "-n" && i + 1 < argc) {
            num_lines = std::stoi(argv[++i]);
        } else if (std::string(argv[i]) == "-f") {
            follow = true;
        }
    }

    file_path = argv[argc - 1];

    // Call the function to print last N lines or follow the file
    print_last_n_lines(file_path, num_lines, follow);

    return 0;
}