// Robert Williams CS 236

#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include "token.h"
using namespace std;

#ifndef DATALOG_PROGRAM_H
#define DATALOG_PROGRAM_H

class DatalogProgram {
public:
	DatalogProgram();
	~DatalogProgram();
private:
	vector<Predicate*> schemes;
	vector<Predicate*> facts;
	vector<Rule*> rules;
	vector<Predicate*> queries;
};

#endif /* DATALOG_PROGRAM_H */