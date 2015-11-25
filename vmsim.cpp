#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

typedef enum {OPT, LRU, FIFO, CLOCK} Algo;

/* function declaration */
int next_FIFO(int cur_index);
void pframe(int i, bool page_fault);

/* variable declaration */
int num_frames;
string input_file;
Algo algorithm;
vector<int> references;
vector<int> frames;

/* function definition */
void getRef() {
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
}

void run() {
    int (* next)(int cur_index);
    int next_index = 0;
    bool page_fault = false;
    string res = "";

    switch (algorithm) {
    case OPT:
	/* TODO */
	printf("!");
	break;
    case LRU:
	/* TODO */
	break;
    case FIFO:
	next = next_FIFO;
	break;
    case CLOCK:
	/* TODO */
	break;
    }

    for (int i = 0; i < references.size(); ++i) {
	if (frames.size() < num_frames) {
	    /* there are empty frames */
	    frames.push_back(references[i]);
	} else if (find(frames.begin(), frames.end(), references[i]) == frames.end()) {
	    page_fault = true;
	    frames[next_index] = references[i];
	    next_index = next(next_index);
	}
	pframe(i, page_fault);
    }
}

void pframe(int i, bool page_fault) {
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
	    page_fault = false;
	    printf(" F\n");
	} else {
	    printf("\n");
	}
}

int next_FIFO(int cur_index) {
    return (cur_index + 1) % num_frames;
}


int main(int argc, char **argv) {
    if (argc < 4) {
	printf("usage: vmsim frame input algorithm\n");
	return 1;
    }
    num_frames = stoi(argv[1]);
    input_file = argv[2];
    string algo = argv[3];
    frames.reserve(num_frames);
    

    if (algo == "OPT" || algo == "opt") {
	algorithm = OPT;
    } else if (algo == "LRU" || algo == "lru") {
	algorithm = LRU;
    } else if (algo == "FIFO" || algo == "fifo") {
	algorithm = FIFO;
    } else if (algo == "CLOCK" || algo == "clock") {
	algorithm = CLOCK;
    } else {
	fprintf(stderr, "invalid algorithm\n");
	return 1;
    }

    getRef();
    run();
}
