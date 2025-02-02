//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <cmath>

// Function to print the sequence
void print_sequence(int start, int end, int step, int width) {
    if (start <= end) {
        // Print sequence in ascending order
        for (int i = start; i <= end; i += step) {
            std::cout << std::setw(width) << i << std::endl;
        }
    } else {
        // Print sequence in descending order
        for (int i = start; i >= end; i -= step) {
            std::cout << std::setw(width) << i << std::endl;
        }
    }
}

// Function to handle command-line arguments
void parse_arguments(int argc, char* argv[], int& start, int& end, int& step, int& width, bool& reverse) {
    if (argc < 2) {
        std::cerr << "Error: Missing required arguments." << std::endl;
        exit(1);
    }

    start = std::stoi(argv[1]);
    end = std::stoi(argv[2]);
    step = 1;  // default step is 1
    width = 0; // default width (no formatting)

    if (argc > 3) {
        // Parse step if provided
        if (argc >= 4) {
            step = std::stoi(argv[3]);
            if (step == 0) {
                std::cerr << "Error: Step value cannot be zero." << std::endl;
                exit(1);
            }
        }
    }

    if (argc > 4) {
        // Parse width if provided
        if (argc >= 5) {
            width = std::stoi(argv[4]);
        }
    }

    // Check if reverse order flag is needed
    reverse = false;
    if (start > end) {
        reverse = true;
    }
}

// Function to print the help message
void print_usage() {
    std::cout << "Usage: sequence [start] [end] [step] [width]\n"
              << "Print a sequence of numbers from start to end with an optional step size.\n\n"
              << "  [start]  The starting number\n"
              << "  [end]    The ending number\n"
              << "  [step]   The step size (default: 1)\n"
              << "  [width]  The width of the printed numbers (optional)\n";
}

int main(int argc, char* argv[]) {
    int start, end, step, width;
    bool reverse;

    // Parse the command-line arguments
    parse_arguments(argc, argv, start, end, step, width, reverse);

    // Print the sequence
    print_sequence(start, end, step, width);

    return 0;
}