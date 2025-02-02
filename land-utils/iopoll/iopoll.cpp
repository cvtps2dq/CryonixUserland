//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <vector>
#include <filesystem>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>

namespace fs = std::filesystem;

constexpr int MAX_EVENTS = 10;

void print_usage() {
    std::cout << "Usage: cryonix_iopoll [FILES]...\n"
              << "Monitor I/O events on given files or devices.\n";
}

void monitor_files(const std::vector<std::string>& files) {
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "\033[1;31mError: Failed to create epoll instance!\033[0m\n";
        return;
    }

    std::vector<int> fds;
    struct epoll_event event, events[MAX_EVENTS];

    // Register files for monitoring
    for (const auto& file : files) {
        int fd = open(file.c_str(), O_RDONLY | O_NONBLOCK);
        if (fd == -1) {
            std::cerr << "\033[1;31mError: Cannot open " << file << "\033[0m\n";
            continue;
        }

        event.events = EPOLLIN; // Monitor for readable data
        event.data.fd = fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
            std::cerr << "\033[1;31mError: Failed to add " << file << " to epoll!\033[0m\n";
            close(fd);
            continue;
        }
        fds.push_back(fd);
        std::cout << "\033[1;32mMonitoring: " << file << "\033[0m\n";
    }

    std::cout << "Listening for I/O events...\n";
    while (true) {
        int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (event_count == -1) {
            std::cerr << "\033[1;31mError: epoll_wait failed!\033[0m\n";
            break;
        }

        for (int i = 0; i < event_count; ++i) {
            if (events[i].events & EPOLLIN) {
                std::cout << "\033[1;34mI/O event detected on file descriptor: "
                          << events[i].data.fd << "\033[0m\n";
            }
        }
    }

    // Cleanup
    for (int fd : fds) close(fd);
    close(epoll_fd);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::vector<std::string> files;
    for (int i = 1; i < argc; ++i) {
        files.push_back(argv[i]);
    }

    monitor_files(files);
    return 0;
}