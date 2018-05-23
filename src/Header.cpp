#include <cstdint>

struct Header{
	uint32_t magicBytes;
	uint32_t blockLength;
	uint32_t version;
	uint8_t * prevHash;
	uint8_t * merkleRoot;
	uint32_t timestamp;
	uint32_t target;
	uint32_t difficulty;
	uint32_t nonce;
	uint64_t txCount;
};
