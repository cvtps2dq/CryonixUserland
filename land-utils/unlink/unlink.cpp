//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <errno.h>
#include <cstring>

// Function to mimic the unlink command
void unlink_file(const std::string& filename) {
    // Attempt to unlink (remove) the file
    if (unlink(filename.c_str()) == -1) {
        std::cerr << "Error: Unable to unlink file '" << filename << "': " << strerror(errno) << std::endl;
    } else {
        std::cout << "Successfully unlinked '" << filename << "'" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: cryonix_unlink <filename>" << std::endl;
        return 1;
    }

    // The file to unlink is provided as an argument
    std::string filename = argv[1];

    // Call the unlink_file function to remove the file
    unlink_file(filename);

    return 0;
}