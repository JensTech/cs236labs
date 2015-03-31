// Robert Williams CS 236

using namespace std;

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <set>
#include <sstream>

class Node {
public:
	Node(string);
	~Node();
	void addChild(Node*);
	
	bool visited;
	set<Node*> children;
	string id;
private:
	
};

#endif /* NODE_H */