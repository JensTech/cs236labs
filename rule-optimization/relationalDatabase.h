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
	Relation* select(Relation*, vector<string>);
	Relation* project(Relation*, vector<string>);
	Relation* rename(Relation*, vector<string>);
	Relation* relation_union(Relation*, Relation*);
	Relation* join(Relation*, Relation*);
	Relation* getRelation(string);
private:
	vector<Relation*> created_relations;
	map<string, Relation*> relations;
};

#endif /* RELATIONALDATABASE_H */