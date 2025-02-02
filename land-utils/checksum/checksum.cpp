//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <zlib.h>  // For CRC32 checksum

void print_usage() {
    std::cout << "Usage: checksum FILE\n"
              << "Calculate the CRC32 checksum and size of the given file.\n";
}

unsigned long calculate_crc32(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'.\n";
        exit(1);
    }

    unsigned long crc = 0;
    char buffer[4096];
    while (file.read(buffer, sizeof(buffer))) {
        crc = crc32(crc, reinterpret_cast<const unsigned char*>(buffer), file.gcount());
    }

    // Read the remaining part of the file (if any)
    if (file.gcount() > 0) {
        crc = crc32(crc, reinterpret_cast<const unsigned char*>(buffer), file.gcount());
    }

    return crc;
}

void cksum_clone(const std::string& filename) {
    // Calculate CRC32 checksum
    const unsigned long crc = calculate_crc32(filename);

    // Get file size
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'.\n";
        exit(1);
    }

    const std::streamsize size = file.tellg();

    // Output the checksum (in decimal), file size, and filename
    std::cout << crc << " " << size << " " << filename << std::endl;
}

int main(const int argc, char* argv[]) {
    if (argc != 2) {
        print_usage();
        return 1;
    }

    const std::string filename = argv[1];

    cksum_clone(filename);

    return 0;
}