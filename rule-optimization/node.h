// Robert Williams CS 236

using namespace std;

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <set>
#include <sstream>
#include <vector>

class Node {
public:
	Node(string);
	~Node();
	bool addChild(Node*);
	Node* findById(string);

	bool visited;
	vector<Node*> children;
	string id;
private:
	
};

#endif /* NODE_H */