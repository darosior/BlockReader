#include "Chain.h"

Chain::Chain(std::string dataDir, int nbToRead, std::string output){
	if(dataDir.length() > 0)
		_dataDir = dataDir;
	else
		_dataDir = "./data"; // Default
	
	_nbToRead = nbToRead;
	
	if(output.length() > 0)
		_outFile = output;
	else
		_outFile = "out.txt";
		
	_blocks = new Block[_nbToRead];
}
