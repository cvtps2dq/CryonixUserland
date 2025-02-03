//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cstdint>

#define BLOCK_SIZE 512

// Function to calculate checksum (BSD style) and block count for a file
void calculate_sum(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << file_path << std::endl;
        exit(1);
    }

    uint32_t checksum = 0;
    uint32_t block_count = 0;
    char buffer[BLOCK_SIZE];
    size_t bytes_read;

    // Read file in blocks and calculate checksum
    while ((bytes_read = file.readsome(buffer, BLOCK_SIZE)) > 0) {
        block_count++;

        // Process 16-bit chunks of the block (checksum calculation)
        for (size_t i = 0; i < bytes_read; i += 2) {
            uint16_t value = 0;
            value |= (static_cast<unsigned char>(buffer[i]) << 8);
            if (i + 1 < bytes_read) {
                value |= static_cast<unsigned char>(buffer[i + 1]);
            }
            checksum += value;
            if (checksum > 0xFFFF) {
                checksum -= 0xFFFF;  // Apply overflow
            }
        }
    }

    // Output checksum and block count
    std::cout << checksum << " " << block_count << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: blocksum <file>" << std::endl;
        return 1;
    }

    std::string file_path = argv[1];
    calculate_sum(file_path);

    return 0;
}