#IFNDEF CHAIN
#DEFINE CHAIN

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>

class Chain{
	public:
	
		Chain(std::string dataDir, int nbToRead, std::string output);
		Chain(std::string dataDir, int nbToRead, bool debug); // Print results to console
	
	private:
	
		std::string _dataDir; // Directory from which to load the blocks
		int _nbToRead; // Number of blocks to read
		std::string _outFile; // File to write
};

#ENDIF
