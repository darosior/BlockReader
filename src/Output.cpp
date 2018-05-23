#include <cstdint>

struct Output{
	uint64_t value; // In Satoshis
	uint64_t scriptLength; // A var int
	uint8_t * script;
};
