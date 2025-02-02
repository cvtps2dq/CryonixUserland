//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <unistd.h>

void print_usage() {
    std::cout << "Usage: showenv [VARIABLE...]\n"
              << "Display environment variables.\n\n"
              << "If no variables are specified, all environment variables are displayed.\n"
              << "  -h, --help               Display this help message\n";
}

void print_env_variable(const std::string& var_name, const std::string& var_value) {
    // Colorize the output for better readability
    std::cout << "\033[1;32m" << var_name << "\033[0m=\033[1;34m" << var_value << "\033[0m\n";
}

void print_all_env_variables() {
    extern char **environ;
    for (char **env = environ; *env != nullptr; ++env) {
        std::string env_entry = *env;
        size_t pos = env_entry.find('=');
        if (pos != std::string::npos) {
            std::string var_name = env_entry.substr(0, pos);
            std::string var_value = env_entry.substr(pos + 1);
            print_env_variable(var_name, var_value);
        }
    }
}

void print_specific_env_variables(const std::vector<std::string>& vars) {
    for (const auto& var_name : vars) {
        const char* var_value = std::getenv(var_name.c_str());
        if (var_value != nullptr) {
            print_env_variable(var_name, var_value);
        } else {
            std::cerr << "\033[1;31mError: Variable " << var_name << " not found\033[0m\n";
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_all_env_variables();
        return 0;
    }

    bool show_help = false;
    std::vector<std::string> variables;

    // Parse flags and arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            show_help = true;
            break;
        } else {
            variables.push_back(arg);
        }
    }

    if (show_help) {
        print_usage();
        return 0;
    }

    if (variables.empty()) {
        print_all_env_variables();
    } else {
        print_specific_env_variables(variables);
    }

    return 0;
}