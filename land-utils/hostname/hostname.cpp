//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <unistd.h>

constexpr size_t HOSTNAME_MAX_LEN = 256;

void print_usage() {
    std::cout << "Usage: hostname\n"
              << "Display the system's hostname.\n";
}

int main(const int argc, char* argv[]) {
    // If the user provides any arguments, display usage
    if (argc > 1) {
        print_usage();
        return 1;
    }

    // Buffer to store the hostname
    char hostname[HOSTNAME_MAX_LEN];

    // Get hostname and check for errors
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        std::cerr << "Error: Unable to retrieve hostname.\n";
        return 1;
    }

    // Print the hostname
    std::cout << hostname << std::endl;

    return 0;
}