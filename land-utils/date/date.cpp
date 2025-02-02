#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <string>

// Function to print the usage instructions
void print_usage() {
    std::cout << "Usage: cryonix_date [--pretty]\n"
              << "Print the current date or an ASCII calendar with clock for the current month.\n"
              << "  --pretty    Print a pretty calendar of the current month with clock and highlight current day.\n";
}

// Function to get the current date and time in a string format
std::string get_current_date() {
    const std::time_t t = std::time(nullptr);
    const std::tm* tm = std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(tm, "%a %b %d %H:%M:%S %Y");  // Default date format
    return oss.str();
}

// Function to print the calendar with the current date highlighted
void print_calendar(const int month, const int year) {
    std::tm time_in = {};
    time_in.tm_year = year - 1900;  // Year since 1900
    time_in.tm_mon = month - 1;     // 0-based month
    time_in.tm_mday = 1;            // Start from the first day of the month
    std::mktime(&time_in);          // Normalize the tm structure

    // Get the weekday of the 1st day of the month
    const int first_day = time_in.tm_wday;  // 0: Sunday, 1: Monday, ..., 6: Saturday
    int days_in_month = 31;

    // Get the number of days in the current month
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        days_in_month = 30;
    } else if (month == 2) {
        // Check for leap year
        days_in_month = year % 4 == 0 && (year % 100 != 0 || year % 400 == 0) ? 29 : 28;
    }

    // Get current day to highlight it
    const std::time_t t = std::time(nullptr);
    const std::tm* tm_current = std::localtime(&t);
    const int current_day = tm_current->tm_mday;

    // Print current time (clock)
    std::cout << "\nCurrent Time: " << std::put_time(tm_current, "%H:%M:%S") << "\n\n";

    // Print month and year
    std::cout << "   " << std::setw(10) << std::put_time(&time_in, "%B %Y") << "\n";
    std::cout << " Su Mo Tu We Th Fr Sa\n";

    // Print leading spaces for the first day
    for (int i = 0; i < first_day; ++i) {
        std::cout << "   ";
    }

    // Print the days of the month
    for (int day = 1; day <= days_in_month; ++day) {
        // Highlight the current day with ANSI color codes
        if (day == current_day) {
            std::cout << "\033[48;5;11m\033[1m";  // Yellow background and bold text
            std::cout << std::setw(2) << day;
            std::cout << "\033[0m ";  // Reset color
        } else {
            std::cout << std::setw(2) << day << " ";
        }

        // Move to the next line at the end of the week (Saturday)
        if ((first_day + day) % 7 == 0) {
            std::cout << "\n";
        }
    }

    // Print trailing spaces to align the last week
    if ((first_day + days_in_month) % 7 != 0) {
        std::cout << "\n";
    }
}

// Main function to handle user input and display the appropriate output
int main(const int argc, char* argv[]) {
    if (argc > 2) {
        print_usage();
        return 1;
    }

    if (argc == 2 && std::string(argv[1]) == "--pretty") {
        // Get current date and time
        const std::time_t t = std::time(nullptr);
        const std::tm* tm = std::localtime(&t);
        print_calendar(tm->tm_mon + 1, tm->tm_year + 1900);  // Display pretty calendar
    } else {
        std::cout << get_current_date() << "\n";  // Display current date
    }

    return 0;
}