// Robert Williams CS 236

#include "parser.h"

Parser::Parser(vector<Token*> tokens) {
	this->tokens = tokens;
	this->errorToken = NULL;
}

Parser::~Parser() {
}

void Parser::datalogParsing() {
	try {
		this->datalogProgram();
	} catch (Token* errorToken) {
		this->errorToken = errorToken;
	}
}

string Parser::toString() {
	if (this->errorToken != NULL) {
		return "Failure!\n  " + this->errorToken->toString();
	}
	return "Success!";
}

// grammar functions

void Parser::datalogProgram() {
	this->consumeToken(SCHEMES);
	this->consumeToken(COLON);
	
	this->scheme();
	this->schemeList();
	
	this->consumeToken(FACTS);
	this->consumeToken(COLON);
	
	this->factList();
	
	this->consumeToken(RULES);
	this->consumeToken(COLON);
	
	this->ruleList();
	
	this->consumeToken(QUERIES);
	this->consumeToken(COLON);
	
	this->query();
	this->queryList();
}

void Parser::scheme() {
	this->consumeToken(ID);
	this->consumeToken(LEFT_PAREN);
	this->consumeToken(ID);
	
	this->idList();
	
	this->consumeToken(RIGHT_PAREN);
}

void Parser::schemeList() {
	vector<Token*> tmp = this->tokens;
	try {
		scheme();
		schemeList();
	} catch (Token* errorToken) {
		this->tokens = tmp;
	}
}

void Parser::idList() {
	vector<Token*> tmp = this->tokens;
	try {
		this->consumeToken(COMMA);
		this->consumeToken(ID);
		this->idList();
	} catch (Token* errorToken) {
		this->tokens = tmp;
	}
}

void Parser::fact() {
	this->consumeToken(ID);
	this->consumeToken(LEFT_PAREN);
	this->consumeToken(STRING);
	
	this->stringList();
	
	this->consumeToken(RIGHT_PAREN);
	this->consumeToken(PERIOD);
}

void Parser::factList() {
	vector<Token*> tmp = this->tokens;
	try {
		this->fact();
		this->factList();
	} catch (Token* errorToken) {
		this->tokens = tmp;
	}
}

void Parser::rule() {
	this->headPredicate();
	
	this->consumeToken(COLON_DASH);
	
	this->predicate();
	this->predicateList();
	
	this->consumeToken(PERIOD);
}

void Parser::ruleList() {
	vector<Token*> tmp = this->tokens;
	try {
		this->rule();
		this->ruleList();
	} catch (Token* errorToken) {
		this->tokens = tmp;
	}
}

void Parser::headPredicate() {
	this->consumeToken(ID);
	this->consumeToken(LEFT_PAREN);
	this->consumeToken(ID);
	
	this->idList();
	
	this->consumeToken(RIGHT_PAREN);
}

void Parser::predicate() {
	this->consumeToken(ID);
	this->consumeToken(LEFT_PAREN);
	
	this->parameter();
	this->parameterList();
	
	this->consumeToken(RIGHT_PAREN);
}

void Parser::predicateList() {
	vector<Token*> tmp = this->tokens;
	try {
		this->consumeToken(COMMA);
		this->predicate();
		this->predicateList();
	} catch (Token* errorToken) {
		this->tokens = tmp;
	}
}

void Parser::parameter() {
	if (this->nextTokenType() == STRING) {
		this->consumeToken(STRING);
	} else if (this->nextTokenType() == ID) {
		this->consumeToken(ID);
	} else {
		this->expression();
	}
}

void Parser::parameterList() {
	vector<Token*> tmp = this->tokens;
	try {
		this->consumeToken(COMMA);
		this->parameter();
		this->parameterList();
	} catch (Token* errorToken) {
		this->tokens = tmp;
	}
}

void Parser::expression() {
	this->consumeToken(LEFT_PAREN);
	
	this->parameter();
	this->operatorGrammar();
	this->parameter();
	
	this->consumeToken(RIGHT_PAREN);
}

void Parser::operatorGrammar() {
	if (this->nextTokenType() == ADD) {
		this->consumeToken(ADD);
	} else {
		this->consumeToken(MULTIPLY);
	}
}

void Parser::query() {
	this->predicate();
	
	this->consumeToken(Q_MARK);
}

void Parser::queryList() {
	vector<Token*> tmp = this->tokens;
	try {
		this->query();
		this->queryList();
	} catch (Token* errorToken) {
		this->tokens = tmp;
	}
}

void Parser::stringList() {
	vector<Token*> tmp = this->tokens;
	try {
		this->consumeToken(COMMA);
		this->consumeToken(STRING);
		this->stringList();
	} catch (Token* errorToken) {
		this->tokens = tmp;
	}
}

// end grammar functions

Token* Parser::nextToken() {
	return this->tokens[0];
}

tokenType Parser::nextTokenType() {
	return this->tokens[0]->getType();
}

Token* Parser::consumeToken(tokenType type) {
	if (this->nextTokenType() != type) {
		throw this->nextToken();
	}
	Token* tmp = this->tokens[0];
	cout << tmp->toString() << endl;
	this->tokens.erase(this->tokens.begin());
	return tmp;
}