#include <iostream>
#include <string>
#include <sys/stat.h>
#include <cerrno>
#include <stdexcept>
#include <format>
#include <vector>
#include <span>

void print_usage() {
    std::cout << std::format("Usage: cryonix_chmod [OPTION] MODE FILE...\n"
                             "Change file mode (permissions) of the specified FILE(s)\n\n"
                             "MODE can be a symbolic mode or a numeric mode.\n"
                             "For symbolic mode, use: [ugoa...][+|-|=][rwx...].\n"
                             "For numeric mode, use a three-digit octal number.\n");
}

mode_t parse_numeric_mode(const std::string& mode_str) {
    try {
        return std::stoi(mode_str, nullptr, 8);
    } catch (const std::invalid_argument&) {
        std::cerr << "Invalid numeric mode: " << mode_str << '\n';
        throw std::invalid_argument("Invalid numeric mode");
    }
}

mode_t permission_mask(const char permission) {
    switch (permission) {
        case 'r': return S_IRUSR; // 0400
        case 'w': return S_IWUSR; // 0200
        case 'x': return S_IXUSR; // 0100
        default: return 0;
    }
}

mode_t apply_permission_change(const mode_t mode, const char op, const mode_t permission_mask, const int shift) {
    const mode_t shifted_mask = permission_mask >> shift;
    switch (op) {
        case '+': return mode | shifted_mask;
        case '-': return mode & ~shifted_mask;
        case '=': {
            mode_t entity_mask;
            switch (shift) {
                case 0: entity_mask = S_IRWXU; break;
                case 3: entity_mask = S_IRWXG; break;
                case 6: entity_mask = S_IRWXO; break;
                default: return mode;
            }
            return mode & ~entity_mask | shifted_mask & entity_mask;
        }
        default: return mode;
    }
}

bool change_permissions(const std::string& path, const std::string& mode) {
    struct stat file_stat{};

    if (stat(path.c_str(), &file_stat) != 0) {
        std::cerr << std::format("Error: Could not stat file {}: {}\n", path, strerror(errno));
        return false;
    }

    const mode_t original_mode = file_stat.st_mode;
    mode_t new_mode = original_mode;

    if (mode.size() == 3 && mode.find_first_not_of("01234567") == std::string::npos) {
        new_mode = parse_numeric_mode(mode);
    } else {
        const size_t op_pos = mode.find_first_of("+-=");
        if (op_pos == std::string::npos) {
            std::cerr << "Invalid symbolic mode: missing operator\n";
            return false;
        }

        std::string who_part = mode.substr(0, op_pos);
        const char op = mode[op_pos];
        const std::string perm_part = mode.substr(op_pos + 1);

        if (who_part.empty()) {
            who_part = "a";
        }

        for (const char entity : who_part) {
            std::vector<int> shifts;
            if (entity == 'u') shifts.push_back(0);
            else if (entity == 'g') shifts.push_back(3);
            else if (entity == 'o') shifts.push_back(6);
            else if (entity == 'a') { shifts = {0, 3, 6}; }
            else {
                std::cerr << "Invalid entity in symbolic mode: " << entity << '\n';
                return false;
            }

            for (const int shift : shifts) {
                for (const char perm : perm_part) {
                    const mode_t mask = permission_mask(perm);
                    if (mask == 0) {
                        std::cerr << "Invalid permission: " << perm << '\n';
                        return false;
                    }
                    new_mode = apply_permission_change(new_mode, op, mask, shift);
                }
            }
        }
    }

    if (new_mode != original_mode) {
        if (chmod(path.c_str(), new_mode) != 0) {
            std::cerr << std::format("Error: Could not change permissions of {}: {}\n", path, strerror(errno));
            return false;
        }
    }

    return true;
}

int main(const int argc, char* argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }

    const std::string mode = argv[1];

    for (const std::span files(argv + 2, argc - 2); const auto& file : files) {
        if (!change_permissions(file, mode)) {
            std::cerr << std::format("Failed to change permissions for {}\n", file);
            return 1;
        }
    }

    std::cout << "Permissions successfully changed.\n";
    return 0;
}