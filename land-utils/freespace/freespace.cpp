//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sys/statvfs.h>
#include <iomanip>
#include <sstream>

// this will fail on macOS. run on linux.
void print_usage() {
    std::cout << "Usage: cryonix_df\n"
              << "Display the amount of disk space used and available on the file systems.\n";
}

// Function to convert bytes to human-readable format (e.g., KB, MB, GB)
std::string format_size(long long size) {
    const char* suffix[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0;
    double formatted_size = size;

    while (formatted_size >= 1024 && i < 4) {
        formatted_size /= 1024;
        ++i;
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << formatted_size << " " << suffix[i];
    return oss.str();
}

// Function to get file system information and print it with color
void print_filesystem_info() {
    std::ifstream proc_mounts("/proc/mounts");
    if (!proc_mounts.is_open()) {
        std::cerr << "Error opening /proc/mounts file.\n";
        return;
    }

    std::string line;
    while (std::getline(proc_mounts, line)) {
        std::istringstream iss(line);
        std::string device, mount_point, fstype, options;
        if (!(iss >> device >> mount_point >> fstype >> options)) {
            continue;  // Skip invalid lines
        }

        // Skip system mount points like /proc, /sys, /dev, etc.
        if (mount_point == "/" || mount_point == "/proc" || mount_point == "/sys" || mount_point == "/dev") {
            continue;
        }

        struct statvfs buf;
        if (statvfs(mount_point.c_str(), &buf) != 0) {
            std::cerr << "Error getting file system stats for: " << mount_point << "\n";
            continue;
        }

        long long total = buf.f_blocks * buf.f_frsize;
        long long free = buf.f_bfree * buf.f_frsize;
        long long available = buf.f_bavail * buf.f_frsize;
        long long used = total - free;

        // Colorizing the output
        std::cout << std::left << std::setw(20) << device;
        std::cout << std::setw(10) << format_size(total);
        std::cout << " ";

        // Used space in red
        std::cout << "\033[38;5;160m" << std::setw(10) << format_size(used) << "\033[0m";

        // Available space in green
        std::cout << "\033[38;5;82m" << std::setw(10) << format_size(available) << "\033[0m";

        std::cout << std::setw(20) << mount_point << "\n";
    }

    proc_mounts.close();
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        print_usage();
        return 1;
    }

    std::cout << "\nFilesystem Disk Space Usage:\n";
    std::cout << std::setw(20) << "Filesystem" << std::setw(10) << "Size" << std::setw(10) << "Used" << std::setw(10) << "Avail" << std::setw(20) << "Mounted on" << "\n";
    print_filesystem_info();
    return 0;
}