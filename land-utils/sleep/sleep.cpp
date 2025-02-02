//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>

// Flag to control if the sleep should be interrupted
bool interrupted = false;

void signal_handler(int signum) {
    if (signum == SIGINT) {
        interrupted = true;
        std::cout << "\nYou woke me up! :( I'm trying to sleep here!" << std::endl;
    } else if (signum == SIGTERM) {
        interrupted = true;
        std::cout << "\nI see you're trying to terminate me... :(" << std::endl;
    } else {
        std::cout << "\nUnknown signal received. Ignoring it... " << std::endl;
    }
}

void custom_sleep(int seconds) {
    std::cout << "Sleeping for " << seconds << " second" << (seconds == 1 ? "" : "s") << "..." << std::endl;

    // Simulate sleeping and allow for interruption with Ctrl+C
    for (int i = 0; i < seconds; ++i) {
        if (interrupted) {
            return; // Exit early if interrupted
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // If we weren't interrupted, print a funny message when finished
    if (!interrupted) {
        std::cout << "I finally woke up! " << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // Check if there's a duration specified
    if (argc != 2) {
        std::cout << "Usage: cryonix_sleep <seconds>" << std::endl;
        return 1;
    }

    // Parse the number of seconds to sleep
    int seconds = std::stoi(argv[1]);

    if (seconds <= 0) {
        std::cout << "You can't make me sleep for negative or zero time! " << std::endl;
        return 1;
    }

    // Register signal handler for Ctrl+C
    signal(SIGINT, signal_handler);

    // Start the sleep process
    custom_sleep(seconds);

    return 0;
}