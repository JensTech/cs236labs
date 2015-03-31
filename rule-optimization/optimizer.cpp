#include "optimizer.h"

Optimizer::Optimizer(vector<Rule*> rules) {
	// assign id to each rule
	stringstream ss;
	for (unsigned int i = 0; i < rules.size(); i++) {
		ss << i;
		string id = 'R' + ss.str();
		ss.str("");

		rules[i]->id = id;
		Node* forward_node = new Node(id);
		Node* backward_node = new Node(id);
		this->rules.push_back(rules[i]);
		this->forward_tree.push_back(forward_node);
		this->backward_tree.push_back(backward_node);
	}

	this->buildTrees();
	this->buildPostorder();
}

Optimizer::~Optimizer() {
	for (unsigned int i = 0; i < this->forward_tree.size(); i++) {
		delete this->forward_tree[i];
	}
	for (unsigned int i = 0; i < this->backward_tree.size(); i++) {
		delete this->backward_tree[i];
	}
}

void Optimizer::buildTrees() {
	cout << "Dependency Graph" << endl;
	// build forward and backward trees
	for (unsigned int i = 0; i < this->rules.size(); i++) {
		cout << this->rules[i]->id << ":";
		for (unsigned int j = 0; j < this->rules[i]->predicateList.size(); j++) {
			for (unsigned int k = 0; k < this->rules.size(); k++) {
				string predicate_id = this->rules[i]->predicateList[j]->id->getExtracted();
				string rule_id = this->rules[k]->headPredicate->id->getExtracted();
				if (predicate_id == rule_id) {
					// forward tree
					if (this->forward_tree[i]->addChild(this->forward_tree[k])) {
						if (this->forward_tree[i]->children.size() > 1) cout << ","; // make sure at least one child was already there
						cout << this->rules[k]->id;
					}
					// backward tree
					this->backward_tree[k]->addChild(this->backward_tree[i]);
				}
			}
		}
		cout << endl;
	}
}

void Optimizer::buildPostorder() {
	for (unsigned int i = 0; i < this->backward_tree.size(); i++) {
		Node* node = this->backward_tree[i];
		if (!node->visited) this->dfsStack(node);
	}
}

vector<vector<Rule*>> Optimizer::strongConnections() {
	vector<vector<Rule*>> forest;
	while (this->postorder.size()) {
		Node* backward_node = postorder.top();
		postorder.pop();
		Node* forward_node = this->findNodeById(backward_node->id);
		if (!forward_node->visited) {
			forest.push_back(this->dfsVector(forward_node));
		}
	}
	return forest;
}

void Optimizer::dfsStack(Node* node) {
	node->visited = true;
	for (unsigned int i = 0; i < node->children.size(); i++) {
		if (!node->children[i]->visited) {
			this->dfsStack(node->children[i]);
		}
	}
	this->postorder.push(node);
}

vector<Rule*> Optimizer::dfsVector(Node* node) {
	node->visited = true;
	vector<Rule*> tree;
	tree.push_back(this->findRuleById(node->id));
	for (unsigned int i = 0; i < node->children.size(); i++) {
		if (node->children[i] == node) tree.back()->reflexive = true;
		if (!node->children[i]->visited) {
			vector<Rule*> sub_tree = this->dfsVector(node->children[i]);
			tree.insert(tree.end(), sub_tree.begin(), sub_tree.end());
		}
	}
	return tree;
}

Node* Optimizer::findNodeById(string id) {
	for (unsigned int i = 0; i < this->forward_tree.size(); i++) {
		if (id == this->forward_tree[i]->id) return this->forward_tree[i];
	}
	return NULL;
}

Rule* Optimizer::findRuleById(string id) {
	for (unsigned int i = 0; i < this->rules.size(); i++) {
		if (id == this->rules[i]->id) return this->rules[i];
	}
	return NULL;
}