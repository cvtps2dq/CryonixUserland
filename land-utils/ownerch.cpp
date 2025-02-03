//
// Created by cv2 on 02.02.2025.
//

#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <cstdlib>
#include <fcntl.h>
#include <sys/stat.h>

//please build with linux because macOS doesn't have the AL_SYMLINK_FOLLOW parameter.

void print_usage() {
    std::cout << "Usage: ownerch [OPTION] USER[:GROUP] FILE\n"
              << "Change the owner and/or group of each given FILE.\n"
              << "-h: Treat symbolic links like regular files.\n";
}

uid_t get_uid(const std::string& username) {
    if (const passwd* pw = getpwnam(username.c_str())) {
        return pw->pw_uid;
    }
    std::cerr << "Error: User '" << username << "' not found.\n";
    exit(1);
}

gid_t get_gid(const std::string& groupname) {
    if (const group* gr = getgrnam(groupname.c_str())) {
        return gr->gr_gid;
    }
    std::cerr << "Error: Group '" << groupname << "' not found.\n";
    exit(1);
}

void chown_clone(const std::string& user_group, const std::string& file, bool follow_symlinks) {
    const size_t colon_pos = user_group.find(':');
    std::string username;
    std::string groupname;

    if (colon_pos != std::string::npos) {
        username = user_group.substr(0, colon_pos);
        groupname = user_group.substr(colon_pos + 1);
    } else {
        username = user_group;
        groupname = "";
    }

    // Get user UID
    const uid_t uid = get_uid(username);

    // Get group GID, if any
    const gid_t gid = groupname.empty() ? -1 : get_gid(groupname);

    // chown() options:
    int flags = follow_symlinks ? 0 : AT_SYMLINK_NOFOLLOW;

    // Call chown system call to change file ownership
    if (chown(file.c_str(), uid, gid) == -1) {
        std::cerr << "Error: Unable to change ownership of file '" << file << "'.\n";
        perror("chown error");
        exit(1);
    }

    std::cout << "Ownership of '" << file << "' changed to " << username;
    if (!groupname.empty()) {
        std::cout << ":" << groupname;
    }
    std::cout << std::endl;
}

int main(const int argc, char* argv[]) {
    bool follow_symlinks = true;

    if (argc < 3) {
        print_usage();
        return 1;
    }

    const std::string user_group = argv[1];
    const std::string file = argv[2];

    // Check if there is a -h flag to not follow symlinks
    if (argc == 4 && std::string(argv[3]) == "-h") {
        follow_symlinks = false;
    }

    chown_clone(user_group, file, follow_symlinks);
    return 0;
}