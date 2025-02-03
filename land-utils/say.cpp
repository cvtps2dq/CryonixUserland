#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {

	try {
		cout << argv[1] << endl;
	} catch (...) {
		cout << "SOMETHING WRONG" << endl;
	}

}
