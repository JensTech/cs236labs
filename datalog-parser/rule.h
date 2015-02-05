// Robert Williams CS 236

#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include "token.h"
using namespace std;

#ifndef RULE_H
#define RULE_H

class Rule {
public:
	Rule();
	~Rule();
private:
	Predicate* headPredicate;
	vector<Predicate*> predicateList;
};

#endif /* RULE_H */