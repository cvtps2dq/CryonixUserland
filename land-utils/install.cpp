//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <vector>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

namespace fs = std::filesystem;

void print_usage() {
    std::cout << "Usage: install [OPTIONS] SOURCE... DEST\n"
              << "Copy files and set attributes.\n\n"
              << "Options:\n"
              << "  -m MODE   Set file mode (permissions)\n"
              << "  -d        Create directories instead of copying files\n"
              << "  -o OWNER  Set ownership to OWNER\n"
              << "  -g GROUP  Set group to GROUP\n"
              << "  -v        Verbose mode (print actions)\n";
}

void copy_file(const fs::path& source, const fs::path& dest, mode_t mode, bool verbose) {
    try {
        fs::copy(source, dest, fs::copy_options::overwrite_existing);
        chmod(dest.c_str(), mode);
        if (verbose) {
            std::cout << "Installed " << source << " -> " << dest << " (mode: " << std::oct << mode << ")\n";
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error copying " << source << ": " << e.what() << std::endl;
        exit(1);
    }
}

void create_directory(const fs::path& dir, mode_t mode, bool verbose) {
    try {
        fs::create_directories(dir);
        chmod(dir.c_str(), mode);
        if (verbose) {
            std::cout << "Created directory: " << dir << " (mode: " << std::oct << mode << ")\n";
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error creating directory " << dir << ": " << e.what() << std::endl;
        exit(1);
    }
}

void change_owner(const fs::path& file, const std::string& owner, const std::string& group) {
    struct passwd* pwd = owner.empty() ? nullptr : getpwnam(owner.c_str());
    struct group* grp = group.empty() ? nullptr : getgrnam(group.c_str());

    if (!owner.empty() && !pwd) {
        std::cerr << "Error: Invalid user " << owner << std::endl;
        exit(1);
    }
    if (!group.empty() && !grp) {
        std::cerr << "Error: Invalid group " << group << std::endl;
        exit(1);
    }

    if (chown(file.c_str(), pwd ? pwd->pw_uid : -1, grp ? grp->gr_gid : -1) != 0) {
        std::cerr << "Error: Failed to change ownership of " << file << std::endl;
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }

    std::vector<std::string> sources;
    std::string dest;
    mode_t mode = 0755;  // Default mode
    std::string owner, group;
    bool make_dirs = false, verbose = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-m" && i + 1 < argc) {
            mode = std::stoi(argv[++i], nullptr, 8);
        } else if (arg == "-d") {
            make_dirs = true;
        } else if (arg == "-o" && i + 1 < argc) {
            owner = argv[++i];
        } else if (arg == "-g" && i + 1 < argc) {
            group = argv[++i];
        } else if (arg == "-v") {
            verbose = true;
        } else if (arg[0] != '-') {
            sources.push_back(arg);
        } else {
            print_usage();
            return 1;
        }
    }

    if (sources.empty()) {
        std::cerr << "Error: No source files provided.\n";
        return 1;
    }

    dest = sources.back();
    sources.pop_back();

    fs::path dest_path = dest;

    if (make_dirs) {
        create_directory(dest_path, mode, verbose);
        return 0;
    }

    if (sources.size() > 1 || fs::is_directory(dest_path)) {
        for (const auto& src : sources) {
            fs::path target = dest_path / fs::path(src).filename();
            copy_file(src, target, mode, verbose);
            if (!owner.empty() || !group.empty()) {
                change_owner(target, owner, group);
            }
        }
    } else {
        copy_file(sources[0], dest_path, mode, verbose);
        if (!owner.empty() || !group.empty()) {
            change_owner(dest_path, owner, group);
        }
    }

    return 0;
}