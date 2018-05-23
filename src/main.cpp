#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>
#include <iomanip>
#include <cstdint>
#include <time.h>

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
	else{
		//In this case, this is debug
		if(argv[3] == "true" || argv[3] == "True"){
			Chain chain(argv[1], argv[2], true);
		}
		else{ // This is an output file
			Chain chain(argv[1], argv[2], argv[3]);
		}
	}
	chain.read();

	return 0;
}
