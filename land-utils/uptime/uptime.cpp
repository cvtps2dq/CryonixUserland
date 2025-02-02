//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <unistd.h>
#include <sys/sysinfo.h>

// Function to calculate system uptime
long get_system_uptime() {
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        std::cerr << "Error getting system uptime" << std::endl;
        return -1;
    }
    return info.uptime;
}

// Function to get the load averages
void get_load_averages(double &load1, double &load5, double &load15) {
    if (getloadavg(&load1, 3) == -1) {
        std::cerr << "Error getting load averages" << std::endl;
        load1 = load5 = load15 = 0.0;
    }
    load5 = load1;  // Placeholder for demonstration
    load15 = load1;  // Placeholder for demonstration
}

// Function to print output in a beautiful and colorful way
void print_beautiful_output(long uptime_seconds, double load1, double load5, double load15) {
    // Use ANSI escape codes for colorful output
    const std::string blue = "\033[34m";
    const std::string green = "\033[32m";
    const std::string yellow = "\033[33m";
    const std::string red = "\033[31m";
    const std::string reset = "\033[0m";

    // Convert seconds to hours, minutes, and seconds
    long hours = uptime_seconds / 3600;
    long minutes = (uptime_seconds % 3600) / 60;
    long seconds = uptime_seconds % 60;

    std::cout << blue << "System Uptime: " << reset;
    std::cout << green << hours << " hours " << minutes << " minutes " << seconds << " seconds" << reset << std::endl;

    std::cout << blue << "Load Averages: " << reset;
    std::cout << yellow << "1 min: " << load1 << reset << ", ";
    std::cout << yellow << "5 min: " << load5 << reset << ", ";
    std::cout << yellow << "15 min: " << load15 << reset << std::endl;

    // Fancy design for the output
    std::cout << "\n" << green << "===============================" << reset << std::endl;
    std::cout << green << "= " << yellow << "Have a nice day! :) " << green << " =";
    std::cout << "\n" << green << "===============================" << reset << std::endl;
}

// Main function to process the uptime command with flags
int main(int argc, char *argv[]) {
    bool beautiful_flag = false;

    // Check for the --beautiful flag
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--beautiful" || arg == "--lovely") {
            beautiful_flag = true;
        } else if (arg == "--help") {
            std::cout << "Usage: cryonix_uptime [--beautiful | --lovely]" << std::endl;
            return 0;
        }
    }

    // Get system uptime
    long uptime_seconds = get_system_uptime();
    if (uptime_seconds == -1) {
        return 1;
    }

    // Get load averages
    double load1, load5, load15;
    get_load_averages(load1, load5, load15);

    if (beautiful_flag) {
        // Print beautiful, colorful, and lovely output
        print_beautiful_output(uptime_seconds, load1, load5, load15);
    } else {
        // Default output similar to regular `uptime`
        std::cout << "System Uptime: " << uptime_seconds / 3600 << " hours "
                  << (uptime_seconds % 3600) / 60 << " minutes "
                  << uptime_seconds % 60 << " seconds" << std::endl;
        std::cout << "Load Averages: " << load1 << " (1 min), "
                  << load5 << " (5 min), "
                  << load15 << " (15 min)" << std::endl;
    }

    return 0;
}