// Robert Williams CS 236

#include "relationalDatabase.h"

RelationalDatabase::RelationalDatabase() {

}

RelationalDatabase::~RelationalDatabase() {
	// make sure there are no duplicates
	for (unsigned int i = 0; i < this->created_relations.size(); i++) {
		for (unsigned int j = i + 1; j < this->created_relations.size(); j++) {
			if (this->created_relations[i] == this->created_relations[i]) {
				this->created_relations.erase(this->created_relations.begin() + j);
				j--;
			}
		}
	}

	for (unsigned int i = 0; i < this->created_relations.size(); i++) {
		delete this->created_relations[i];
	}
}

void RelationalDatabase::addRelation(string name, Relation* relation) {
	// to avoid segfaults
	if (relation == NULL) return;
	// replace duplicates
	if (this->getRelation(name) != NULL) {
		this->relations.erase(this->relations.find(name));
	}
	relation->name = name;
	this->relations.insert(pair<string, Relation*>(name, relation));
	// log this relation
	this->created_relations.push_back(relation);
}

Relation* RelationalDatabase::select(string relation, vector<string> values) {
	Relation* operand = this->getRelation(relation);
	if (operand == NULL) return NULL;

	Relation* result = new Relation("query", operand->scheme);
	// log this relation
	this->created_relations.push_back(result);
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

Relation* RelationalDatabase::project(string relation, vector<string> new_scheme) {
	Relation* operand = this->getRelation(relation);
	if (operand == NULL) return NULL;

	Relation* result = new Relation("query", new_scheme);
	// log this relation
	this->created_relations.push_back(result);
	// loop over relation rows
	for (set<vector<string>>::iterator i = operand->rows.begin(); i != operand->rows.end(); i++) {
		vector<string> operand_row = *i;
		vector<string> result_row;

		for (unsigned int j = 0; j < new_scheme.size(); j++) {
			for (unsigned int k = 0; k < operand_row.size(); k++) {
				if (operand->scheme[k] == new_scheme[j]) {
					result_row.push_back(operand_row[k]);
				}
			}
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
	// log this relation
	this->created_relations.push_back(result);
	result->rows = operand->rows;

	return result;
}

Relation* RelationalDatabase::relation_union(string relation1, string relation2) {
	Relation* operand1 = this->getRelation(relation1);
	Relation* operand2 = this->getRelation(relation2);
	if (operand1 == NULL || operand2 == NULL) {
		return NULL;
	}

	Relation* result = new Relation("query", operand1->scheme);
	// log this relation
	this->created_relations.push_back(result);
	result->rows = operand1->rows;

	// loop over relation rows
	for (set<vector<string>>::iterator i = operand2->rows.begin(); i != operand2->rows.end(); i++) {
		vector<string> row = *i;
		result->addRow(row);
	}

	return result;
}

Relation* RelationalDatabase::join(string relation1, string relation2) {
	Relation* operand1 = this->getRelation(relation1);
	Relation* operand2 = this->getRelation(relation2);
	if (operand1 == NULL || operand2 == NULL) {
		return NULL;
	}

	// construct the new scheme
	vector<string> new_scheme = operand1->scheme;
	for (unsigned int i = 0; i < operand2->scheme.size(); i++) {
		if (find(new_scheme.begin(), new_scheme.end(), operand2->scheme[i]) == new_scheme.end()) {
			new_scheme.push_back(operand2->scheme[i]);
		}
	}

	Relation* result = new Relation("query", new_scheme);
	// log this relation
	this->created_relations.push_back(result);

	// loop over operand1 rows
	for (set<vector<string>>::iterator i = operand1->rows.begin(); i != operand1->rows.end(); i++) {
		vector<string> operand1_row = *i;
		// loop over operand2 rows
		for (set<vector<string>>::iterator j = operand2->rows.begin(); j != operand2->rows.end(); j++) {
			vector<string> operand2_row = *j;

			// should this row be added to the resulting relation?
			bool add_row = true;
			// store the values of the new row
			vector<string> new_row;

			for (unsigned int row1 = 0; row1 < operand1_row.size(); row1++) {
				// add the operand1 row value to the new row
				new_row.push_back(operand1_row[row1]);
				for (unsigned int row2 = 0; row2 < operand2_row.size(); row2++) {
					
					// check if operand1 scheme matches operand2 scheme
					if (operand1->scheme[row1] == operand2->scheme[row2]) {
						if (operand1_row[row1] != operand2_row[row2]) {
							add_row = false;
							// nothing really matters anymore
							break;
						}
					} else {
						// check if it's on the last iteration
						if (row1 == operand1_row.size() - 1) {
							// add the second operand values
							new_row.push_back(operand2_row[row2]);
						}
					}

				}
				if (!add_row) break;
			}

			// add the row if appropriate
			if (add_row) result->addRow(new_row);
		}
	}

	return result;
}

Relation* RelationalDatabase::getRelation(string name) {
	if (this->relations.find(name) == relations.end()) {
		return NULL;
	}

	return this->relations[name];
}