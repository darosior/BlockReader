#include <cstdint>

struct Input{
	uint8_t * hash;
	uint32_t index;
	uint64_t scriptLength; // A var int
	uint8_t * script;
	uint32_t sequence;
};
