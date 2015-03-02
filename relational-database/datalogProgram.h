// Robert Williams CS 236

#ifndef DATALOG_PROGRAM_H
#define DATALOG_PROGRAM_H

#include "token.h"
#include <vector>
#include <algorithm>
#include "predicate.h"
#include "rule.h"
#include <sstream>

class DatalogProgram {
public:
	DatalogProgram();
	~DatalogProgram();
	void addScheme(Predicate* scheme);
	void addFact(Predicate* fact);
	void addRule(Rule* rule);
	void addQuery(Predicate* query);
	void addDomainValue(string domainValue);
	void setError(Token* errorToken);
	string toString();
	
	vector<Predicate*> schemes;
	vector<Predicate*> facts;
	vector<Rule*> rules;
	vector<Predicate*> queries;
	vector<string> domain;
	Token* errorToken;

private:
	void sortDomain();
};

#endif /* DATALOG_PROGRAM_H */