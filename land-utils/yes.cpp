//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

void show_help() {
    std::cout << "Usage: cryonix_yes [string]\n"
              << "If no string is provided, it will repeat 'y'.\n";
}

int main(int argc, char* argv[]) {
    // If --help flag is passed, display help message
    if (argc == 2 && std::string(argv[1]) == "--help") {
        show_help();
        return 0;
    }

    // Default string is "y" if no argument is provided
    std::string repeat_string = (argc > 1) ? argv[1] : "y";

    // Infinite loop to mimic the behavior of the yes command
    while (true) {
        std::cout << repeat_string << std::endl;
        // Add a small delay (optional) to prevent overwhelming the terminal too quickly
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}