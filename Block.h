#IFNDEF BLOCK
#DEFINE BLOCK

#include "Header.h"
#include "Transaction.h"

struct Block{
	Header header;
	Transaction * transactions;
};

#ENDIF
