// Robert Williams CS 236

#include "rule.h"

Rule::Rule() {
	this->headPredicate = NULL;
}

Rule::~Rule() {
	delete this->headPredicate;
	for (unsigned int i = 0; i < this->predicateList.size(); i++) {
		delete this->predicateList[i];
	}
}

void Rule::setHeadPredicate(Predicate* headPredicate) {
	this->headPredicate = headPredicate;
}

void Rule::addPredicate(Predicate* predicate) {
	this->predicateList.push_back(predicate);
}

string Rule::toString() {
	string output = "";
	output += this->headPredicate->toString() + " :- " + this->predicateList[0]->toString();
	for (unsigned int i = 1; i < this->predicateList.size(); i++) {
		output += "," + this->predicateList[i]->toString();
	}
	return output;
}