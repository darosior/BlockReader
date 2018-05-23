#IFNDEF CHAIN
#DEFINE CHAIN

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include "Block.h"

class Chain{
	public:
	
		Chain(std::string dataDir, int nbToRead, std::string output);
		Chain(std::string dataDir, int nbToRead, bool debug); // Print results to console
		void read();
	
	private:
	
		std::string _dataDir; // Directory from which to load the blocks
		int _nbToRead; // Number of blocks to read
		std::string _outFile; // File to write
		Block * _blocks;
		std::string _filename; // Current file to read
		int _curPos; // Position of the cursor in the file. Max file size = 2.8MB = 280 000 000 bytes, should fit in an int
		int _curFile; // Current blk file nb
		
		void set_fileName(); // If end of blkxxxn.dat, sets filename to blkxxxx(n+1).dat
};

#ENDIF
