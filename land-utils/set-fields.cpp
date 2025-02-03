//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Function to split a line into fields based on a delimiter
std::vector<std::string> split_line(const std::string& line, char delimiter) {
    std::vector<std::string> fields;
    std::string field;
    std::stringstream ss(line);

    while (std::getline(ss, field, delimiter)) {
        fields.push_back(field);
    }

    return fields;
}

// Function to join fields back into a line with the delimiter
std::string join_fields(const std::vector<std::string>& fields, char delimiter) {
    std::string line;
    for (size_t i = 0; i < fields.size(); ++i) {
        line += fields[i];
        if (i < fields.size() - 1) {
            line += delimiter;
        }
    }
    return line;
}

// Function to process the file and set the specified field values
void set_fields(const std::string& input_file, const std::string& output_file,
                char delimiter, int field_num, const std::string& new_value) {
    std::ifstream infile(input_file);
    std::ofstream outfile(output_file);
    std::string line;

    if (!infile.is_open()) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return;
    }
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open output file." << std::endl;
        return;
    }

    while (std::getline(infile, line)) {
        std::vector<std::string> fields = split_line(line, delimiter);

        if (field_num > 0 && field_num <= fields.size()) {
            // Update the specified field (1-based index)
            fields[field_num - 1] = new_value;
        } else {
            std::cerr << "Error: Invalid field number for line: " << line << std::endl;
        }

        // Write the modified line to the output file
        outfile << join_fields(fields, delimiter) << std::endl;
    }

    infile.close();
    outfile.close();
    std::cout << "Fields updated successfully in the file: " << output_file << std::endl;
}

// Function to print the help message
void print_usage() {
    std::cout << "Usage: cryonix_set-fields [options] input_file output_file\n"
              << "Update fields in a delimited file.\n\n"
              << "Options:\n"
              << "  -d, --delimiter DELIM   The delimiter used to separate fields (default: space)\n"
              << "  -f, --field FIELD       The field number to update (1-based index)\n"
              << "  -v, --value VALUE       The new value to set in the specified field\n";
}

int main(int argc, char* argv[]) {
    std::string input_file, output_file, new_value;
    char delimiter = ' '; // default delimiter is space
    int field_num = 0;

    // Parse flags and arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            print_usage();
            return 0;
        } else if (arg == "-d" || arg == "--delimiter") {
            if (i + 1 < argc) {
                delimiter = argv[++i][0]; // Get the delimiter character
            }
        } else if (arg == "-f" || arg == "--field") {
            if (i + 1 < argc) {
                field_num = std::stoi(argv[++i]);
            }
        } else if (arg == "-v" || arg == "--value") {
            if (i + 1 < argc) {
                new_value = argv[++i];
            }
        } else if (input_file.empty()) {
            input_file = arg;
        } else if (output_file.empty()) {
            output_file = arg;
        }
    }

    // Validate input arguments
    if (input_file.empty() || output_file.empty() || field_num == 0 || new_value.empty()) {
        std::cerr << "Error: Missing required arguments." << std::endl;
        print_usage();
        return 1;
    }

    // Process the file and update the fields
    set_fields(input_file, output_file, delimiter, field_num, new_value);

    return 0;
}