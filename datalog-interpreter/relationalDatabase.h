// Robert Williams CS 236

#ifndef RELATIONALDATABASE_H
#define RELATIONALDATABASE_H

#include <iostream>
#include <stdio.h>
#include <map>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include "relation.h"
using namespace std;

class RelationalDatabase {
public:
	RelationalDatabase();
	~RelationalDatabase();
	void addRelation(string, Relation*);
	Relation* select(string, vector<string>);
	Relation* project(string, vector<string>);
	Relation* rename(string, vector<string>);
	Relation* relation_union(string, string);
	Relation* join(string, string);
	Relation* getRelation(string);
private:
	vector<Relation*> created_relations;
	map<string, Relation*> relations;
};

#endif /* RELATIONALDATABASE_H */