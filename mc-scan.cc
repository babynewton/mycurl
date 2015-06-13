#include "mc-scan.h"
#include <cstdio>
#include <map>

struct mcSymbolTable{
	const char* str;
	mcTokenID id;
} s_table[] = {
		{"open", MC_TOKEN_OPEN},
		{"get", MC_TOKEN_GET},
		{"put", MC_TOKEN_PUT},
		{"post", MC_TOKEN_POST},
		{"delete", MC_TOKEN_DELETE},
		{"close", MC_TOKEN_CLOSE},
		{"opt", MC_TOKEN_OPT},
		{"quit", MC_TOKEN_QUIT},
		{"help", MC_TOKEN_HELP},
		{NULL, MC_TOKEN_NONE}
};

map<string, mcTokenID> s_map;

mcScanner::mcScanner(const char* line):m_line(line), m_pos((char*)line) {
	if(s_map.empty()) {
		for(size_t i = 0 ; s_table[i].str ; i++)
			s_map[s_table[i].str] = s_table[i].id;
	}
}

mcToken mcScanner::scan(void) {
	mcTokenID id;
	string buffer;

	if (*m_pos == '\n') {
		id = MC_TOKEN_EOL;
		m_pos++;
	} else {
		while (*m_pos && *m_pos != '\n' && *m_pos != ' ') {
			buffer += *m_pos;
			m_pos++;
		}
		id = MC_TOKEN_STRING;
		map<string, mcTokenID>::iterator it = s_map.find(buffer);
		if(it != s_map.end()) id = it->second;
	}
	mcToken token(id, buffer);
	return token;
}