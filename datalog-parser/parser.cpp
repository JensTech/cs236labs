// Robert Williams CS 236

#include "parser.h"

Parser::Parser(vector<Token*> tokens) {
	this->tokens = tokens;
}

Parser::~Parser() {
}

void Parser::datalogParsing() {
	this->datalogProgram();
}

void Parser::datalogProgram() {
	if (this->tokens[0]->getType() == SCHEMES) {
		cout << this->tokens[0]->toString();
	}
}