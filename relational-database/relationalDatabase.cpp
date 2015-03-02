// Robert Williams CS 236

#include "relationalDatabase.h"

RelationalDatabase::RelationalDatabase() {

}

RelationalDatabase::~RelationalDatabase() {
	map<string, Relation*>::iterator it;
	for (it = this->relations.begin(); it != this->relations.end(); it++) {
		delete this->getRelation(it->first);
		// this->relations.erase(it);
	}
}

void RelationalDatabase::addRelation(string name, Relation* relation) {
	if (this->getRelation(name) != NULL) {
		delete this->getRelation(name);
		this->relations.erase(this->relations.find(name));
	}
	relation->name = name;
	this->relations.insert(pair<string, Relation*>(name, relation));
}

Relation* RelationalDatabase::select(string relation, vector<string> values) {
	Relation* operand = this->getRelation(relation);
	if (operand == NULL) return NULL;

	Relation* result = new Relation("query", operand->scheme);
	// loop over relation rows
	for (set<vector<string>>::iterator i = operand->rows.begin(); i != operand->rows.end(); i++) {
		bool add_row = true;
		vector<string> row = *i;
		for (unsigned int j = 0; j < values.size(); j++) {
			if (values[j].find("'") != string::npos && values[j] != row[j]) {
				add_row = false;
			}


			// make sure matching IDs also match value
			if (values[j].find("'") == string::npos) {
				for (unsigned int k = j + 1; k < values.size(); k++) {
					if (values[j] == values[k] && row[j] != row[k]) {
						add_row = false;
						break;
					}
				}
			}

			if (!add_row) break;
		}

		if (add_row) {
			result->addRow(row);
		}
	}

	return result;
}

Relation* RelationalDatabase::project(string relation, vector<int> indexes) {
	Relation* operand = this->getRelation(relation);
	if (operand == NULL) return NULL;

	Relation* result = new Relation("query", operand->scheme);
	// loop over relation rows
	for (set<vector<string>>::iterator i = operand->rows.begin(); i != operand->rows.end(); i++) {
		vector<string> operand_row = *i;
		vector<string> result_row;

		for (unsigned int j = 0; j < indexes.size(); j++) {
			result_row.push_back(operand_row[indexes[j]]);
		}

		if (result_row.size() > 0) {
			result->addRow(result_row);
		}
	}

	return result;
}

Relation* RelationalDatabase::rename(string relation, vector<string> scheme) {
	Relation* operand = this->getRelation(relation);
	if (operand == NULL) return NULL;

	Relation* result = new Relation("query", scheme);
	result->rows = operand->rows;

	return result;
}

Relation* RelationalDatabase::getRelation(string name) {
	if (this->relations.find(name) == relations.end()) {
		return NULL;
	}

	return this->relations[name];
}