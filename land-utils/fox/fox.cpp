#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {

    string line;
 
    ifstream in(argv[1]); // окрываем файл для чтения
	
	if (argc < 2){
	
		cout << "Usage:" << endl << "\tfox {file}" << endl;
		return 0;
	
	} else {
		
		if (in.is_open()){

			while (std::getline(in, line)) {

				cout << line << endl;
			}
		} else {
			cout << "Can't open " << argv[1] << endl;
		}

		in.close();     // закрываем файл

	}
    

	return 0;
}
