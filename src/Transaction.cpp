#include "Input.cpp"
#include "Output.cpp"

struct Transaction{
	uint64_t inputCount; // A var int
	Input * inputs;
	uint64_t outputCount; // Another one
	Output * outputs;
	uint32_t lockTime;
};
