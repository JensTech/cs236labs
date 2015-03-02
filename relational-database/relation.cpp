// Robert Williams CS 236

#include "relation.h"

Relation::Relation(string name, vector<string> scheme) {
	this->name = name;
	this->scheme = scheme;
}

Relation::~Relation() {

}

void Relation::addRow(vector<string> row) {
	this->rows.insert(row);
}

string Relation::toString() {
	string output = "";
	for (unsigned int i = 0; i < this->scheme.size(); i++) {
		output += this->scheme[i] + " | ";
	}
	output += "\n";
	for (set<vector<string>>::iterator i = this->rows.begin(); i != this->rows.end(); i++) {
		vector<string> row = *i;
		for (unsigned int j = 0; j < row.size(); j++) {
			output += row[j] + ", ";
		}
		output += "\n";
	}

	return output;
}