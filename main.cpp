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

// From https://en.bitcoin.it/wiki/Block and http://codesuppository.blogspot.fr/2014/01/how-to-parse-bitcoin-blockchain.html
#define MAGIC_SIZE 4
#define NBBYTES_SIZE 4
#define VERSION_SIZE 4
#define PREVHASH_SIZE 32
#define MERKLE_SIZE 32
#define TIMESTAMP_SIZE 4
#define TARGET_SIZE 4
#define NONCE_SIZE 4


/**
*@brief adds all bytes the tab contains
*@param bytes -> a tab of bytes
*@param size -> nb of bytes
*@param be -> if set to true, for big endians
*@return the sum of the bytes
*/
uint32_t addBytes(uint8_t bytes[], uint32_t size, bool be = false){
	uint32_t total;
	//If little endian
	if(!be){
		total = bytes[0];
		for(uint32_t i = 1; i < size; i++){
			total = total<<8 | bytes[i];
		}
	}
	//If big endian
	else{
		total = bytes[3];
		//Note that the type of i isnt uint anymore, because of i>=0, i--
		for(int32_t i = 2; i >= 0; i--){
			total = total<<8 | bytes[i];
		}
	}
	return total;
}

/**
*@brief display all bytes the tab contains
*@param bytes -> a tab of bytes
*@param size -> nb of bytes
*@param be -> if set to true, for big endians
*/
void displayBytes(uint8_t bytes[], uint32_t size, bool be = false){
	if(!be){
		for(uint32_t i = 0; i < size; i++){
			cout<<hex<<(int)bytes[i];
		}
	}
	//If big endian
	else{
		for(int32_t i = size; i >= 0; i--){
			cout<<hex<<(int)bytes[i];
		}
	}
}

int main(int argc, char * argv[]){
	if(!(argc == 3)){
		cout<<"usage : ./bin <blockDir> <nb of blocks to read>"<<endl;
		return 0;
	}
	string dirname = argv[1];
	const uint32_t nbBlocks = atoi(argv[2]);
	uint32_t curBlock = 0;
	string filename;
	uint8_t *magicBytes, *nbBytes, *version, *prevHash, *merkle, *timestamp, *target, *nonce;
	while(curBlock <= nbBlocks){
		filename = dirname+"/blk0000"+to_string(curBlock)+".dat"; // Concatenation of the block name + its number
		cout<<filename<<endl;
		ifstream blockFile(filename, ios::in | ios::binary | ios::ate);
		if(blockFile.is_open()){
			magicBytes = new uint8_t[4];// The magic bytes are on 4 bytes..
			nbBytes = new uint8_t[4];// ...So is the number of bytes till in the file...
			version = new uint8_t[4];// ...Just like the version number
			prevHash = new uint8_t[32];// Becoming serious
			merkle = new uint8_t[32];
			timestamp = new uint8_t[4];// Back on our feets
			target = new uint8_t[4];
			nonce = new uint8_t[4];
			
			blockFile.seekg(0);
			blockFile.read((char*)magicBytes, MAGIC_SIZE);
			blockFile.read((char*)nbBytes, NBBYTES_SIZE);
			blockFile.read((char*)version, VERSION_SIZE);
			blockFile.read((char*)prevHash, PREVHASH_SIZE);
			blockFile.read((char*)merkle, MERKLE_SIZE);
			blockFile.read((char*)timestamp, TIMESTAMP_SIZE);
			blockFile.read((char*)target, TARGET_SIZE);
			blockFile.read((char*)nonce, NONCE_SIZE);
			blockFile.close();
			
			cout<<"                                       ----------"<<endl;
			cout<<"                                     || BLOCK #1 ||"<<endl;
			cout<<"                                       ----------"<<endl;
			cout<<hex<<"Magic byte : "<<addBytes(magicBytes, MAGIC_SIZE)<<"(little endian), "<<addBytes(magicBytes, MAGIC_SIZE, true)<<"(big endian)"<<endl;
			uint32_t nb = addBytes(nbBytes, NBBYTES_SIZE, true);
			uint32_t nb2 = addBytes(nbBytes, NBBYTES_SIZE);
			cout<<hex<<"Nb of bytes : "<< nb <<" (="<<dec<< nb <<" in dec) => little endian"<<endl;
			cout<<hex<<"Nb of bytes : "<< nb2 <<" (="<<dec<< nb2 <<" in dec) => big endian"<<endl;
			
			cout<<endl<<"-----------------HEADER-------------------"<<endl;
			cout<<hex<<"Version number : "<<addBytes(version, VERSION_SIZE, true)<<endl;
			cout<<hex<<"Previous block hash : ";
			displayBytes(prevHash, PREVHASH_SIZE, true);
			cout<<endl;
			cout<<hex<<"Merkle root : ";
			displayBytes(merkle, MERKLE_SIZE, true);
			cout<<endl;
			cout<<dec<<"Timestamp : "<<addBytes(timestamp, TIMESTAMP_SIZE)<<endl;
			cout<<hex<<"Target : "<<addBytes(target, TARGET_SIZE)<<endl;
			cout<<dec<<"Nonce : "<<addBytes(nonce, NONCE_SIZE, true)<<endl;
			
			cout<<endl<<endl<<endl;
			
			
			delete[] magicBytes;
			delete[] nbBytes;
			delete[] version;
			delete[] prevHash;
			delete[] merkle;
			delete[] timestamp;
			delete[] target;
			delete[] nonce;
			magicBytes = NULL;// We are gonna use them again
			nbBytes = NULL;
			version = NULL;
			prevHash = NULL;
			merkle = NULL;
			timestamp = NULL;
			target = NULL;
			nonce = NULL;
		}
		else{
			cout<<"Wrong directory name, or wrong number of file to read."<<endl;
			return 1;
		}
		curBlock++; // Next please !!
	}
	return 0;
}
