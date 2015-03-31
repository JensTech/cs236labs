// Robert Williams CS 236

using namespace std;

#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <stack>
#include <sstream>
#include "datalogProgram.h"
#include "node.h"

class Optimizer {
public:
	Optimizer(vector<Rule*>);
	~Optimizer();
private:
	void buildTrees();
	void buildPostorder();
	vector<vector<Rule*>> strongConnections();
	void dfsStack(Node*);
	vector<Rule*> dfsVector(Node*);
	Node* findNodeById(string);
	Rule* findRuleById(string);

	vector<Rule*> rules;
	vector<Node*> forward_tree;
	vector<Node*> backward_tree;
	stack<Node*> postorder;

};

#endif /* OPTIMIZER_H */