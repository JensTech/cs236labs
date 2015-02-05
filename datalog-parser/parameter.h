// Robert Williams CS 236

#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include "token.h"
using namespace std;

#ifndef PARAMETER_H
#define PARAMETER_H

class Parameter {
public:
	Parameter();
	~Parameter();
private:
	Predicate* headPredicate;
	vector<Predicate*> predicateList;
};

#endif /* PARAMETER_H */