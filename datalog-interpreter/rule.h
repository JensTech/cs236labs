// Robert Williams CS 236

#ifndef RULE_H
#define RULE_H

#include "token.h"
#include "predicate.h"

class Rule {
public:
	Rule();
	~Rule();
	void setHeadPredicate(Predicate* headPredicate);
	void addPredicate(Predicate* predicate);
	string toString();
	
	Predicate* headPredicate;
	vector<Predicate*> predicateList;
};

#endif /* RULE_H */