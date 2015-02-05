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
	string toString();
private:
	vector<Token*> tokens;
	Token* errorToken;
	// grammar functions
	void datalog();
	void scheme();
	void schemeList();
	void idList();
	void fact();
	void factList();
	void rule();
	void ruleList();
	void headPredicate();
	void predicate();
	void predicateList();
	void parameter();
	void parameterList();
	void expression();
	void operatorGrammar();
	void query();
	void queryList();
	void stringList();
	// end grammar functions
	Token* nextToken();
	tokenType nextTokenType();
	Token* consumeToken(tokenType);
	void putbackToken(Token*);
};

#endif /* PARSER_H */