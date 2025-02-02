//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <chrono>
#include <thread>

void print_usage() {
    std::cerr << "Usage: cryonix_timeout <duration> <command> [arguments...]\n";
    std::cerr << "Duration format: [TIME][SUFFIX]\n";
    std::cerr << "SUFFIX: 's' for seconds (default), 'm' for minutes, 'h' for hours, 'd' for days\n";
}

long parse_duration(const std::string& duration_str) {
    if (duration_str.empty()) {
        return 0;
    }

    char suffix = duration_str.back();
    long duration = std::stol(duration_str.substr(0, duration_str.size() - 1));

    switch (suffix) {
        case 's': return duration;
        case 'm': return duration * 60;
        case 'h': return duration * 3600;
        case 'd': return duration * 86400;
        default:
            std::cerr << "Error: Invalid duration suffix. Use 's', 'm', 'h', or 'd'.\n";
            return -1;
    }
}

void run_command_with_timeout(const std::string& duration_str, const std::vector<std::string>& command) {
    long timeout = parse_duration(duration_str);
    if (timeout == -1) return;

    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "Error: Failed to fork process.\n";
        return;
    }

    if (pid == 0) {
        // Child process
        std::vector<const char*> cmd_args;
        for (const auto& arg : command) {
            cmd_args.push_back(arg.c_str());
        }
        cmd_args.push_back(nullptr);

        execvp(cmd_args[0], const_cast<char* const*>(cmd_args.data()));
        std::cerr << "Error: Failed to execute command.\n";
        exit(1);
    } else {
        // Parent process
        std::this_thread::sleep_for(std::chrono::seconds(timeout));

        // Check if the child process is still running
        if (waitpid(pid, nullptr, WNOHANG) == 0) {
            // Terminate the process if it's still running after the timeout
            std::cout << "Timeout reached. Sending SIGTERM to process " << pid << "...\n";
            kill(pid, SIGTERM);
            waitpid(pid, nullptr, 0);  // Wait for the process to exit
            std::cout << "Process " << pid << " terminated.\n";
        } else {
            std::cout << "Command completed within the timeout.\n";
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }

    std::string duration = argv[1];
    std::vector<std::string> command(argv + 2, argv + argc);

    run_command_with_timeout(duration, command);

    return 0;
}