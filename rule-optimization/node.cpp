#include "node.h"

Node::Node(string id) {
	this->id = id;
	this->visited = false;
}

Node::~Node() {}

bool Node::addChild(Node* node) {
	// disallow duplicates
	if (this->findById(node->id) != NULL) {
		return false;
	}
	// insert in sorted order
	if (this->children.size() == 0) {
		this->children.push_back(node);
		return true;
	}
	for (unsigned int i = 0; i < this->children.size(); i++) {
		if (node->id < (this->children[i]->id)) {
			this->children.insert(this->children.begin() + i, node);
			return true;
		}
	}
	this->children.push_back(node);
	return true;
}

Node* Node::findById(string id) {
	for (unsigned int i = 0; i < this->children.size(); i++) {
		if (id == this->children[i]->id) return this->children[i];
	}
	return NULL;
}