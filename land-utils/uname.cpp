//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <sys/utsname.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iomanip>
#include <sstream>

// Function to fetch system information
std::string get_kernel_version() {
    struct utsname buffer;
    if (uname(&buffer) != 0) {
        return "Unable to fetch kernel version";
    }
    return std::string(buffer.release);
}

std::string get_system_name() {
    struct utsname buffer;
    if (uname(&buffer) != 0) {
        return "Unable to fetch system name";
    }
    return std::string(buffer.sysname);
}

std::string get_machine_arch() {
    struct utsname buffer;
    if (uname(&buffer) != 0) {
        return "Unable to fetch machine architecture";
    }
    return std::string(buffer.machine);
}

std::string get_hostname() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        return std::string(hostname);
    }
    return "Unknown Hostname";
}

// Function to mimic neofetch-style output with colors and format
void fetch_system_info() {
    // ANSI Escape Codes for Colors
    const std::string BLUE = "\033[34m";
    const std::string GREEN = "\033[32m";
    const std::string CYAN = "\033[36m";
    const std::string RESET = "\033[0m";

    // Fetching system info
    std::string kernel = get_kernel_version();
    std::string system_name = get_system_name();
    std::string architecture = get_machine_arch();
    std::string hostname = get_hostname();

    // Displaying the "neofetch-like" output
    std::cout << BLUE << "     .-" << RESET << std::endl;
    std::cout << BLUE << "    /  " << GREEN << "o " << RESET << "  " << BLUE << ".__" << RESET << std::endl;
    std::cout << BLUE << "   |    " << CYAN << hostname << RESET << "   |" << std::endl;
    std::cout << BLUE << "   |" << RESET << " Kernel: " << CYAN << kernel << RESET << std::endl;
    std::cout << BLUE << "   |" << RESET << " Architecture: " << CYAN << architecture << RESET << std::endl;
    std::cout << BLUE << "    \\_____" << RESET << std::endl;
    std::cout << BLUE << "       [Info]" << RESET << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc == 2 && std::string(argv[1]) == "--fetch") {
        // Call the fetch function to display the system info
        fetch_system_info();
        return 0;
    }

    if (argc == 1) {
        // Default output (basic uname functionality)
        struct utsname buffer;
        if (uname(&buffer) != 0) {
            std::cerr << "Error: Unable to fetch system information." << std::endl;
            return 1;
        }

        std::cout << "System Name: " << buffer.sysname << std::endl;
        std::cout << "Node Name: " << buffer.nodename << std::endl;
        std::cout << "Release: " << buffer.release << std::endl;
        std::cout << "Version: " << buffer.version << std::endl;
        std::cout << "Machine: " << buffer.machine << std::endl;
    } else {
        std::cerr << "Usage: cryonix_uname [--fetch]" << std::endl;
        return 1;
    }

    return 0;
}