#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int range;
int length;
string filename;

/* generate reference number randomly */
void vmgen()
{
    srand (time(NULL)); // seed the rand() function
    ofstream out_file(filename); // open a stream
    
    for (int i = 0; i < length; ++i) {
	int value = rand() % range;
	out_file << value << " ";
    }

    out_file.close();
}

int main(int argc, char** argv)
{
    if (argc < 4) {
	printf("usage: vmgenU range length file\n");
	return 1;
    }

    try {
	range = stoi(argv[1]);
	length = stoi(argv[2]);
	filename = argv[3];
    } catch(exception &e) {
	cout << "Invalid argument. Exception: " << e.what() << endl;
	return 1;
    }

    if (range > 100) {
	cout << "the range of page references exceeds its maximum (100)" << endl;
	return 1;
    }

    vmgen();
}
