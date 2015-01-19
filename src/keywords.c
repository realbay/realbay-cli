
#include "keywords.h"

#define is_keyword_char(c) (isalnum((int)c))

#define keyword_append(buf, i, c) \
	buf[i] = (char)tolower((int)c); \
	i++; \
	buf[i] = '\0';

size_t keyword_parse(const char *text, keyword_func *f) {
	size_t i;
	char c;
	size_t in_word;
	char keyword[MAX_KEYWORD_LENGTH];
	size_t keyword_len;
	
	if (text[0] == '\0') {
		return 0;
	}
	
	keyword[0] = '\0';
	keyword_len = 0;
	in_word = is_keyword_char(text[0]);
	
	for (i=0; i < MAX_TEXT_LENGTH; i++) {
		c = text[i];
		
		if (c == '\0') {
			break;
		}
		
		if (in_word) {
			if (is_keyword_char(c) && keyword_len < MAX_KEYWORD_LENGTH) {
				keyword_append(keyword, keyword_len, c);
			} else {
				//keyword[keyword_len] = '\0';
				f(keyword, keyword_len);
				
				in_word = 0;
				keyword_len = 0;
				keyword[0] = '\0';
			}
		} else {
			if (is_keyword_char(c)) {
				in_word = 1;
				keyword_append(keyword, keyword_len, c);
			} else {
				// Skipping this character
			}
		}
	}
	
	if (in_word && keyword_len > 0) {
		keyword[keyword_len] = '\0';
		f(keyword, keyword_len);
	}
	
	return 0;
}