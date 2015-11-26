#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

int main(int argc, char **argv) {
	// command line arguments checking
    if (argc < 4) {
		cerr << "usage: vmgenU page_range(max 100) length file_name" << endl;
		return 1;
    }

    // the range of page references (max 100)
    int page_range = stoi(argv[1]);
    // the length of the sequence of page references
    int seq_length = stoi(argv[2]);
    // the name of the file that will be generated
    string file_name = argv[3];

    // check if the range of page references is not larger than 100
    if (page_range > 100) {
    	cerr << "usage: vmgenU page_range(max 100) length file_name" << endl;
		return 1;
    }

    // create and open a file.
    // if the file already exits, it will be overwritten
    ofstream file(file_name);

    // check if file is opened properly
    if(file.is_open()){
    	/* initialize random seed: */
  		srand (time(NULL));

  		// generate 'seq_length' number of page references 
  		// containing random page numbers between 0 and 'page_range'
    	for(int i=0; i<seq_length; i++){
    		file << rand() % page_range << " ";
    	}
    	file << endl;
    	file.close();
    	return 0;
    } else{
    	cerr << "file " << file_name << " can not be opened properly" << endl;
		return 1;
    }
}
