// Robert Williams CS 236

#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include "token.h"
using namespace std;

#ifndef PARSER_H
#define PARSER_H

class Parser {
public:
	Parser(vector<Token*>);
	~Parser();
	void datalogParsing();
private:
	vector<Token*> tokens;
	void datalogProgram();
};

#endif /* PARSER_H */