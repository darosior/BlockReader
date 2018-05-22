#IFNDEF BLOCK
#DEFINE BLOCK

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include "Header.h"

class Block{
	public:
	
		Block(Header header);
	
	private:
	
		Header _header;
		Transaction * _transactions;
};

#ENDIF
