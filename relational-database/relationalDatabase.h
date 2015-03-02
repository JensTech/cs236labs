// Robert Williams CS 236

#ifndef RELATIONALDATABASE_H
#define RELATIONALDATABASE_H

#include <iostream>
#include <stdio.h>
#include <map>
#include <string>
#include <set>
#include <vector>
#include "relation.h"
using namespace std;

class RelationalDatabase {
public:
	RelationalDatabase();
	~RelationalDatabase();
	void addRelation(string, Relation*);
	Relation* select(string, vector<string>);
	Relation* project(string, vector<int>);
	Relation* rename(string, vector<string>);
	Relation* getRelation(string);
private:
	map<string, Relation*> relations;
};

#endif /* RELATIONALDATABASE_H */