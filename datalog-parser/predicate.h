// Robert Williams CS 236

#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include "token.h"
using namespace std;

#ifndef PREDICATE_H
#define PREDICATE_H

class Predicate {
public:
	Predicate();
	~Predicate();
private:
	Token* id;
	vector<Parameter*> parameters;
};

#endif /* PREDICATE_H */