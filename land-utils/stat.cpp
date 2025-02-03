//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <sys/stat.h>
#include <ctime>
#include <iomanip>
#include <unistd.h>
#include <cstring>

#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_CYAN    "\033[36m"

// Function to format time
std::string format_time(time_t raw_time) {
    struct tm *time_info;
    char buffer[80];

    time_info = localtime(&raw_time);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", time_info);

    return {buffer};
}

// Function to convert file permissions to a human-readable string
std::string get_permissions(mode_t mode) {
    std::string permissions = "---------";
    if (S_IRUSR & mode) permissions[0] = 'r';
    if (S_IWUSR & mode) permissions[1] = 'w';
    if (S_IXUSR & mode) permissions[2] = 'x';
    if (S_IRGRP & mode) permissions[3] = 'r';
    if (S_IWGRP & mode) permissions[4] = 'w';
    if (S_IXGRP & mode) permissions[5] = 'x';
    if (S_IROTH & mode) permissions[6] = 'r';
    if (S_IWOTH & mode) permissions[7] = 'w';
    if (S_IXOTH & mode) permissions[8] = 'x';

    return permissions;
}

// Function to display the file's detailed information
void print_stat(const char* filepath) {
    struct stat file_stat;

    if (stat(filepath, &file_stat) == -1) {
        std::cerr << COLOR_RED << "Error: Unable to get file status for " << filepath << COLOR_RESET << std::endl;
        return;
    }

    // File name
    std::cout << COLOR_BOLD << "File: " << COLOR_GREEN << filepath << COLOR_RESET << std::endl;

    // File type
    std::cout << COLOR_BOLD << "File Type: ";
    if (S_ISDIR(file_stat.st_mode)) std::cout << COLOR_CYAN << "Directory" << COLOR_RESET << std::endl;
    else if (S_ISREG(file_stat.st_mode)) std::cout << COLOR_GREEN << "Regular File" << COLOR_RESET << std::endl;
    else if (S_ISLNK(file_stat.st_mode)) std::cout << COLOR_YELLOW << "Symbolic Link" << COLOR_RESET << std::endl;
    else std::cout << COLOR_RED << "Other" << COLOR_RESET << std::endl;

    // File permissions
    std::cout << COLOR_BOLD << "Permissions: " << COLOR_GREEN << get_permissions(file_stat.st_mode) << COLOR_RESET << std::endl;

    // Number of hard links
    std::cout << COLOR_BOLD << "Links: " << COLOR_GREEN << file_stat.st_nlink << COLOR_RESET << std::endl;

    // Owner and group
    std::cout << COLOR_BOLD << "Owner: " << COLOR_YELLOW << file_stat.st_uid << COLOR_RESET << "  ";
    std::cout << COLOR_BOLD << "Group: " << COLOR_YELLOW << file_stat.st_gid << COLOR_RESET << std::endl;

    // File size in bytes
    std::cout << COLOR_BOLD << "Size: " << COLOR_GREEN << file_stat.st_size << " bytes" << COLOR_RESET << std::endl;

    // Block size and number of blocks
    std::cout << COLOR_BOLD << "Block Size: " << COLOR_GREEN << file_stat.st_blksize << " bytes" << COLOR_RESET << std::endl;
    std::cout << COLOR_BOLD << "Blocks: " << COLOR_GREEN << file_stat.st_blocks << COLOR_RESET << std::endl;

    // Last access time
    std::cout << COLOR_BOLD << "Last Access: " << COLOR_GREEN << format_time(file_stat.st_atime) << COLOR_RESET << std::endl;

    // Last modification time
    std::cout << COLOR_BOLD << "Last Modified: " << COLOR_GREEN << format_time(file_stat.st_mtime) << COLOR_RESET << std::endl;

    // Last status change time
    std::cout << COLOR_BOLD << "Last Status Change: " << COLOR_GREEN << format_time(file_stat.st_ctime) << COLOR_RESET << std::endl;

    // Device ID and inode number
    std::cout << COLOR_BOLD << "Device ID: " << COLOR_GREEN << file_stat.st_dev << COLOR_RESET << std::endl;
    std::cout << COLOR_BOLD << "Inode: " << COLOR_GREEN << file_stat.st_ino << COLOR_RESET << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << COLOR_RED << "Usage: cryonix_stat <file>" << COLOR_RESET << std::endl;
        return 1;
    }

    // Display detailed file stats
    print_stat(argv[1]);

    return 0;
}