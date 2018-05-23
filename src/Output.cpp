#IFNDEF OUTPUT
#DEFINE OUTPUT

struct Input{
	uint64_t value; // In Satoshis
	uint64_t scriptLength; // A var int
	uint8_t script[scriptLength];
};

#ENDIF
