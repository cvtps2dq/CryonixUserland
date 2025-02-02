#pragma once

#include <iostream>
#include <string>
#include <filesystem>

// Colors
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define MAGNETA "\033[35m"
#define CYAN "\033[36m"
#define B_BLUE "\033[94m"

using namespace std;

using filesystem::directory_iterator;

bool show_all = false;

// Функция для получения типа файла
std::string get_file_type(const filesystem::path& file_path) {

	// Директория
	if (is_directory(file_path)) {
        return B_BLUE;
    }

    // Получаем расширение файла
    std::string extension = file_path.extension().string();
    
    // Определяем тип файла по расширению
    if (extension == ".jpg" || extension == ".jpeg" || extension == ".png" || extension == ".gif" || extension == ".bmp" || extension == ".webp") {
        return MAGNETA;
    } else if (extension == ".mp3" || extension == ".wav" || extension == ".aac" || extension == ".flac") {
        return CYAN;
    } else if (extension == ".mp4" || extension == ".mkv" || extension == ".avi" || extension == ".mov" || extension == ".mpv" || extension == ".webm") {
        return MAGNETA;
	} else {
		// Проверка прав на исполнение
		auto perms = filesystem::status(file_path).permissions();
		if ((perms & filesystem::perms::owner_exec) != filesystem::perms::none || 
			(perms & filesystem::perms::group_exec) != filesystem::perms::none || 
			(perms & filesystem::perms::others_exec) != filesystem::perms::none) {
			return GREEN;
		}
    }
	
	return ""; 
}


void list_dir(string path) {
	if (path[0] != '-') {
		try {
			for (const auto& file : directory_iterator(path)) {
				string file_name = file.path().filename().string();
				if (show_all == false) {
					if (file_name[0] != '.') {
						string file_type = get_file_type(file.path());
						std::cout << file_type << file_name << RESET << "\n";
					}
				} else {
					string file_type = get_file_type(file.path());
					std::cout << file_type << file_name << RESET << "\n";
				}
			}
		} catch (...) { 
			string file_type = get_file_type(path);
			std::cout << file_type << path << RESET << " ";
		}
	}
}

void is_flag(string flag) {
	if (flag[0] == '-') {

		if (flag == "-a") {
			show_all = true;
			//list_dir(".");
		}

		if (flag == "-take") {
			cout << "Bro... why you took that LSD? :(" << endl;
		}

	}
}
