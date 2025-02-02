//
// Created by cv2 on 02.02.2025.
//
#include <iostream>
#include <csignal>
#include <cstring>
#include <map>
#include <algorithm>
#include <stdexcept>

std::map<int, std::string> signal_map = {
    {SIGINT, "SIGINT"},
    {SIGTERM, "SIGTERM"},
    {SIGKILL, "SIGKILL"},
    {SIGSEGV, "SIGSEGV"},
    {SIGABRT, "SIGABRT"},
    {SIGFPE, "SIGFPE"},
    {SIGILL, "SIGILL"},
    {SIGQUIT, "SIGQUIT"},
    {SIGBUS, "SIGBUS"},
    {SIGSYS, "SIGSYS"},
    {SIGPIPE, "SIGPIPE"},
    {SIGALRM, "SIGALRM"},
    {SIGTSTP, "SIGTSTP"},
    {SIGCHLD, "SIGCHLD"},
    {SIGCONT, "SIGCONT"},
    {SIGSTOP, "SIGSTOP"},
    {SIGTTIN, "SIGTTIN"},
    {SIGTTOU, "SIGTTOU"},
    {SIGURG, "SIGURG"},
    {SIGXCPU, "SIGXCPU"},
    {SIGXFSZ, "SIGXFSZ"},
    {SIGVTALRM, "SIGVTALRM"},
    {SIGPROF, "SIGPROF"},
    {SIGWINCH, "SIGWINCH"},
    {SIGIO, "SIGIO"},
    {SIGPOLL, "SIGPOLL"},
    {SIGUSR1, "SIGUSR1"},
    {SIGUSR2, "SIGUSR2"}
};

void print_usage() {
    std::cout << "Usage: cryonix_operand2sig [OPTIONS] operand\n"
              << "Convert signal numbers to signal names and vice versa.\n\n"
              << "  operand          Signal number or signal name (e.g., 2 or SIGINT)\n"
              << "  -h, --help       Display this help message\n";
}

void operand_to_signal(const std::string& operand) {
    // First, check if operand is a valid signal name
    if (operand.substr(0, 3) == "SIG") {
        std::string signal_name = operand;
        std::transform(signal_name.begin(), signal_name.end(), signal_name.begin(), ::toupper);
        auto it = std::find_if(signal_map.begin(), signal_map.end(), [&signal_name](const auto& pair) {
            return pair.second == signal_name;
        });

        if (it != signal_map.end()) {
            std::cout << "\033[1;32mSignal: " << it->second << " => " << it->first << "\033[0m\n";
        } else {
            std::cerr << "\033[1;31mError: Invalid signal name\033[0m\n";
        }
    }
    // Else, assume operand is a signal number
    else {
        try {
            int signal_num = std::stoi(operand);
            auto it = signal_map.find(signal_num);
            if (it != signal_map.end()) {
                std::cout << "\033[1;32mSignal: " << it->second << " => " << it->first << "\033[0m\n";
            } else {
                std::cerr << "\033[1;31mError: Invalid signal number\033[0m\n";
            }
        } catch (const std::invalid_argument&) {
            std::cerr << "\033[1;31mError: Operand must be a valid number or signal name\033[0m\n";
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        print_usage();
        return 1;
    }

    std::string operand = argv[1];

    if (operand == "-h" || operand == "--help") {
        print_usage();
        return 0;
    }

    operand_to_signal(operand);
    return 0;
}