#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include "token.h"
using namespace std;

class Scanner {
public:
	Scanner(istream* in);
	~Scanner();
	void lexicalAnalysis();
private:
	Token* consume_string();
	Token* consume_line_comment();
	Token* consume_block_comment();
	Token* consume_alpha_numeric();
	map<string, tokenType> keyword = {{"Schemes", SCHEMES}, {"Facts", FACTS}, {"Rules", RULES}, {"Queries", QUERIES}};
	int line = 1;
	istream* in;
};
