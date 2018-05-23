#IFNDEF BLOCK
#DEFINE BLOCK

#include "Header.h"
#include "Transaction.h"

struct Block{
	Header _header;
	Transaction * _transactions;
};

#ENDIF
