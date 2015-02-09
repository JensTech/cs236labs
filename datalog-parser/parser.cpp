// Robert Williams CS 236

#include "parser.h"

Parser::Parser(vector<Token*> tokens) {
	this->tokens = tokens;
	this->datalogProgram = NULL;
}

Parser::~Parser() {
}

DatalogProgram* Parser::datalogParsing() {
	if (this->datalogProgram != NULL) delete this->datalogProgram;
	this->datalogProgram = new DatalogProgram();
	try {
		this->datalog();
		// delete unused tokens
		for (unsigned int i = 0; i < this->tokens.size(); i++) {
			delete this->tokens[i];
		}
	} catch (Token* errorToken) {
		this->datalogProgram->setError(errorToken);
		// delete unused tokens
		for (unsigned int i = 1; i < this->tokens.size(); i++) {
			delete this->tokens[i];
		}
	}
	return this->datalogProgram;
}

// grammar functions

void Parser::datalog() {
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
	Predicate* scheme = new Predicate(this->consumeToken(ID));
	this->datalogProgram->addScheme(scheme);
	this->consumeToken(LEFT_PAREN);
	
	// put the expected id into a parameter object and add it to the scheme
	scheme->addParameter(new Parameter(this->consumeToken(ID)));
	this->idList(scheme);
	
	this->consumeToken(RIGHT_PAREN);
}

void Parser::schemeList() {
	if (this->nextTokenType() == ID) {
		scheme();
		schemeList();
	}
}

void Parser::idList(Predicate* predicate) {
	if (this->nextTokenType() == COMMA) {
		this->consumeToken(COMMA);
		
		// put the expected id into a parameter object and add it to the predicate
		predicate->addParameter(new Parameter(this->consumeToken(ID)));
		this->idList(predicate);
	}
}

void Parser::fact() {
	Predicate* fact = new Predicate(this->consumeToken(ID));
	this->datalogProgram->addFact(fact);
	this->consumeToken(LEFT_PAREN);
	
	
	// add the string to the program domain
	Token* stringToken = this->consumeToken(STRING);
	this->datalogProgram->addDomainValue(stringToken->getExtracted());
	// put the expected string into a parameter object and add it to the predicate
	fact->addParameter(new Parameter(stringToken));
	this->stringList(fact);
	
	this->consumeToken(RIGHT_PAREN);
	this->consumeToken(PERIOD);
}

void Parser::factList() {
	if (this->nextTokenType() == ID) {
		this->fact();
		this->factList();
	}
}

void Parser::rule() {
	Rule* rule = new Rule();
	this->datalogProgram->addRule(rule);
	rule->setHeadPredicate(this->headPredicate());
	
	this->consumeToken(COLON_DASH);
	
	rule->addPredicate(this->predicate());
	this->predicateList(rule);
	
	this->consumeToken(PERIOD);
}

void Parser::ruleList() {
	if (this->nextTokenType() == ID) {
		this->rule();
		this->ruleList();
	}
}

Predicate* Parser::headPredicate() {
	Token* id = this->consumeToken(ID);
	Predicate* headPredicate = new Predicate(id);
	this->consumeToken(LEFT_PAREN);
	
	// put the expected id into a parameter object and add it to the head predicate
	headPredicate->addParameter(new Parameter(this->consumeToken(ID)));
	this->idList(headPredicate);
	
	this->consumeToken(RIGHT_PAREN);
	
	return headPredicate;
}

Predicate* Parser::predicate() {
	Predicate* predicate = new Predicate(this->consumeToken(ID));
	this->consumeToken(LEFT_PAREN);
	
	predicate->addParameter(this->parameter());
	this->parameterList(predicate);
	
	this->consumeToken(RIGHT_PAREN);
	
	return predicate;
}

void Parser::predicateList(Rule* rule) {
	if (this->nextTokenType() == COMMA) {
		this->consumeToken(COMMA);
		rule->addPredicate(this->predicate());
		this->predicateList(rule);
	}
}

Parameter* Parser::parameter() {
	if (this->nextTokenType() == STRING) {
		return new Parameter(this->consumeToken(STRING));
	} else if (this->nextTokenType() == ID) {
		return new Parameter(this->consumeToken(ID));
	} else {
		return this->expression();
	}
}

void Parser::parameterList(Predicate* predicate) {
	if (this->nextTokenType() == COMMA) {
		this->consumeToken(COMMA);
		predicate->addParameter(this->parameter());
		this->parameterList(predicate);
	}
}

Parameter* Parser::expression() {
	this->consumeToken(LEFT_PAREN);
	
	Parameter* expression = new Parameter(NULL);
	Parameter* firstParameter = this->parameter();
	Token* operatorToken = this->operatorGrammar();
	Parameter* secondParameter = this->parameter();
	expression->expression(firstParameter, operatorToken, secondParameter);
	
	this->consumeToken(RIGHT_PAREN);
	
	return expression;
}

Token* Parser::operatorGrammar() {
	if (this->nextTokenType() == ADD) {
		return this->consumeToken(ADD);
	} else {
		return this->consumeToken(MULTIPLY);
	}
}

void Parser::query() {
	Predicate* query = this->predicate();
	this->datalogProgram->addQuery(query);
	
	this->consumeToken(Q_MARK);
}

void Parser::queryList() {
	if (this->nextTokenType() == ID) {
		this->query();
		this->queryList();
	}
}

void Parser::stringList(Predicate* predicate) {
	if (this->nextTokenType() == COMMA) {
		this->consumeToken(COMMA);
		
		// store the string in the domain
		Token* stringToken = this->consumeToken(STRING);
		this->datalogProgram->addDomainValue(stringToken->getExtracted());
		// put the expected string into a parameter object and add it to the predicate
		predicate->addParameter(new Parameter(stringToken));
		this->stringList(predicate);
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
	this->tokens.erase(this->tokens.begin());
	// delete unused tokens
	if (type != ID && type != STRING && type != ADD && type != MULTIPLY) {
		delete tmp;
	}
	return tmp;
}