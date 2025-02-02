//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <cstdlib>

void print_usage() {
    std::cerr << "Usage: cryonix_sed [OPTIONS] [FILE...]\n"
              << "Try 'cryonix_sed --help' for more information.\n";
}

// Apply substitution 's/old/new/'
void apply_substitution(const std::string& pattern, const std::string& replacement, std::string& line, bool global) {
    std::regex reg(pattern);
    if (global) {
        line = std::regex_replace(line, reg, replacement);
    } else {
        std::smatch match;
        if (std::regex_search(line, match, reg)) {
            line.replace(match.position(0), match.length(0), replacement);
        }
    }
}

// Handle line deletion ('d' command)
void handle_delete(std::string& line, bool& should_delete) {
    should_delete = true;
}

// Handle line print ('p' command)
void handle_print(std::string& line, bool should_print) {
    if (should_print) {
        std::cout << line << "\n";
    }
}

// Handle inserting text before the line ('i' command)
void handle_insert(std::string& line, const std::string& text) {
    std::cout << text << "\n" << line;
}

// Handle appending text after the line ('a' command)
void handle_append(std::string& line, const std::string& text) {
    std::cout << line << "\n" << text;
}

// Handle changing line content ('c' command)
void handle_change(std::string& line, const std::string& text) {
    line = text;
}

// Apply the = (line number) command
void handle_line_number(int line_number) {
    std::cout << line_number << "\n";
}

void process_file(const std::string& filename, const std::vector<std::string>& commands, bool inplace) {
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << "\n";
        exit(1);
    }

    std::ostringstream output_stream;
    std::string line;
    int line_number = 0;

    while (std::getline(input_file, line)) {
        line_number++;
        bool should_delete = false;

        for (const auto& cmd : commands) {
            if (cmd.substr(0, 1) == "s") {
                // Substitution command: s/old/new/g
                std::string pattern, replacement;
                bool global = false;
                std::stringstream cmd_stream(cmd.substr(1)); // Remove 's' character

                // Parse the substitution pattern and replacement
                std::getline(cmd_stream, pattern, '/');
                std::getline(cmd_stream, replacement, '/');
                if (cmd_stream.peek() == 'g') {
                    global = true;
                    cmd_stream.ignore();
                }

                apply_substitution(pattern, replacement, line, global);
            } else if (cmd == "d") {
                handle_delete(line, should_delete);
            } else if (cmd == "p") {
                handle_print(line, true);
            } else if (cmd.substr(0, 1) == "i") {
                handle_insert(line, cmd.substr(1));
            } else if (cmd.substr(0, 1) == "a") {
                handle_append(line, cmd.substr(1));
            } else if (cmd.substr(0, 1) == "c") {
                handle_change(line, cmd.substr(1));
            } else if (cmd == "=") {
                handle_line_number(line_number);
            }
        }

        if (!should_delete) {
            output_stream << line << "\n";
        }
    }

    if (inplace) {
        std::ofstream output_file(filename);
        if (!output_file.is_open()) {
            std::cerr << "Error: Unable to open file for writing: " << filename << "\n";
            exit(1);
        }
        output_file << output_stream.str();
    } else {
        std::cout << output_stream.str();
    }
}

void process_input(const std::vector<std::string>& commands, bool inplace, const std::vector<std::string>& filenames) {
    if (filenames.empty()) {
        std::string line;
        while (std::getline(std::cin, line)) {
            for (const auto& cmd : commands) {
                if (cmd.substr(0, 1) == "s") {
                    // Substitution command: s/old/new/g
                    std::string pattern, replacement;
                    bool global = false;
                    std::stringstream cmd_stream(cmd.substr(1)); // Remove 's' character

                    // Parse the substitution pattern and replacement
                    std::getline(cmd_stream, pattern, '/');
                    std::getline(cmd_stream, replacement, '/');
                    if (cmd_stream.peek() == 'g') {
                        global = true;
                        cmd_stream.ignore();
                    }

                    apply_substitution(pattern, replacement, line, global);
                }
            }
            std::cout << line << "\n";
        }
    } else {
        for (const auto& filename : filenames) {
            process_file(filename, commands, inplace);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    bool inplace = false;
    std::vector<std::string> filenames;
    std::vector<std::string> commands;

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-i") {
            inplace = true;
        } else if (argv[i][0] == '-') {
            std::cerr << "Error: Unknown option " << argv[i] << "\n";
            return 1;
        } else {
            filenames.push_back(argv[i]);
        }
    }

    // For this example, assume commands are passed as arguments
    // like 's/foo/bar/' for substitution commands
    commands.push_back("s/foo/bar/g"); // Example substitution

    process_input(commands, inplace, filenames);

    return 0;
}