//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cctype>

// Function to print the usage instructions
void print_usage() {
    std::cout << "Usage: cryonix_expr <expression>\n"
              << "Evaluate expressions. Supports arithmetic, string operations, and logical comparisons.\n";
}

// Function to evaluate an arithmetic expression
int eval_arithmetic(const int a, const int b, const std::string& op) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") {
        if (b == 0) throw std::runtime_error("Division by zero");
        return a / b;
    }
    if (op == "%") {
        if (b == 0) throw std::runtime_error("Modulo by zero");
        return a % b;
    }
    throw std::invalid_argument("Invalid arithmetic operator");
}

// Function to evaluate a logical expression
bool eval_logical(const int a, const int b, const std::string& op) {
    if (op == "==") return a == b;
    if (op == "!=") return a != b;
    if (op == "<") return a < b;
    if (op == ">") return a > b;
    throw std::invalid_argument("Invalid logical operator");
}

// Function to evaluate a string operation (concatenation or length)
std::string eval_string(const std::string& a, const std::string& b, const std::string& op) {
    if (op == "+") return a + b;
    throw std::invalid_argument("Invalid string operator");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        print_usage();
        return 1;
    }

    std::string expr = argv[1];
    std::istringstream ss(expr);
    std::string token;
    ss >> token;

    try {
        if (isdigit(token[0])) {
            // Handle arithmetic expressions
            int a = std::stoi(token);
            ss >> token;
            std::string op = token;
            ss >> token;
            int b = std::stoi(token);
            std::cout << eval_arithmetic(a, b, op) << std::endl;
        } else if (token[0] == '"') {
            // Handle string expressions
            std::string a = token.substr(1, token.size() - 2);  // Remove surrounding quotes
            ss >> token;
            std::string op = token;
            ss >> token;
            std::string b = token.substr(1, token.size() - 2);  // Remove surrounding quotes
            std::cout << eval_string(a, b, op) << std::endl;
        } else if (token == "true" || token == "false") {
            // Handle logical operations
            bool a = token == "true";
            ss >> token;
            std::string op = token;
            ss >> token;
            bool b = token == "true";
            std::cout << eval_logical(a, b, op) << std::endl;
        } else {
            throw std::invalid_argument("Invalid expression");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}