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
	for (set<vector<string>>::iterator i = this->rows.begin(); i != this->rows.end(); i++) {
		vector<string> row = *i;
		for (unsigned int j = 0; j < row.size(); j++) {
			// don't print duplicate columns
			bool print_row = true;
			for (unsigned int k = 0; k < j; k++) {
				if (this->scheme[j] == this->scheme[k]) {
					print_row = false;
					break;
				}
			}
			if (!print_row) continue;

			if (j > 0) {
				output += ", ";
			} else {
				output += "  ";
			}
			output += this->scheme[j] + "=" + row[j];
		}
		output += "\n";
	}

	return output;
}