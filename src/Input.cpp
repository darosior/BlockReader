#IFNDEF INPUT
#DEFINE INPUT

struct Input{
	uint8_t hash[32];
	uint32_t index;
	uint64_t scriptLength; // A var int
	uint8_t script[scriptLength];
	uint32_t sequence;
};

#ENDIF
