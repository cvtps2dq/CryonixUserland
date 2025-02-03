//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <unistd.h>
#include <cstring>

int main() {
    // Get the file name of the terminal associated with the process's controlling terminal
    char* tty_name = ttyname(STDIN_FILENO);

    if (tty_name == nullptr) {
        std::cerr << "Not a terminal.\n";
        return 1;
    }

    std::cout << tty_name << "\n";
    return 0;
}