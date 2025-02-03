//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"

enum BufferMode { NONE, LINE, FULL };

// Function to set buffering mode
void set_buffering(FILE* stream, BufferMode mode) {
    switch (mode) {
        case NONE:
            setvbuf(stream, nullptr, _IONBF, 0);
            break;
        case LINE:
            setvbuf(stream, nullptr, _IOLBF, BUFSIZ);
            break;
        case FULL:
            setvbuf(stream, nullptr, _IOFBF, BUFSIZ);
            break;
    }
}

// Function to parse buffering mode from string
BufferMode parse_buffer_mode(const std::string& mode) {
    if (mode == "none") return NONE;
    if (mode == "line") return LINE;
    if (mode == "full") return FULL;
    return FULL; // Default to fully buffered if invalid input
}

// Function to execute the command with the specified buffering mode
void execute_with_buffering(int argc, char* argv[], BufferMode stdin_mode, BufferMode stdout_mode, BufferMode stderr_mode) {
    // Set buffering for stdin, stdout, and stderr
    set_buffering(stdin, stdin_mode);
    set_buffering(stdout, stdout_mode);
    set_buffering(stderr, stderr_mode);

    // Execute the command
    execvp(argv[0], argv);
    std::cerr << COLOR_RED << "Error: Command execution failed" << COLOR_RESET << std::endl;
    exit(1); // Exit if execvp fails
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << COLOR_RED << "Usage: cryonix_stdbuf <stdin_mode> <stdout_mode> <stderr_mode> -- <command> [args]" << COLOR_RESET << std::endl;
        return 1;
    }

    // Parsing the buffer modes for stdin, stdout, stderr
    BufferMode stdin_mode = parse_buffer_mode(argv[1]);
    BufferMode stdout_mode = parse_buffer_mode(argv[2]);
    BufferMode stderr_mode = parse_buffer_mode(argv[3]);

    // Find the command and its arguments
    char* new_argv[argc - 3 + 1]; // +1 for the NULL argument for execvp
    std::memcpy(new_argv, &argv[4], (argc - 4) * sizeof(char*));
    new_argv[argc - 4] = nullptr; // NULL-terminate the argument list

    // Execute the command with the specified buffering modes
    execute_with_buffering(argc, argv, stdin_mode, stdout_mode, stderr_mode);

    return 0;
}