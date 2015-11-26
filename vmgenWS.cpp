/** Usage: vmgenWS ws_size LB UB page_range(max 100) length file_name
 *
 *	ws_size: the size of the working set
 *	LB: the lower bound on the number of pages to generate in a given 
 *		working set exhibiting locality of reference
 *	UB: the upper bound on the number of pages to generate in a given 
  *		working set exhibiting locality of reference
 *	page_range:	the range of page references (maximum 100)
 *	length:	the length of the sequence of page references
 *	file_name: the name of the file that will be generated
 */

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>

using namespace std;

int main(int argc, char **argv) {
	// command line arguments checking
    if (argc < 7) {
		cerr << "Usage: vmgenWS ws_size LB UB page_range(max 100) length file_name" << endl;
		return 1;
    }
    // working set size
    int ws_size = stoi(argv[1]);
    // lower bound on the number of pages to generate
    int lb = stoi(argv[2]);
    // upper bound on the number of pages to generate
    int ub = stoi(argv[3]);
    // the range of page references (max 100)
    int page_range = stoi(argv[4]);
    // the length of the sequence of page references
    int seq_length = stoi(argv[5]);
    // the name of the file that will be generated
    string file_name = argv[6];
    // randomly generated between LB and UB 
    int num_gen = 0;
    // working set array
    vector<int> ws_arr;

    // check if the range of page references is not larger than 100
    if (page_range > 100) {
    	cerr << "Usage: vmgenWS ws_size LB UB page_range(max 100) length file_name" << endl;
		return 1;
    }
    // check if ub > lb
    if (ub - lb <= 0){
    	cerr << "UB must be greater than LB" << endl;
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
    	for(int i=0; i<seq_length; ){
    		// generate num_gen – randomly generate a number between (LB) and (UB)
    		num_gen = rand() % (ub-lb) + lb;
    		// generate (ws_size) page reference numbers in range 0 to (page_range)
    		for(int j=0; j<ws_size; j++){
    			ws_arr.push_back(rand() % page_range);
    		}
    		// make sure no extra page references are generated
    		if((i+num_gen) > seq_length){
    			num_gen = seq_length - i;
    		}
    		// randomly generate (num_gen) of page references from the set of page
			// references (ws_arr)
    		for(int k=0; k<num_gen; k++){
    			file << ws_arr[rand() % ws_size] << " ";
    		}
    		// update i and empty ws_arr
    		i += num_gen;
    		ws_arr.clear();
    	}
    	file << endl;
    	file.close();
    	return 0;
    } else{
    	cerr << "file " << file_name << " can not be opened properly" << endl;
		return 1;
    }
}
