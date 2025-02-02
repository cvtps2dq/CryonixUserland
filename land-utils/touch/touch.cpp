//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <string>

// Function to print usage instructions
void print_usage() {
    std::cerr << "Usage: cryonix_touch [OPTION]... FILE...\n";
    std::cerr << "Update the access and modification times of each FILE to the current time.\n";
    std::cerr << "If FILE does not exist, it is created empty.\n";
    std::cerr << "\nOptions:\n";
    std::cerr << "  --grass    : Have more fun with the easter egg!\n";
}

// Easter egg functionality
void easter_egg() {
    std::cout << " You've activated the dark side of the 'touch' command... \n";
    std::cout << "Beware, for the grass may be greener on the other side... but at what cost? \n";
    std::cout << "You touched it, and now it's cursed... like your past mistakes. \n";
    std::cout << "Embrace the void... but remember, there's no turning back. \n";
    std::cout << "Now, go ahead and touch life, because life certainly touches you first... in the dark. <3\n";
}

// Function to create a file or update timestamps
void create_or_update_file(const std::string& filename) {
    std::ofstream file(filename, std::ios::app); // Open file in append mode
    if (file.is_open()) {
        std::cout << "File '" << filename << "' created or timestamps updated.\n";
    } else {
        std::cerr << "Error: Unable to create or update file '" << filename << "'.\n";
    }
}

// Main function
int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    // Parse flags and files
    bool woman_flag = false;
    bool grass_flag = false;
    std::vector<std::string> files;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

         if (arg == "--grass") {
            grass_flag = true;
        } else {
            files.push_back(arg);
        }
    }

    // If the easter egg flags are both present
    if (grass_flag) {
        easter_egg();
    }

    // Create or update files
    for (const auto& file : files) {
        create_or_update_file(file);
    }

    return 0;
}