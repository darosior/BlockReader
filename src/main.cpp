#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <bitset>
#include <iomanip>
#include <cstdint>
#include <time.h>
#include "Chain.h"

using namespace std;

/*
	Doc used : https://en.bitcoin.it/wiki/Block and http://codesuppository.blogspot.fr/2014/01/how-to-parse-bitcoin-blockchain.html
	Cf Readme for further details
*/

int main(int argc, char * argv[]){
	if(!(argc == 4) && !(argc == 5)){
		cout<<"usage : "<<argv[0]<<" <blockDir> [<output file> / <debug>] <nb of blocks to read> (<starting block>)"<<endl;
		cout<<"Examples : "<<argv[0]<<" /path/to/data debug 10"<<endl;
		cout<<"           "<<argv[0]<<" /path/to/data test.txt 34"<<endl;
		cout<<"           "<<argv[0]<<" /path/to/data test.txt 100 1200"<<endl;
		return 0;
	}
	
	// We create an instance of the chain reader with the correct folder and number of blocks to read
	Chain chain(argv[1], atoi(argv[3]));
	// We read them
	chain.read();
	// And write / display them
	if(strcmp(argv[3], "debug") == 0 || strcmp(argv[3], "Debug") == 0){ //In this case, this is debug
		chain.debug();
	}
	else{ // This is an output file
		if(argc == 5)
			chain.write(argv[2], atoi(argv[4]));
		else
			chain.write(argv[2]);
	}

	return 0;
}
