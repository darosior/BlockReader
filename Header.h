#IFNDEF HEADER
#DEFINE HEADER

struct Header{
	uint32_t _magicBytes;
	uint32_t _blockLength;
	uint32_t _version;
	uint8_t * _prevHash;
	uint8_t * _merkleRoot;
	uint32_t _timestamp;
	uint32_t _target;
	uint32_t _difficulty;
	uint32_t _nonce;
	uint64_t _txCount;
};

#ENDIF
