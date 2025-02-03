//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>

void print_usage() {
    std::cout << "Usage: cryonix_kill [-l] [-s SIGNAL] PID...\n"
              << "Send signals to processes.\n\n"
              << "  -l           List all available signals\n"
              << "  -s SIGNAL    Send specified SIGNAL (name or number)\n";
}

std::map<std::string, int> signal_map = {
    {"SIGHUP", 1},   {"SIGINT", 2},  {"SIGQUIT", 3},  {"SIGILL", 4},
    {"SIGTRAP", 5},  {"SIGABRT", 6}, {"SIGBUS", 7},   {"SIGFPE", 8},
    {"SIGKILL", 9},  {"SIGUSR1", 10}, {"SIGSEGV", 11}, {"SIGUSR2", 12},
    {"SIGPIPE", 13}, {"SIGALRM", 14}, {"SIGTERM", 15}, {"SIGCHLD", 17},
    {"SIGCONT", 18}, {"SIGSTOP", 19}, {"SIGTSTP", 20}, {"SIGTTIN", 21},
    {"SIGTTOU", 22}
};

void list_signals() {
    std::cout << "\033[1;34mAvailable signals:\033[0m\n";
    for (const auto& [name, num] : signal_map) {
        std::cout << "\033[1;32m" << name << "\033[0m (" << num << ")\n";
    }
}

int get_signal(const std::string& signal_name) {
    if (std::isdigit(signal_name[0])) return std::stoi(signal_name);
    if (signal_map.contains(signal_name)) return signal_map[signal_name];
    return -1;
}

void send_signal(int signal, int pid) {
    if (kill(pid, signal) == 0) {
        std::cout << "\033[1;32mSent signal " << signal << " to PID " << pid << "\033[0m\n";
    } else {
        std::cerr << "\033[1;31mError: Failed to send signal to PID " << pid << "\033[0m\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    int signal = SIGTERM;  // Default signal
    std::vector<int> pids;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-l") {
            list_signals();
            return 0;
        } else if (arg == "-s" && i + 1 < argc) {
            signal = get_signal(argv[++i]);
            if (signal == -1) {
                std::cerr << "\033[1;31mError: Invalid signal name or number!\033[0m\n";
                return 1;
            }
        } else if (std::isdigit(arg[0])) {
            pids.push_back(std::stoi(arg));
        } else {
            std::cerr << "\033[1;31mError: Invalid argument '" << arg << "'\033[0m\n";
            return 1;
        }
    }

    if (pids.empty()) {
        std::cerr << "\033[1;31mError: No PIDs specified!\033[0m\n";
        return 1;
    }

    for (int pid : pids) {
        send_signal(signal, pid);
    }

    return 0;
}