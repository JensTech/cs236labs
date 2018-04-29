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

Relation* RelationalDatabase::select(Relation* operand, vector<string> values) {
	if (operand == NULL) return NULL;

	Relation* result = new Relation("query", operand->scheme);
	// log this relation
	this->created_relations.push_back(result);
	// loop over relation rows
	for (set<vector<string>>::iterator i = operand->rows.begin(); i != operand->rows.end(); i++) {
		bool add_row = true;
		vector<string> row = *i;
		for (unsigned int j = 0; j < values.size(); j++) {
			// make sure strings match
			if (values[j].find("'") != string::npos && values[j] != row[j]) {
				add_row = false;
				break;
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

Relation* RelationalDatabase::project(Relation* operand, vector<string> new_scheme) {
	if (operand == NULL) return NULL;

	Relation* result = new Relation("query", new_scheme);
	// log this relation
	this->created_relations.push_back(result);

	// map the old scheme onto the new one with indexes
	vector<int> operand_indexes;
	// loop over the new scheme
	for (unsigned int i = 0; i < new_scheme.size(); i++) {
		// for every position in the new scheme, find the corresponding index in the old scheme
		for (unsigned int j = 0; j < operand->scheme.size(); j++) {
			// if the scheme positions match, add the operand scheme index to the mapping
			if (operand->scheme[j] == new_scheme[i]) {
				operand_indexes.push_back(j);
			}
		}
	}

	// loop over relation rows
	for (set<vector<string>>::iterator i = operand->rows.begin(); i != operand->rows.end(); i++) {
		vector<string> operand_row = *i;
		vector<string> new_row;

		// loop over the old->new scheme mapping
		for (unsigned int j = 0; j < operand_indexes.size(); j++) {
			unsigned int operand_position = operand_indexes[j];
			new_row.push_back(operand_row[operand_position]);
		}

		// make sure the row is at all populated
		if (new_row.size() > 0) result->addRow(new_row);
	}

	return result;
}

Relation* RelationalDatabase::rename(Relation* operand, vector<string> scheme) {
	if (operand == NULL) return NULL;

	// replace the relation's scheme
	Relation* result = new Relation("query", scheme);
	// log this relation
	this->created_relations.push_back(result);
	// move the old relation's rows to the new one
	result->rows = operand->rows;

	return result;
}

Relation* RelationalDatabase::relation_union(Relation* operand1, Relation* operand2) {
	if (operand1 == NULL && operand2 == NULL) return NULL;
	if (operand1 == NULL) return operand2;
	if (operand2 == NULL) return operand1;

	// use the scheme from the first operand (an arbitrary decision, both schemes should match)
	Relation* result = new Relation("query", operand1->scheme);
	// log this relation
	this->created_relations.push_back(result);
	// another arbitrary decision (you could have started with operand2's rows)
	result->rows = operand1->rows;

	// loop over relation rows
	for (set<vector<string>>::iterator i = operand2->rows.begin(); i != operand2->rows.end(); i++) {
		vector<string> row = *i;
		result->addRow(row);
	}

	return result;
}

Relation* RelationalDatabase::join(Relation* operand1, Relation* operand2) {
	if (operand1 == NULL || operand2 == NULL) return NULL;

	// construct the new scheme
	vector<string> new_scheme = operand1->scheme;
	for (unsigned int i = 0; i < operand2->scheme.size(); i++) {
		// only include scheme elements from the second operand which are not found in the first operand
		if (find(new_scheme.begin(), new_scheme.end(), operand2->scheme[i]) == new_scheme.end()) {
			new_scheme.push_back(operand2->scheme[i]);
		}
	}

	Relation* result = new Relation("query", new_scheme);
	// log this relation
	this->created_relations.push_back(result);

	// create a map matching indexes from operand2 to operand1 (-1 means there is no match)
	vector<int> match_indexes;
	for (unsigned int i = 0; i < operand2->scheme.size(); i++) {
		// record if a match is found
		bool match_found = false;
		for (unsigned int j = 0; j < operand1->scheme.size(); j++) {
			// check for match
			if (operand2->scheme[i] == operand1->scheme[j]) {
				match_indexes.push_back(j);
				match_found = true;
				break;
			}
		}

		// -1 means there is no match
		if (!match_found) match_indexes.push_back(-1);
	}

	// loop over operand1 rows
	for (set<vector<string>>::iterator i = operand1->rows.begin(); i != operand1->rows.end(); i++) {
		vector<string> operand1_row = *i;
		// loop over operand2 rows
		for (set<vector<string>>::iterator j = operand2->rows.begin(); j != operand2->rows.end(); j++) {
			vector<string> operand2_row = *j;

			// should this row be added to the resulting relation?
			bool add_row = true;
			// store the values of the new row; initialize it with the row from the first operand
			vector<string> new_row = operand1_row;

			// loop over the second operand's row using the index map
			for (unsigned int k = 0; k < match_indexes.size(); k++) {
				if (match_indexes[k] == -1) {
					// no compare necessary
					new_row.push_back(operand2_row[k]);
				} else {
					// compare is necessary
					if (operand2_row[k] != operand1_row[match_indexes[k]]) {
						add_row = false;
						break;
					}
				}
			}

			// add the row if appropriate
			if (add_row) result->addRow(new_row);
		}
	}

	return result;
}

Relation* RelationalDatabase::getRelation(string name) {
	if (this->relations.find(name) == relations.end()) return NULL;

	return this->relations[name];
}