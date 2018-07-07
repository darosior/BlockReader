#ifndef CHAIN
#define CHAIN

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>
#include <iomanip>
#include <cstdint>
#include <time.h>
#include "Block.cpp"

class Chain{
	public:
	
		Chain(std::string dataDir, int nbToRead);
		~Chain();
		
		/**
 		*@brief reads the varInt, following https://en.bitcoin.it/wiki/Protocol_documentation#Variable_length_integer
		*@param f an input stream, where to read
		*@return a 64 bit int containing the int to read (even if it was a 8, 16 or 32 bit
		*/
		uint64_t readVarInt(std::ifstream &f);
		
		/**
 		*@brief reads the specified number of blocks, and stock their spec in struct attributes
		*/
		void read();
		
		/**
		*@brief display all bytes the tab contains
		*@param bytes -> a tab of bytes
		*@param size -> nb of bytes
		*@param be -> if set to true, for big endians
		*/
		void displayBytes(uint8_t bytes[], uint32_t size, bool be = false);
		
		/**
		*@brief display all bytes the tab contains, but trying to convert them as character (using ASCI table) if it's impossible, displays "."
		*@param bytes -> a tab of bytes
		*@param size -> nb of bytes
		*@param be -> if set to true, for big endians
		*/
		void displayAsciBytes(uint8_t bytes[], uint32_t size, bool be = false);
		
		/**
		*@brief write all bytes the tab contains to the file
		*@param file -> the output file
		*@param bytes -> a tab of bytes
		*@param size -> nb of bytes
		*@param be -> if set to true, for big endians
		*/
		void displayBytes2File(std::ofstream &f, uint8_t bytes[], uint32_t size, bool be = false);
		
		/**
		*@brief writing all bytes the tab contains to the file, but trying to convert them as character (using ASCI table) if it's impossible, displays "."
		*@param bytes -> a tab of bytes
		*@param size -> nb of bytes
		*@param be -> if set to true, for big endians
		*/
		void displayAsciBytes2File(std::ofstream &f, uint8_t bytes[], uint32_t size, bool be = false);
		
		
		/**
 		*@brief Writes the blocks specifications in output file
		*@param output the file to write
		*@param startingBlock start writing result from this block
		*/
		void write(std::string output, int startingBlock = 0);	
		
		/**
 		*@brief Writes the blocks specifications to the console
		*/
		void debug();
		
	private:
	
		std::string _dataDir; // Directory from which to load the blocks
		int _nbToRead; // Number of blocks to read
		int _nbRead; // Number of blocks already read
		Block * _blocks;
		std::string _filename; // Current file to read
		std::ifstream::pos_type _curPos; // Position of the cursor in the file. Max file size = 2.8MB = 280 000 000 bytes, should fit in an int
		int _curFile; // Current blk file nb
		
		void set_fileName(); // If end of blkxxxn.dat, sets filename to blkxxxx(n+1).dat
};

#endif
