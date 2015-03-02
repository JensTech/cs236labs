// Robert Williams CS 236

using namespace std;

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
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
private:
	DatalogProgram* program;
	RelationalDatabase* database;

	void addSchemes();
	void addFacts();
	void runQueries();
};

#endif /* INTERPRETER_H */