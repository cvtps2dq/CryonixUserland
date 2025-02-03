//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void print_usage() {
    std::cout << "Usage: cryonix_nohup COMMAND [ARGUMENTS...]\n"
              << "Run a command immune to hangups and redirect its output.\n\n"
              << "  -o FILE    Redirect output to FILE (default: nohup.out)\n"
              << "  -p         Suppress output and prevent redirection\n";
}

void execute_nohup(const std::string& command, const std::string& output_file, bool suppress_output) {
    // Forking the process
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "\033[1;31mError: Fork failed!\033[0m\n";
        exit(1);
    }

    if (pid == 0) {  // Child process
        // Make the process immune to hangups (SIGHUP)
        signal(SIGHUP, SIG_IGN);

        if (!suppress_output) {
            // Redirect standard output to a file
            int output_fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (output_fd == -1) {
                std::cerr << "\033[1;31mError: Unable to open output file " << output_file << "\033[0m\n";
                exit(1);
            }

            // Redirect stdout and stderr to the output file
            if (dup2(output_fd, STDOUT_FILENO) == -1 || dup2(output_fd, STDERR_FILENO) == -1) {
                std::cerr << "\033[1;31mError: Failed to redirect output\033[0m\n";
                exit(1);
            }
        }

        // Execute the command
        execlp(command.c_str(), command.c_str(), nullptr);
        std::cerr << "\033[1;31mError: Failed to execute command\033[0m\n";
        exit(1);
    }

    // Parent process exits immediately
    std::cout << "\033[1;32mRunning in background. Output will be written to " << output_file << "\033[0m\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::string output_file = "nohup.out";
    bool suppress_output = false;
    std::string command;

    // Parse flags and arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-o") {
            if (i + 1 < argc) {
                output_file = argv[++i];
            } else {
                std::cerr << "\033[1;31mError: Missing argument for -o option\033[0m\n";
                return 1;
            }
        } else if (arg == "-p") {
            suppress_output = true;
        } else {
            if (command.empty()) {
                command = arg;
            }
        }
    }

    if (command.empty()) {
        std::cerr << "\033[1;31mError: No command specified\033[0m\n";
        return 1;
    }

    execute_nohup(command, output_file, suppress_output);
    return 0;
}