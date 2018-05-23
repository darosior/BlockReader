#IFNDEF TRANSACTION
#DEFINE TRANSACTION

#include "Input.h"
#include "Output.h"

struct Transaction{
	uint64_t inputCount; // A var int
	Input inputs[inputCount];
	uint64_t outputCount; // Another one
	Output outputs[outputCount];
	uint32_t lockTime;
};

#ENDIF
