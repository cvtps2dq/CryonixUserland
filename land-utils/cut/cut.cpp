//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

void print_usage() {
    std::cout << "Usage: cut [-d DELIM] [-f FIELDS] [-c RANGE] [FILE...]\n"
              << "Extract sections from each line of input.\n"
              << "  -d DELIM    Specify delimiter (default is tab).\n"
              << "  -f FIELDS   Specify fields to extract, separated by commas.\n"
              << "  -c RANGE    Specify the byte range to extract.\n";
}

std::vector<std::string> split(const std::string& str, const char delim) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(str);

    while (std::getline(token_stream, token, delim)) {
        tokens.push_back(token);
    }

    return tokens;
}

void process_input(std::istream& input, char delim, const std::string& fields, const std::string& range) {
    std::string line;
    while (std::getline(input, line)) {
        std::vector<std::string> tokens = split(line, delim);

        // Handle byte range with -c option
        if (!range.empty()) {
            int start = std::stoi(range.substr(0, range.find('-')));
            int end = range.find('-') == std::string::npos ? start : std::stoi(range.substr(range.find('-') + 1));

            // Extract specified byte range
            if (start < line.size()) {
                if (end > line.size()) end = static_cast<int>(line.size());
                std::cout << line.substr(start - 1, end - start + 1) << std::endl;
            }
            continue;
        }

        // Handle fields with -f option
        if (!fields.empty()) {
            std::vector<int> field_numbers;
            std::istringstream field_stream(fields);
            std::string field;
            while (std::getline(field_stream, field, ',')) {
                field_numbers.push_back(std::stoi(field));
            }

            // Print the requested fields
            for (size_t i = 0; i < field_numbers.size(); ++i) {
                if (int field_num = field_numbers[i]; field_num <= tokens.size()) {
                    std::cout << tokens[field_num - 1];
                }
                if (i < field_numbers.size() - 1) {
                    std::cout << "\t";  // Add delimiter between fields
                }
            }
            std::cout << std::endl;
            continue;
        }

        // Print the whole line if no options are specified
        std::cout << line << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    char delim = '\t';  // Default delimiter is tab
    std::string fields;
    std::string range;
    std::vector<std::string> files;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        if (std::string arg = argv[i]; arg == "-d") {
            if (i + 1 < argc) {
                delim = argv[++i][0];  // Use the first character as the delimiter
            } else {
                std::cerr << "Error: Missing delimiter after -d option.\n";
                return 1;
            }
        } else if (arg == "-f") {
            if (i + 1 < argc) {
                fields = argv[++i];
            } else {
                std::cerr << "Error: Missing fields after -f option.\n";
                return 1;
            }
        } else if (arg == "-c") {
            if (i + 1 < argc) {
                range = argv[++i];
            } else {
                std::cerr << "Error: Missing range after -c option.\n";
                return 1;
            }
        } else {
            files.push_back(arg);
        }
    }

    if (files.empty()) {
        // If no file is provided, read from stdin
        process_input(std::cin, delim, fields, range);
    } else {
        // Process each file
        for (const auto& file : files) {
            std::ifstream infile(file);
            if (!infile.is_open()) {
                std::cerr << "Error: Could not open file '" << file << "'\n";
                return 1;
            }
            process_input(infile, delim, fields, range);
        }
    }

    return 0;
}