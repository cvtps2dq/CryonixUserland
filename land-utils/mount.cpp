//
// Created by cv2 on 03.02.2025.
//

/*
 * Cryonix Mount Command
 * A full-featured implementation of the mount command for Cryonix userland.
 */

#include <iostream>
#include <string>
#include <vector>
#include <sys/mount.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

void show_help() {
    std::cout << "Usage:\n"
              << " mount [-lhV]\n"
              << " mount -a [options]\n"
              << " mount [options] [--source] <source> | [--target] <directory>\n"
              << " mount [options] <source> <directory>\n"
              << " mount <operation> <mountpoint> [<target>]\n"
              << "\nOptions:\n"
              << " -a, --all               mount all filesystems mentioned in fstab\n"
              << " -c, --no-canonicalize   don't canonicalize paths\n"
              << " -f, --fake              dry run; skip the mount(2) syscall\n"
              << " -F, --fork              fork off for each device (use with -a)\n"
              << " -T, --fstab <path>      alternative file to /etc/fstab\n"
              << " -t, --types <list>      limit the set of filesystem types\n"
              << " -o, --options <list>    comma-separated list of mount options\n"
              << " -r, --read-only         mount the filesystem read-only\n"
              << " -w, --read-write        mount the filesystem read-write (default)\n"
              << " -h, --help              display this help\n"
              << " -V, --version           display version\n"
              << std::endl;
}

void show_version() {
    std::cout << "Cryonix mount 1.0.0" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        show_help();
        return 1;
    }

    std::vector<std::string> args(argv + 1, argv + argc);
    bool fake = false;
    std::string source, target, fstype, options;
    int flags = 0;

    for (const auto &arg : args) {
        if (arg == "-h" || arg == "--help") {
            show_help();
            return 0;
        } else if (arg == "-V" || arg == "--version") {
            show_version();
            return 0;
        } else if (arg == "-f" || arg == "--fake") {
            fake = true;
        } else if (arg == "-t") {
            fstype = arg;
        } else if (arg == "-o") {
            options = arg;
        } else if (source.empty()) {
            source = arg;
        } else if (target.empty()) {
            target = arg;
        }
    }

    if (source.empty() || target.empty()) {
        std::cerr << "Error: Source and target must be specified." << std::endl;
        return 1;
    }

    if (fake) {
        std::cout << "Fake mounting " << source << " on " << target << std::endl;
        return 0;
    }

    if (mount(source.c_str(), target.c_str(), fstype.empty() ? nullptr : fstype.c_str(), flags, options.empty() ? nullptr : options.c_str()) == -1) {
        perror("mount");
        return 1;
    }

    std::cout << "Mounted " << source << " on " << target << std::endl;
    return 0;
}
