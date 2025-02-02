//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <cstdlib>

// Function to flush the file system buffers
void sync_filesystem() {
    if (sync() == -1) {
        std::cerr << "Error: Unable to sync file system buffers: " << strerror(errno) << std::endl;
        exit(1);
    }
    std::cout << "File system buffers synchronized successfully!" << std::endl;
}

int main() {
    sync_filesystem();
    return 0;
}