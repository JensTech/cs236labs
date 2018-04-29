// Robert Williams CS 236

#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include "token.h"
using namespace std;

#ifndef SCANNER_H
#define SCANNER_H

class Scanner {
public:
	Scanner(istream* in);
	~Scanner();
	vector<Token*> lexicalAnalysis();
private:
	Token* consume_string();
	Token* consume_line_comment();
	Token* consume_block_comment();
	Token* consume_alpha_numeric();
	map<string, tokenType> keyword = {{"Schemes", SCHEMES}, {"Facts", FACTS}, {"Rules", RULES}, {"Queries", QUERIES}};
	int line = 1;
	istream* in;
};

#endif /* SCANNER_H */
