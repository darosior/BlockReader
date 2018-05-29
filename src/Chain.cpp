#include "Chain.h"

Chain::Chain(std::string dataDir, int nbToRead){
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
	while(curBlock < _nbToRead - 1){ // -1 -> first block is 0
		std::ifstream f(_filename, std::ios::in | std::ios::binary | std::ios::ate);
		while(f.is_open()){
			//To get the size of the remaining part of the file
			unsigned int length = f.tellg() - _curPos;
			// Header max length = 96 bytes, cf doc (for the doc cf main.cpp or Readme.md)
			if(length > 96){
				//We can go, we set the cursor to the right position and we begin to read
				f.seekg(_curPos);
				
				_blocks[curBlock].header.prevHash = new uint8_t[32];
				_blocks[curBlock].header.merkleRoot = new uint8_t[32];
				
				f.read((char*)& _blocks[curBlock].header.magicBytes, 4);
				std::cout<<f.tellg()<<std::endl;
				f.read((char*)& _blocks[curBlock].header.blockLength, 4);
				std::cout<<f.tellg()<<std::endl;
				_curPos = f.tellg(); // To compare length of block with length of remaining data, see below
				f.read((char*)& _blocks[curBlock].header.version, 4);
				std::cout<<f.tellg()<<std::endl;
				f.read((char*) _blocks[curBlock].header.prevHash, 32);
				std::cout<<f.tellg()<<std::endl;
				f.read((char*) _blocks[curBlock].header.merkleRoot, 32);
				std::cout<<f.tellg()<<std::endl;
				f.read((char*)& _blocks[curBlock].header.timestamp, 4);
				std::cout<<f.tellg()<<std::endl;
				f.read((char*)& _blocks[curBlock].header.target, 4);
				f.read((char*)& _blocks[curBlock].header.nonce, 4);
				_blocks[curBlock].header.txCount = readVarInt(f);
				
				// We have here the header, so we have the blockLength value, so we know how many bytes are following up until
				// the end of the block (from the reading of blockLength) (cf https://en.bitcoin.it/wiki/Block#Block_structure)
				int headerEndPos = f.tellg();
				f.seekg(f.end);
				length = f.tellg() - _curPos;
				if(length > _blocks[curBlock].header.blockLength){
					// Okay, we can go on
					f.seekg(headerEndPos);
					// We are now going to read the transactions. To do so we will initialize the tab containing them
					_blocks[curBlock].transactions = new Transaction[_blocks[curBlock].header.txCount];
					// And now for each transaction
					for(unsigned int i = 0; i<_blocks[curBlock].header.txCount; i++){
						f.read((char*)& _blocks[curBlock].transactions[i].version, 4);
						_blocks[curBlock].transactions[i].inputCount = readVarInt(f);
						_blocks[curBlock].transactions[i].inputs = new Input[_blocks[curBlock].transactions[i].inputCount];
						// For each input
						for(unsigned int j = 0; j<_blocks[curBlock].transactions[i].inputCount; j++){
							f.read((char*)& _blocks[curBlock].transactions[i].inputs[j].hash, 32);
							f.read((char*)& _blocks[curBlock].transactions[i].inputs[j].index, 4);
							_blocks[curBlock].transactions[i].inputs[j].scriptLength = readVarInt(f);
							f.read((char*)& _blocks[curBlock].transactions[i].inputs[j].script, _blocks[curBlock].transactions[i].inputs[j].scriptLength);
							f.read((char*)& _blocks[curBlock].transactions[i].inputs[j].sequence, 4);
						}
						// For each output
						_blocks[curBlock].transactions[i].outputCount = readVarInt(f);
						_blocks[curBlock].transactions[i].outputs = new Output[_blocks[curBlock].transactions[i].outputCount];
						for(unsigned int j = 0; j<_blocks[curBlock].transactions[i].inputCount; j++){
							f.read((char*)& _blocks[curBlock].transactions[i].outputs[j].value, 8);
							_blocks[curBlock].transactions[i].outputs[j].scriptLength = readVarInt(f);
							f.read((char*)& _blocks[curBlock].transactions[i].outputs[j].script, _blocks[curBlock].transactions[i].outputs[j].scriptLength);
						}
						// And finally the lock time
						f.read((char*)& _blocks[curBlock].transactions[i].lockTime, 4);
					}
				}
				else{
				
				}
				
				delete[] _blocks[curBlock].header.prevHash;
				delete[] _blocks[curBlock].header.merkleRoot;
			}
			else{
				//We need to take what we can..
			}
			break;
		}
		curBlock++;
	}
}

void Chain::displayBytes(uint8_t bytes[], uint32_t size, bool be /*= false*/){
	if(!be){
		for(uint8_t i = 0; i < size; i++){
			std::cout<<std::hex<<(int)bytes[i];
		}
	}
	//If big endian
	else{
		for(int8_t i = size; i >= 0; i--){
			std::cout<<std::hex<<(int)bytes[i];
		}
	}
}

void Chain::write(std::string output){
	
}


void Chain::debug(){
	for(int i = 0; i < _nbToRead; i++){
		std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
		std::cout<<"*******************************************************************************"<<std::endl;
		std::cout<<"                                  ----------"<<std::endl;
		std::cout<<"                                || BLOCK #"<<i<<" ||"<<std::endl;
		std::cout<<"                                  ----------"<<std::endl;
		std::cout<<std::endl;
		std::cout<<"-----------------------------------METADATAs-----------------------------------"<<std::endl;
		std::cout<<std::hex<<"Magic byte : "<<_blocks[i].header.magicBytes<<std::endl;
		std::cout<<std::hex<<"Nb of bytes : "<<_blocks[i].header.blockLength<<std::endl;
		std::cout<<std::endl<<"-------------------------------------------------------------------------------"<<std::endl;
		std::cout<<std::endl;
		
		std::cout<<std::endl<<"-------------------------------------HEADER------------------------------------"<<std::endl;
		std::cout<<std::hex<<"Version number : "<<_blocks[i].header.version<<std::endl;
		std::cout<<std::hex<<"Previous block hash : ";
		displayBytes(_blocks[i].header.prevHash, 32);
		std::cout<<std::endl;
		std::cout<<std::hex<<"Merkle root : ";
		displayBytes(_blocks[i].header.merkleRoot, 32, true);
		std::cout<<std::endl;
		time_t t = 0;
		t += _blocks[i].header.timestamp;
		std::cout<<std::dec<<"Timestamp : "<<asctime(localtime(&t)); //No need to std::endl after asctime
		std::cout<<std::hex<<"Target : "<<_blocks[i].header.target<<std::endl;
		std::cout<<std::dec<<"Nonce : "<<_blocks[i].header.nonce<<std::endl;
		std::cout<<std::dec<<"Number of transactions : "<<_blocks[i].header.txCount<<std::endl;
		std::cout<<std::endl<<"-------------------------------------------------------------------------------"<<std::endl;
		std::cout<<std::endl;
	}
}

