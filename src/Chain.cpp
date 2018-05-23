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

Chain::Chain(std::string dataDir, int nbToRead, bool debug){
	if(dataDir.length() > 0)
		_dataDir = dataDir+"/";
	else
		_dataDir = "./data/"; // Default
	
	_nbToRead = nbToRead;
		
	_blocks = new Block[_nbToRead];
	_filename = _dataDir+"blk00000.dat"; // First file
	_curPos = 0;
	_curFile = 0;
}

void Chain::set_fileName(){
	if(_curFile < 10)
		_filename = _dataDir+"blk0000"+std::to_string(_curFile)+".dat";
	else if(_curFile < 100)
		_filename = _dataDir+"blk000"+std::to_string(_curFile)+".dat";
	else if(_curFile < 1000)
		_filename = _dataDir+"blk00"+std::to_string(_curFile)+".dat";
	else if(_curFile < 10000)
		_filename = _dataDir+"blk0"+std::to_string(_curFile)+".dat";
	else if(_curFile < 100000)
		_filename = _dataDir+"blk"+std::to_string(_curFile)+".dat";
}

void Chain::read(){
	int curBlock = 0;
	while(curBlock < _nbToRead){
		std::ifstream f(_filename, std::ios::in | std::ios::binary | std::ios::ate);
		if(f.is_open()){
			//To get the size of the remaining part of the file
			int length = f.tellg() - _curPos;
			// Header max length = 96 bytes, cf doc (for the doc cf main.cpp or Readme.md)
			if(length > 96){
				//We can go, we set the cursor to the right position and we begin to read
				f.seekg(_curPos);
				f.read((char*)& _blocks[curBlock].header.magicBytes, 4);
				f.read((char*)& _blocks[curBlock].header.blockLength, 4);
				f.read((char*)& _blocks[curBlock].header.version, 4);
				f.read((char*)& _blocks[curBlock].header.prevHash, 32);
				f.read((char*)& _blocks[curBlock].header.merkleRoot, 32);
				f.read((char*)& _blocks[curBlock].header.timestamp, 4);
				f.read((char*)& _blocks[curBlock].header.target, 4);
				f.read((char*)& _blocks[curBlock].header.nonce, 4);
			}
			else{
				//We need to take what we can..
			}
		}
		curBlock++;
	}
}
