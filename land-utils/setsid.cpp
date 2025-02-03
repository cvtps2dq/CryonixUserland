//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>

void print_usage(const char* prog_name) {
    std::cerr << "Usage: " << prog_name << " [options] <program> [arguments ...]" << std::endl;
    std::cerr << "Run a program in a new session.\n" << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << " -c, --ctty     set the controlling terminal to the current one" << std::endl;
    std::cerr << " -f, --fork     always fork" << std::endl;
    std::cerr << " -w, --wait     wait program to exit, and use the same return" << std::endl;
    std::cerr << " -h, --help     display this help" << std::endl;
    std::cerr << " -V, --version  display version" << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
    }

    bool fork_flag = false;
    bool wait_flag = false;
    bool ctty_flag = false;

    int opt;
    while ((opt = getopt(argc, argv, "cfwhV")) != -1) {
        switch (opt) {
            case 'c':
                ctty_flag = true;
                break;
            case 'f':
                fork_flag = true;
                break;
            case 'w':
                wait_flag = true;
                break;
            case 'h':
                print_usage(argv[0]);
                break;
            case 'V':
                std::cout << "setsid clone 1.0" << std::endl;
                exit(EXIT_SUCCESS);
            default:
                print_usage(argv[0]);
        }
    }

    if (optind >= argc) {
        print_usage(argv[0]);
    }

    if (setsid() == -1) {
        std::cerr << "Failed to create new session: " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    if (ctty_flag) {
        int fd = open("/dev/tty", O_RDWR);
        if (fd >= 0) {
            ioctl(fd, TIOCSCTTY, 0);
            close(fd);
        } else {
            std::cerr << "Failed to set controlling terminal: " << strerror(errno) << std::endl;
            return EXIT_FAILURE;
        }
    }

    if (fork_flag) {
        pid_t pid = fork();
        if (pid > 0) {
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            std::cerr << "Fork failed: " << strerror(errno) << std::endl;
            return EXIT_FAILURE;
        }
    }

    pid_t child_pid = fork();
    if (child_pid == 0) {
        execvp(argv[optind], &argv[optind]);
        std::cerr << "Failed to execute command: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (wait_flag) {
        int status;
        waitpid(child_pid, &status, 0);
        return WEXITSTATUS(status);
    }

    return EXIT_SUCCESS;
}
