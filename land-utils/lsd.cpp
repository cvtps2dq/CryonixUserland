#include "lsd.h"

// BITCH!!! AM ON LSD!!!!!!!

#include <iostream>
#include <vector>

#define RESET "\033[0m"
#define GREEN "\033[32m"

using namespace std;

int main(int argc, char* argv[]) {

	vector <char*>dir_vec;

	for (short i = 1; i != argc; i++) {
		string comand_name = argv[i];
		if (comand_name[0] != '-')
			dir_vec.push_back(argv[i]);
	}

	if (argc > 1) {
		for (short i = 1; i != argc; i++) {
			is_flag(argv[i]);
		}
	}

	if (dir_vec.size() >= 2) {
		for (const auto& dir : dir_vec) {
			cout << "\t" << dir << ":\n";
			cout << GREEN << "--------------------------------" << RESET << endl;
			list_dir(dir);
			cout << GREEN << "--------------------------------" << RESET << endl << endl;
		}
	} else if (dir_vec.size() == 1) {
		for (short i = 1; i != argc; i++) {
			list_dir(argv[i]);
		}
		cout << endl;
	} else {
		list_dir(".");
		cout << endl;
	}

	return 0;
}
