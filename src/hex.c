
#include "hex.h"


uint8_t hex_decode_char(const char c) {
	switch (c) {
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case 'a': return 10;
	case 'A': return 10;
	case 'b': return 11;
	case 'B': return 11;
	case 'c': return 12;
	case 'C': return 12;
	case 'd': return 13;
	case 'D': return 13;
	case 'e': return 14;
	case 'E': return 14;
	case 'f': return 15;
	case 'F': return 15;
	}
	
	return 0;
}

char hex_encode_nibble(const uint8_t nibble) {
	switch (nibble) {
	case 0: return '0';
	case 1: return '1';
	case 2: return '2';
	case 3: return '3';
	case 4: return '4';
	case 5: return '5';
	case 6: return '6';
	case 7: return '7';
	case 8: return '8';
	case 9: return '9';
	case 10: return 'a';
	case 11: return 'b';
	case 12: return 'c';
	case 13: return 'd';
	case 14: return 'e';
	case 15: return 'f';
	}
	
	return '0';
}

void hex_encode_byte(const uint8_t value, char *a, char *b) {
	*a = hex_encode_nibble(value & 0x0f);
	*b = hex_encode_nibble((value >> 4) & 0x0f);
}

void hex_decode(const char *hex, uint8_t *bytes) {
	size_t i;
	size_t pos = 0;
	uint8_t a, b;
	
	if (hex == NULL || hex[0] == '\0') {
		return;
	}
	
	for (i=0; i < 40; i += 2) {
		if (hex[i] == '\0') {
			break;
		}
		
		a = hex_decode_char(hex[i]);
		
		if (hex[i + 1] == '\0') {
			b = 0;
		} else {
			b = hex_decode_char(hex[i + 1]);
		}
		
		bytes[pos] = (a) | (b << 4);
		pos++;
	}
	
	while (pos < 20) {
		bytes[pos] = 0;
		pos++;
	}
}

void hex_encode(const uint8_t *bytes, char *hex) {
	size_t i;
	
	for (i=0; i < 20; i++) {
		hex_encode_byte(bytes[i], hex, hex + 1);
		hex += 2;
	}
	
	*hex = '\0';
}


