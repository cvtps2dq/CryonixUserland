#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <ctime>
#include <cstring>

#define RESET "\033[0m"
#define GREEN "\033[32m"

using namespace std;

void print_file_info(const char* file_path) {
    struct stat file_stat;

    if (stat(file_path, &file_stat) != 0) {
        perror("Ошибка при получении информации о файле");
        return;
    }

    // Права доступа
    std::cout << std::oct << (file_stat.st_mode & 0777) << std::dec << " ";

    // Владелец
    struct passwd *pw = getpwuid(file_stat.st_uid);
    struct group  *gr = getgrgid(file_stat.st_gid);
    std::cout  << (pw ? pw->pw_name : "Неизвестно") << " ";
    std::cout << (gr ? gr->gr_name : "Неизвестно") << " ";

    // Дата последнего изменения
    std::cout <<  ctime(&file_stat.st_mtime);
    // Размер файла
    std::cout << file_stat.st_size << " B\n";
}

void list_files_in_directory(const char* dir_path) {
    DIR* dir = opendir(dir_path);
    if (dir == nullptr) {
        perror("Ошибка при открытии директории");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        // Игнорируем специальные записи "." и ".."
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            std::string file_path = std::string(dir_path) + "/" + entry->d_name;
            std::cout << "File: " << entry->d_name << "\n";
            print_file_info(file_path.c_str());
            std::cout << "---------------------------\n";
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[]) {

	if (argc > 2) {
		cout << "Use only one dir pls" << endl;
	} else if (argc == 2) {
		for (short i = 1; i != argc; i++) {
			list_files_in_directory(argv[1]);
		}
		cout << endl;
	} else {
		list_files_in_directory(".");
		cout << endl;
	}

    return 0;
}
