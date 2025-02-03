//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <cmath>
#include <vector>
#include <stdexcept>

// Function to print the usage instructions
void print_usage() {
    std::cout << "Usage: factor <number>...\n"
              << "Find prime factors of the given number(s).\n";
}

// Function to calculate the prime factors of a number
std::vector<int> prime_factors(int number) {
    std::vector<int> factors;

    // Handle 2 as a special case
    while (number % 2 == 0) {
        factors.push_back(2);
        number /= 2;
    }

    // Now check odd numbers from 3 upwards
    for (int i = 3; i <= std::sqrt(number); i += 2) {
        while (number % i == 0) {
            factors.push_back(i);
            number /= i;
        }
    }

    // If number is a prime greater than 2
    if (number > 2) {
        factors.push_back(number);
    }

    return factors;
}

// Function to process each number and print its prime factors
void process_number(const int number) {
    if (number <= 1) {
        std::cerr << "Error: Number must be greater than 1\n";
        return;
    }

    const std::vector<int> factors = prime_factors(number);

    std::cout << number << ": ";
    for (size_t i = 0; i < factors.size(); ++i) {
        std::cout << factors[i];
        if (i != factors.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << "\n";
}

int main(const int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        try {
            const int number = std::stoi(argv[i]);
            process_number(number);
        } catch ([[maybe_unused]] const std::invalid_argument& e) {
            std::cerr << "Error: Invalid number: " << argv[i] << "\n";
        } catch ([[maybe_unused]] const std::out_of_range& e) {
            std::cerr << "Error: Number out of range: " << argv[i] << "\n";
        }
    }

    return 0;
}