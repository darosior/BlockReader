#include "Block.h"

Block::Block(Header header){
	_header = header;
	_transactions = new Transaction[header.get_txCount];
}
