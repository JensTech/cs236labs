// Robert Williams CS 236

using namespace std;

#ifndef RELATION_H
#define RELATION_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <set>
#include <vector>

class Relation {
public:
	Relation(string, vector<string>);
	~Relation();
	void addRow(vector<string>);
	string toString();
	string name;
	vector<string> scheme;
	set<vector<string>> rows;
};

#endif /* RELATION_H */