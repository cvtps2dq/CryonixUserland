//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <string>


// Function to print usage instructions
void print_usage() {
    std::cerr << "Usage: cryonix_tr SET1 [SET2] or cryonix_tr [OPTION]... SET1 [SET2]...\n";
    std::cerr << "Translate or delete characters from the input.\n";
    std::cerr << "\nOptions:\n";
    std::cerr << "  -d, --delete    : Delete characters in SET1, do not translate.\n";
    std::cerr << "  -c, --complement : Complement the SET1 characters.\n";
    std::cerr << "  -s, --squeeze   : Squeeze multiple adjacent occurrences of the same character.\n";
}

// Function to delete characters from the input
std::string delete_chars(const std::string& input, const std::string& set) {
    std::string result;
    for (char c : input) {
        if (set.find(c) == std::string::npos) {  // Skip characters in the set
            result.push_back(c);
        }
    }
    return result;
}

// Function to complement characters from the input
std::string complement_chars(const std::string& input, const std::string& set) {
    std::string result;
    for (char c : input) {
        if (set.find(c) == std::string::npos) {  // Complement if character not in the set
            result.push_back(c);
        }
    }
    return result;
}

// Function to translate characters from one set to another
std::string translate_chars(const std::string& input, const std::string& set1, const std::string& set2) {
    std::string result;
    for (char c : input) {
        size_t index = set1.find(c);
        if (index != std::string::npos) {
            result.push_back(set2[index % set2.size()]);  // Use modulus to handle different sizes
        } else {
            result.push_back(c);  // If not in set1, just keep the character
        }
    }
    return result;
}

// Function to squeeze multiple adjacent characters in the input
std::string squeeze_chars(const std::string& input) {
    std::string result;
    for (size_t i = 0; i < input.size(); ++i) {
        if (i == 0 || input[i] != input[i - 1]) {  // Only add if not the same as previous
            result.push_back(input[i]);
        }
    }
    return result;
}

// Main function
int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }

    bool delete_flag = false;
    bool complement_flag = false;
    bool squeeze_flag = false;
    std::string set1, set2;

    // Parse flags
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-d" || arg == "--delete") {
            delete_flag = true;
        } else if (arg == "-c" || arg == "--complement") {
            complement_flag = true;
        } else if (arg == "-s" || arg == "--squeeze") {
            squeeze_flag = true;
        } else if (set1.empty()) {
            set1 = arg;
        } else {
            set2 = arg;
        }
    }

    if (set1.empty()) {
        std::cerr << "Error: Missing SET1 argument.\n";
        print_usage();
        return 1;
    }

    std::string input;
    std::getline(std::cin, input);  // Read the entire input from stdin

    if (delete_flag) {
        input = delete_chars(input, set1);
    } else if (complement_flag) {
        input = complement_chars(input, set1);
    } else {
        input = translate_chars(input, set1, set2);
    }

    if (squeeze_flag) {
        input = squeeze_chars(input);
    }

    std::cout << input << std::endl;  // Output the modified string

    return 0;
}