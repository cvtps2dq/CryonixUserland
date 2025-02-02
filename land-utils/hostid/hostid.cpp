//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <iomanip>
#include <unistd.h>

void print_usage() {
    std::cout << "Usage: hostid\n"
              << "Display the unique host identifier (hostid).\n";
}

int main(const int argc) {
    // If there are any arguments, show usage message
    if (argc > 1) {
        print_usage();
        return 1;
    }

    // Get the host identifier
    const unsigned long host_id = gethostid();

    // Print the host ID in hexadecimal format
    std::cout << std::hex << "Host ID: 0x" << std::setw(8) << std::setfill('0') << host_id << std::dec << std::endl;

    return 0;
}