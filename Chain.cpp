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
	_filename = "blk00000.dat"; // First file
	_curPos = 0;
	_curFile = 0;
}

void set_fileName(){
	if(_curFile < 10)
		_filename = "blk0000"+to_string(_curFile)+".dat";
	else if(_curFile < 100)
		_filename = "blk000"+to_string(_curFile)+".dat";
	else if(_curFile < 1000)
		_filename = "blk00"+to_string(_curFile)+".dat";
	else if(_curFile < 10000)
		_filename = "blk0"+to_string(_curFile)+".dat";
	else if(_curFile < 100000)
		_filename = "blk"+to_string(_curFile)+".dat";
}

void Chain::read(){
	int curBlock = 0;
	while(curBlock < _nbToRead){
		ifstream f(filename, ios::in | ios::binary);
		if(f.isopen()){
			//We set the cursor to the position we were
			f.seekg(curPos);
			//And determine if we got the space to read the header of the block till the end of the file
			f.seekg(f.end());
			int length = f.tellg() - curPos;
			// Header max length = 96 bytes, cf doc (for the doc cf main.cpp or Readme.md)
			if(length > 96){
				//We can go, we set the cursor to the right position and we begin to read
				f.seekg(curPos);
			}
			else{
				//We need to take what we can..
			}
		}
	}
}
