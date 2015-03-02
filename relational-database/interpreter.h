// Robert Williams CS 236

using namespace std;

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "token.h"
#include "datalogProgram.h"
#include "predicate.h"
#include "parameter.h"
#include "relationalDatabase.h"
#include "relation.h"

class Interpreter {
public:
	Interpreter(DatalogProgram*);
	~Interpreter();
	void buildDatabase();
	string runQueries();
private:
	DatalogProgram* program;
	RelationalDatabase* database;

	void addSchemes();
	void addFacts();
};

#endif /* INTERPRETER_H */