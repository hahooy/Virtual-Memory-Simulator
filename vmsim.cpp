#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

typedef enum {OPT, LRU, FIFO, CLOCK} Algo;

/* function declaration */
int next_FIFO(int cur_index, int ref_index);
int next_CLOCK(int cur_index, int ref_index);
int next_OPT(int cur_index, int ref_index);
int next_LRU(int cur_index, int ref_index);
void print_frame(int i, bool page_fault);
void run();
void getRef();

/* variable declaration */
int miss = 0;
int num_frames;
string input_file;
Algo algorithm;
vector<bool> usebits;
vector<int> references;
vector<int> frames;
set<int> s;

/* function definitions */

/* get references from the input file and store them into an array called frames */
void getRef()
{
    string line;
    ifstream infile(input_file);

    while (getline(infile, line))  // this does the checking!
    {
	istringstream iss(line);
	int reference;

	while (iss >> reference)
	{	   
	    references.push_back(reference);
	}
    }

    infile.close();
}

/* simulate the memory page reference */
void run()
{
    int (* next)(int curr_index, int ref_index);
    int replace_index = -1;
    bool page_fault = false;
    string res = "";

    switch (algorithm) {
    case OPT:
	next = next_OPT;
	break;
    case LRU:
	next = next_LRU;
	break;
    case FIFO:
	next = next_FIFO;
	break;
    case CLOCK:
	next = next_CLOCK;
	break;
    }

    /* check all page references */
    for (int i = 0; i < references.size(); ++i) {
	if (s.find(references[i]) == s.end()) {
	    /* page fault */
	    s.insert(references[i]);

	    if (frames.size() < num_frames) {
		/* there are empty frames */
		replace_index++;
		frames.push_back(references[i]);
	    } else {
		replace_index = next(replace_index, i);
		page_fault = true;
		miss++;
		s.erase(frames[replace_index]);
		frames[replace_index] = references[i];
	    }
	}
	/* need to set the use bit for clock policy */
	if (algorithm == CLOCK) {
	    usebits[find(frames.begin(), frames.end(), references[i]) - frames.begin()] = true;
	}
	
	print_frame(i, page_fault);
	if (page_fault) {
	    page_fault = false;
	}
    }
}

/* print the frame to standard out */
void print_frame(int i, bool page_fault)
{
    	printf("%d: [", references[i]);
	for (int i = 0; i < num_frames; ++i) {
	    if (i < frames.size() && frames[i] < 10) {
		printf(" %d", frames[i]);
	    } else if (i < frames.size() && frames[i] > 10) {
		printf("%d", frames[i]);
	    } else {
		printf("  ");
	    }

	    if (i == num_frames - 1) {
		printf("]");
	    } else {
		printf("|");
	    }
	}
	if (page_fault) {
	    printf("  F\n");
	} else {
	    printf("\n");
	}
}

/* FIFO policy */
int next_FIFO(int cur_index, int ref_index)
{
    return (cur_index + 1) % num_frames;
}

/* Clock policy */
int next_CLOCK(int cur_index, int ref_index)
{
    cur_index = (cur_index + 1) % num_frames;
    while (usebits[cur_index]) {
	usebits[cur_index] = false;
	cur_index = (cur_index + 1) % num_frames;
    }
    return cur_index;
}

/* OPT policy */
int next_OPT(int cur_index, int ref_index)
{
    int longest_time = 0, longest_index = 0;
    
    for (int i = 0; i < frames.size(); ++i) {
	for (int j = ref_index + 1; j < references.size(); ++j) {
	    if (frames[i] == references[j]) {
		if (j > longest_time) {
		    longest_time = j; // found a page whose time to the next reference is longer, update the time and index
		    longest_index = i;
		}
		break;		    
	    }
	    if (j == references.size() - 1) {
		return i; // if no reference of this frame is found in the future, it must be the longest
	    }
	}
    }
    return longest_index;
}

/* LRU policy */
int next_LRU(int cur_index, int ref_index)
{
    int least_time = ref_index, least_index = 0;
    for (int i = 0; i < frames.size(); ++i) {
	for (int j = ref_index - 1; j >= 0; --j) {
	    if (frames[i] == references[j]) {
		if (j < least_time) {
		    least_time = j; // update time and index
		    least_index = i;
		}
		break;
	    }
	}
    }
    return least_index;
}

int main(int argc, char **argv)
{
    if (argc < 4) {
	printf("usage: vmsim frame input algorithm\n");
	return 1;
    }

    num_frames = stoi(argv[1]);
    input_file = argv[2];
    string algo = argv[3];

    if (num_frames > 100) {
	fprintf(stderr, "the maximum number of physical memory frames is 100\n");
	return 1;
    }
    
    frames.reserve(num_frames);    

    if (algo == "OPT" || algo == "opt") {
	algorithm = OPT;
    } else if (algo == "LRU" || algo == "lru") {
	algorithm = LRU;
    } else if (algo == "FIFO" || algo == "fifo") {
	algorithm = FIFO;
    } else if (algo == "CLOCK" || algo == "clock") {
	algorithm = CLOCK;
	usebits = vector<bool>(num_frames, false);
    } else {
	fprintf(stderr, "invalid algorithm\n");
	return 1;
    }

    getRef();
    run();

    printf("Miss rate = %d/%lu = %.2f%%\n", miss, references.size(), (double) miss / references.size() * 100);
}
