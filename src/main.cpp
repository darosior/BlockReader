#include <iostream>
#include <stdlib.h>
#include <string>
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
	if(!(argc == 4)){
		cout<<"usage : ./"<<argv[0]<<" <blockDir> <nb of blocks to read> [<output file> / <debug=false>]"<<endl;
		cout<<"Examples : "<<argv[0]<<" /path/to/data 10 true"<<endl;
		cout<<"Examples : "<<argv[0]<<" /path/to/data 10 test.txt"<<endl;
		return 0;
	}
	
	// We create an instance of the chain reader with the correct folder and number of blocks to read
	Chain chain(argv[1], atoi(argv[2]));
	// We read them
	chain.read();
	// And write / display them
	if(argv[3] == "true" || argv[3] == "True"){ //In this case, this is debug
		chain.debug();
	}
	else{ // This is an output file
		chain.write(argv[3]);
	}

	return 0;
}
