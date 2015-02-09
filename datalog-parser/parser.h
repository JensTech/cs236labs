// Robert Williams CS 236

#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "token.h"
#include "datalogProgram.h"
#include "rule.h"
#include "predicate.h"
#include "parameter.h"

class Parser {
public:
	Parser(vector<Token*>);
	~Parser();
	DatalogProgram* datalogParsing();
private:
	vector<Token*> tokens;
	DatalogProgram* datalogProgram;
	// grammar functions
	void datalog();
	void scheme();
	void schemeList();
	void idList(Predicate* predicate);
	void fact();
	void factList();
	void rule();
	void ruleList();
	Predicate* headPredicate();
	Predicate* predicate();
	void predicateList(Rule* rule);
	Parameter* parameter();
	void parameterList(Predicate* predicate);
	Parameter* expression();
	Token* operatorGrammar();
	void query();
	void queryList();
	void stringList(Predicate* predicate);
	// end grammar functions
	Token* nextToken();
	tokenType nextTokenType();
	Token* consumeToken(tokenType);
	void putbackToken(Token*);
};

#endif /* PARSER_H */