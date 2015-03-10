// Robert Williams CS 236


#ifndef PARAMETER_H
#define PARAMETER_H

#include "token.h"

class Parameter {
public:
	Parameter(Token* token);
	~Parameter();
	void expression(Parameter* firstParameter, Token* operatorToken, Parameter* secondParameter);
	void terminal(Token* token);
	string toString();
	
	tokenType type;
	Token* value;
	Parameter* firstParameter;
	Parameter* secondParameter;
	Token* operatorToken;
};

#endif /* PARAMETER_H */