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

/**
*@brief reads the varInt, following https://en.bitcoin.it/wiki/Protocol_documentation#Variable_length_integer
*@param f an input stream, where to read
*@return a 64 bit int containing the int to read (even if it was a 8, 16 or 32 bit
*/
uint64_t Chain::readVarInt(std::ifstream &f){
	uint8_t tmp;
	uint64_t buff, finalInt;
	f.read((char*)&tmp, 1); //The byte which give information about the length of int
	if(tmp < 0xfd){
		finalInt = tmp;
	}
	else if(tmp == 0xfd){
		f.read((char*)&buff, 2);
		finalInt = buff;
	}
	else if(tmp == 0xfe){
		f.read((char*)&buff, 4);
		finalInt = buff;
	}
	else if(tmp == 0xff){
		f.read((char*)&buff, 8);
		finalInt = buff;
	}
	return finalInt;
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
				_curPos = f.tellg(); // To compare length of block with length of remaining data, see below
				f.read((char*)& _blocks[curBlock].header.version, 4);
				f.read((char*)& _blocks[curBlock].header.prevHash, 32);
				f.read((char*)& _blocks[curBlock].header.merkleRoot, 32);
				f.read((char*)& _blocks[curBlock].header.timestamp, 4);
				f.read((char*)& _blocks[curBlock].header.target, 4);
				f.read((char*)& _blocks[curBlock].header.nonce, 4);
				_blocks[curBlock].header.varInt = readVarInt(f);
				
				//We have here the header, so we have the blockLength value, so we know how many bytes are following up until
				//the end of the block (from the reading of blockLength) (cf https://en.bitcoin.it/wiki/Block#Block_structure)
				int headerEndPos = f.tellg();
				f.seekg(f.end);
				length = f.tellg() - _curPos;
				if(length > _blocks[curBlock].header.blockLength){
					//Okay, we can go on
					f.seekg(headerEndPos);
					
				}
				else{
				
				}
			}
			else{
				//We need to take what we can..
			}
		}
		curBlock++;
	}
}
