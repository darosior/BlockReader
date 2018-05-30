#include "Chain.h"

Chain::Chain(std::string dataDir, int nbToRead){
	if(dataDir.length() > 0)
		_dataDir = dataDir+"/";
	else
		_dataDir = "./data/"; // Default
	
	_nbToRead = nbToRead;
	_nbRead = 0;
	
	_blocks = new Block[_nbToRead];
	_filename = _dataDir+"blk00000.dat"; // First file
	_curPos = 0;
	_curFile = 0;
}

Chain::~Chain(){
	//To delete the memory allocated
	for(int i = 0; i < _nbToRead; i++){
		for(int j = 0; j < _blocks[i].header.txCount; j++){
			for(int k = 0; k < _blocks[i].transactions[j].inputCount; k++){
				delete[] _blocks[i].transactions[j].inputs[k].hash;
				delete[] _blocks[i].transactions[j].inputs[k].script;
			}
			for(int k = 0; k < _blocks[i].transactions[j].outputCount; k++){
				delete[] _blocks[i].transactions[j].outputs[k].script;
			}
			delete[] _blocks[i].transactions[j].inputs;
			delete[] _blocks[i].transactions[j].outputs;
		}
		delete[] _blocks[i].header.prevHash;
		delete[] _blocks[i].header.merkleRoot;
	}
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
*@brief reads a varInt, following https://en.bitcoin.it/wiki/Protocol_documentation#Variable_length_integer
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
							_blocks[curBlock].transactions[i].inputs[j].hash = new uint8_t[32];
							f.read((char*) _blocks[curBlock].transactions[i].inputs[j].hash, 32);
							
							f.read((char*)& _blocks[curBlock].transactions[i].inputs[j].index, 4);
							
							_blocks[curBlock].transactions[i].inputs[j].scriptLength = readVarInt(f);
							_blocks[curBlock].transactions[i].inputs[j].script = new uint8_t[_blocks[curBlock].transactions[i].inputs[j].scriptLength];
							
							f.read((char*) _blocks[curBlock].transactions[i].inputs[j].script, _blocks[curBlock].transactions[i].inputs[j].scriptLength);
							f.read((char*)& _blocks[curBlock].transactions[i].inputs[j].sequence, 4);
						}
						// For each output
						_blocks[curBlock].transactions[i].outputCount = readVarInt(f);
						_blocks[curBlock].transactions[i].outputs = new Output[_blocks[curBlock].transactions[i].outputCount];
						for(unsigned int j = 0; j<_blocks[curBlock].transactions[i].inputCount; j++){
							f.read((char*)& _blocks[curBlock].transactions[i].outputs[j].value, 8);
							
							_blocks[curBlock].transactions[i].outputs[j].scriptLength = readVarInt(f);
							_blocks[curBlock].transactions[i].outputs[j].script = new uint8_t[_blocks[curBlock].transactions[i].outputs[j].scriptLength];
							
							f.read((char*) _blocks[curBlock].transactions[i].outputs[j].script, _blocks[curBlock].transactions[i].outputs[j].scriptLength);
						}
						// And finally the lock time
						f.read((char*)& _blocks[curBlock].transactions[i].lockTime, 4);
					}
				}
				else{
				
				}

				_curPos = f.tellg();
			}
			else{
				//We need to take what we can..
			}
			break;
		}
		curBlock++;
		_nbRead++;
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
		//Note that the type of i isnt uint anymore, because of i>=0, i--
		for(int16_t i = size; i >= 0; i--){
			std::cout<<std::hex<<(int)bytes[i];
		}
	}
}

/**
*@brief display all bytes the tab contains, but trying to convert them as character (using ASCI table) if it's impossible, displays "."
*@param bytes -> a tab of bytes
*@param size -> nb of bytes
*@param be -> if set to true, for big endians
*/
void Chain::displayAsciBytes(uint8_t bytes[], uint32_t size, bool be /*= false*/){
	if(!be){
		for(uint8_t i = 0; i < size; i++){
			if(bytes[i] <= 126  && bytes[i] >= 32)
				std::cout<<std::hex<<bytes[i];
			else
				std::cout<<".";
		}
	}
	//If big endian
	else{
		for(int16_t i = size; i >= 0; i--){
			if(bytes[i] <= 126  && bytes[i] >= 32)
				std::cout<<std::hex<<bytes[i];
			else
				std::cout<<".";
		}
	}
}

void Chain::write(std::string output){
	std::ofstream f(output, std::ios::out);
	if(f.is_open()){
		for(int i = 0; i < _nbRead; i++){
			f<<std::dec<<"BLOCK #"<<i<<" : {"<<std::endl;
			f<<std::hex<<"	Magic byte : \""<<_blocks[i].header.magicBytes<<"\";"<<std::endl;
			f<<std::dec<<"	Nb of bytes : \""<<_blocks[i].header.blockLength<<"\";"<<std::endl;
			f<<"	header : {"<<std::endl;
			f<<std::hex<<"		Version number : "<<_blocks[i].header.version<<"\";"<<std::endl;
			f<<std::hex<<"		Previous block hash : \"";
			displayBytes(_blocks[i].header.prevHash, 32, true);
			f<<"\";"<<std::endl;
			f<<std::hex<<"		Merkle root : \"";
			displayBytes(_blocks[i].header.merkleRoot, 32, true);
			f<<"\";"<<std::endl;
			time_t t = 0;
			t += _blocks[i].header.timestamp;
			f<<std::dec<<"		Timestamp : \""<<asctime(localtime(&t)); //No need to std::endl after asctime
			f<<std::hex<<"		Target : \""<<_blocks[i].header.target<<"\";"<<std::endl;
			f<<std::dec<<"		Nonce : \""<<_blocks[i].header.nonce<<"\";"<<std::endl;
			f<<std::dec<<"		Number of transactions : \""<<_blocks[i].header.txCount<<"\";"<<std::endl;
			f<<<<"	};"<<std::endl;
					
			f<<"	Transactions : {"<<std::endl;
			for(unsigned int j = 0; j<_blocks[i].header.txCount; j++){
				f<<std::dec<<"		Transaction n° "<<j+1<<" : {"<<std::endl;
				f<<std::dec<<"			Version : \""<<_blocks[i].transactions[j].version<<"\";"<<std::endl;
				for(unsigned int k = 0; k<_blocks[i].transactions[j].inputCount; k++){
					f<<std::dec<<"			Input n° "<<k+1<<" : {"<<std::endl;
					f<<std::hex<<"				Hash : \"";
					displayBytes(_blocks[i].transactions[j].inputs[k].hash, 32);
					f<<std::endl;
					f<<std::hex<<"				Index : \""<<_blocks[i].transactions[j].inputs[k].index<<"\";"<<std::endl;
					f<<std::dec<<"				Script : \"";
					displayBytes(_blocks[i].transactions[j].inputs[k].script, _blocks[i].transactions[j].inputs[k].scriptLength);
					f<<std::endl;
					f<<std::hex<<"				Script (Asci) : \"";
					displayAsciBytes(_blocks[i].transactions[j].inputs[k].script, _blocks[i].transactions[j].inputs[k].scriptLength);
					f<<std::endl;
					f<<std::hex<<"				Sequence : \""<<_blocks[i].transactions[j].inputs[k].sequence<<"\";"<<std::endl;
					f<<"			};"<<std::endl;
				}
				for(unsigned int k = 0; k<_blocks[i].transactions[j].outputCount; k++){
					f<<std::dec<<"			Output n° "<<k+1<<" : {"<<std::endl;
					f<<std::dec<<"				Value : \""<<_blocks[i].transactions[j].outputs[k].value<<"\";"<<std::endl;
					f<<std::dec<<"		Script : \"";
					displayBytes(_blocks[i].transactions[j].outputs[k].script, _blocks[i].transactions[j].outputs[k].scriptLength);
					f<<std::endl;
					f<<std::hex<<"		Script (Asci) : \"";
					displayAsciBytes(_blocks[i].transactions[j].outputs[k].script, _blocks[i].transactions[j].outputs[k].scriptLength);
					f<<"			};"<<std::endl;
				}
				f<<"			Locktime : \""<<_blocks[i].transactions[j].lockTime<<"\";"<<std::endl;
				f<<"		};"<<std::endl;
			}
			f<<"	};"<<std::endl;
			f<<"};"<<std::endl;
			f<<std::endl;
	}
	else{
		f<<"Problem with output file"<<std::endl;
	}
}


void Chain::debug(){
	// Nothing to comment in this part, I'll just display on the prompt everything read in the read function,
	// for any question about the format see the read function or the doc.
	// Display format :
	// ~~~~~~~~~~~~~~~
	// ***************
	//     BLOCK n

	// ---METADATAs---
	// ---------------
	// ---HEADER------
	// ---------------
	// ---DATA-------- (the transactions)
	// ---------------
	
	// ***************
	// ~~~~~~~~~~~~~~~
	for(int i = 0; i < _nbRead; i++){
		std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
		std::cout<<"*******************************************************************************"<<std::endl;
		std::cout<<"                                  ----------"<<std::endl;
		std::cout<<std::dec<<"                                || BLOCK #"<<i<<" ||"<<std::endl;
		std::cout<<"                                  ----------"<<std::endl;
		std::cout<<std::endl;
		std::cout<<"-----------------------------------METADATAs-----------------------------------"<<std::endl;
		std::cout<<std::hex<<"Magic byte : "<<_blocks[i].header.magicBytes<<std::endl;
		std::cout<<std::dec<<"Nb of bytes : "<<_blocks[i].header.blockLength<<std::endl;
		std::cout<<std::endl<<"-------------------------------------------------------------------------------"<<std::endl;
		std::cout<<std::endl;
		
		std::cout<<std::endl<<"-------------------------------------HEADER------------------------------------"<<std::endl;
		std::cout<<std::hex<<"Version number : "<<_blocks[i].header.version<<std::endl;
		std::cout<<std::hex<<"Previous block hash : ";
		displayBytes(_blocks[i].header.prevHash, 32, true);
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
				
		std::cout<<std::endl<<"--------------------------------------DATA-------------------------------------"<<std::endl;
		for(unsigned int j = 0; j<_blocks[i].header.txCount; j++){
			std::cout<<std::dec<<"Transaction n° "<<j+1<<std::endl;
			std::cout<<std::dec<<"	Version : "<<_blocks[i].transactions[j].version<<std::endl;
			for(unsigned int k = 0; k<_blocks[i].transactions[j].inputCount; k++){
				std::cout<<std::dec<<"	Input n° "<<k+1<<std::endl;
				std::cout<<std::hex<<"		Hash : ";
				displayBytes(_blocks[i].transactions[j].inputs[k].hash, 32);
				std::cout<<std::endl;
				std::cout<<std::hex<<"		Index : "<<_blocks[i].transactions[j].inputs[k].index<<std::endl;
				std::cout<<std::dec<<"		Script : ";
				displayBytes(_blocks[i].transactions[j].inputs[k].script, _blocks[i].transactions[j].inputs[k].scriptLength);
				std::cout<<std::endl;
				std::cout<<std::hex<<"		Script (Asci) : ";
				displayAsciBytes(_blocks[i].transactions[j].inputs[k].script, _blocks[i].transactions[j].inputs[k].scriptLength);
				std::cout<<std::endl;
				std::cout<<std::hex<<"		Sequence :"<<_blocks[i].transactions[j].inputs[k].sequence<<std::endl;
			}
			for(unsigned int k = 0; k<_blocks[i].transactions[j].outputCount; k++){
				std::cout<<std::dec<<"	Output n° "<<k+1<<std::endl;
				std::cout<<std::dec<<"		Value : "<<_blocks[i].transactions[j].outputs[k].value<<std::endl;
				std::cout<<std::dec<<"		Script : ";
				displayBytes(_blocks[i].transactions[j].outputs[k].script, _blocks[i].transactions[j].outputs[k].scriptLength);
				std::cout<<std::endl;
				std::cout<<std::hex<<"		Script (Asci) : ";
				displayAsciBytes(_blocks[i].transactions[j].outputs[k].script, _blocks[i].transactions[j].outputs[k].scriptLength);
			}
			std::cout<<"Locktime : "<<_blocks[i].transactions[j].lockTime<<std::endl;
		}
		std::cout<<std::endl<<"-------------------------------------------------------------------------------"<<std::endl;
		std::cout<<std::endl<<std::endl<<std::endl;
		std::cout<<"*******************************************************************************"<<std::endl;
		std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
		if(i < _nbRead-1){
			std::cout<<"                                       |"<<std::endl;
			std::cout<<"                                       |"<<std::endl;
			std::cout<<"                                       |"<<std::endl;
			std::cout<<"                                       |"<<std::endl;
			std::cout<<"                                       |"<<std::endl;
		}
	}
}

