// Robert Williams CS 236

#include "predicate.h"

Predicate::Predicate(Token* id) {
	this->id = id;
}

Predicate::~Predicate() {
}

void Predicate::addParameter(Parameter* parameter) {
	this->parameterList.push_back(parameter);
}

string Predicate::toString() {
	string output = "";
	output += this->id->getExtracted();
	output += "(" + this->parameterList[0]->toString();
	for (unsigned int i = 1; i < this->parameterList.size(); i++) {
		output += "," + this->parameterList[i]->toString();
	}
	output += ")";
	return output;
}