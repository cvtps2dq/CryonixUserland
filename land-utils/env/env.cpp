//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <unistd.h>

// Function to print the environment variables
void print_environment() {
    extern char **environ;
    for (char **env = environ; *env != nullptr; ++env) {
        std::cout << *env << "\n";
    }
}

// Function to parse and set environment variables
void set_environment_variables(const std::vector<std::string>& env_vars) {
    for (const auto& var : env_vars) {
        if (const size_t pos = var.find('='); pos != std::string::npos) {
            std::string name = var.substr(0, pos);
            std::string value = var.substr(pos + 1);
            setenv(name.c_str(), value.c_str(), 1);
        }
    }
}

// Function to execute the command with the modified environment
void execute_command(const std::vector<std::string>& args) {
    std::vector<char*> c_args;
    c_args.reserve(args.size());
for (const auto& arg : args) {
        c_args.push_back(const_cast<char*>(arg.c_str()));
    }
    c_args.push_back(nullptr); // Null-terminate the array of arguments

    if (execvp(c_args[0], c_args.data()) == -1) {
        std::cerr << "Error executing command: " << strerror(errno) << "\n";
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    // If no command is given, print the environment variables
    if (argc == 1) {
        print_environment();
        return 0;
    }

    // Collect environment variable settings (e.g., VAR=value)
    std::vector<std::string> env_vars;
    int i = 1;
    while (i < argc && strchr(argv[i], '=') != nullptr) {
        env_vars.emplace_back(argv[i]);
        ++i;
    }

    // Set the environment variables
    set_environment_variables(env_vars);

    // If there's a command after the environment variables, run it
    if (i < argc) {
        const std::vector<std::string> args(argv + i, argv + argc);
        execute_command(args);
    }

    return 0;
}