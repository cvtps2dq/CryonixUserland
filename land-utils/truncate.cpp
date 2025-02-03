//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <stdexcept>

// Function to truncate or extend a file
void truncate_file(const std::string& filename, off_t size) {
    std::ofstream file(filename, std::ios::in | std::ios::out | std::ios::binary);

    if (!file) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    // Seek to the desired size
    file.seekp(size);

    // If the file is extended, fill the remaining space with null bytes
    if (file.tellp() < size) {
        file.write("\0", 1);
    }

    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: cryonix_truncate <filename> <size>\n";
        return 1;
    }

    std::string filename = argv[1];
    off_t size = std::stoll(argv[2]);

    try {
        truncate_file(filename, size);
        std::cout << "File '" << filename << "' truncated/extended to size " << size << " bytes.\n";
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}