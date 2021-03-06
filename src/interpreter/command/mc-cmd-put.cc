#include "command/mc-cmd-put.h"
#include <cstdio>
#include <cstdlib>

void mcCmdPut::help(void){
	fprintf(stdout, "Usage: put [< file_in[:number]][ > file_out][ - list]\n");
	fprintf(stdout, "  posts GET request to the server.\n");
	fprintf(stdout, "  < operator redirects the request body from the file if there is any.\n");
	fprintf(stdout, "  :number specifies the size of the chunk.\n");
	fprintf(stdout, "  Transfer-Encoding:chunked header is automatically attached when specified.\n");
	fprintf(stdout, "  > operator redirects the response body to the file if there is any.\n");
	fprintf(stdout, "  list has headers for the request. Default list is defhdr.\n");
}

mcLanguageState mcCmdPut::parse(mcScanner& scanner, mcIPerformer* performer){
	string inpath, outpath, lst = "defhdr";
	size_t chunk = 0;
	mcToken token = scanner.tokenize();
	if(token.id == MC_TOKEN_LT) {
		token = scanner.tokenize();
		if(token.id == MC_TOKEN_STRING) {
			size_t pos = token.buffer.find(':');
			if(pos == string::npos) {
				inpath = token.buffer;
			} else {
				inpath = token.buffer.substr(0, pos);
				chunk = atoi(token.buffer.substr(pos + 1).c_str());
			}
		} else {
			fprintf(stderr, "in file path is missing\n");
			return MC_LANG_CONTINUE;
		}
		token = scanner.tokenize();
	}
	if(token.id == MC_TOKEN_GT) {
		token = scanner.tokenize();
		if(token.id == MC_TOKEN_STRING) {
			outpath = token.buffer;
		} else {
			fprintf(stderr, "out file path is missing\n");
			return MC_LANG_CONTINUE;
		}
		token = scanner.tokenize();
	}
	if(token.id == MC_TOKEN_HIPEN) {
		token = scanner.tokenize();
		if(token.id == MC_TOKEN_STRING) {
			lst = token.buffer;
		} else {
			fprintf(stderr, "list name is missing\n");
			return MC_LANG_CONTINUE;
		}
		token = scanner.tokenize();
	}
	if(token.id != MC_TOKEN_EOL) {
		fprintf(stderr, "invalid argument %s\n", token.buffer.c_str());
		return MC_LANG_CONTINUE;
	}
	return performer->put(inpath, chunk, outpath, lst);
}
