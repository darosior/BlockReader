#include "Header.cpp"
#include "Transaction.cpp"

struct Block{
	Header header;
	Transaction * transactions;
};
