// Robert Williams CS 236

#include "interpreter.h"

Interpreter::Interpreter(DatalogProgram* program) {
	this->program = program;
	this->database = new RelationalDatabase();
}

Interpreter::~Interpreter() {
	delete this->database;
}

void Interpreter::buildDatabase() {
	this->addSchemes();
	this->addFacts();
}

void Interpreter::addSchemes() {
	vector<Predicate*> schemes = program->schemes;
	// loop over schemes
	for (unsigned int i = 0; i < schemes.size(); i++) {
		string relation_name = schemes[i]->id->getExtracted();
		vector<string> relation_scheme;

		// loop over parameters
		vector<Parameter*> parameter_list = schemes[i]->parameterList;
		for (unsigned int j = 0; j < parameter_list.size(); j++) {
			relation_scheme.push_back(parameter_list[j]->value->getExtracted());
		}

		// create the new relation
		Relation* relation = new Relation(relation_name, relation_scheme);

		// add the relation to the database
		this->database->addRelation(relation_name, relation);
	}
}

void Interpreter::addFacts() {
	vector<Predicate*> facts = program->facts;
	// loop over facts
	for (unsigned int i = 0; i < facts.size(); i++) {
		string relation_name = facts[i]->id->getExtracted();

		// make sure that the relation exists in the database
		Relation* relation = this->database->getRelation(relation_name);
		if (relation == NULL) continue;

		vector<string> relation_row;

		// loop over parameters
		vector<Parameter*> parameter_list = facts[i]->parameterList;
		for (unsigned int j = 0; j < parameter_list.size(); j++) {
			relation_row.push_back(parameter_list[j]->value->getExtracted());
		}

		// add the row to the relation
		relation->addRow(relation_row);
	}
}

string Interpreter::runQueries() {
	string output = "";

	vector<Predicate*> queries = program->queries;
	// loop over queries
	for (unsigned int i = 0; i < queries.size(); i++) {
		string relation_name = queries[i]->id->getExtracted();

		// make sure that the relation exists in the database
		Relation* relation = this->database->getRelation(relation_name);
		if (relation == NULL) continue;

		// select with the query parameters
		vector<Parameter*> parameter_list = queries[i]->parameterList;
		vector<string> values;
		for (unsigned int j = 0; j < parameter_list.size(); j++) {
			values.push_back(parameter_list[j]->value->getExtracted());
		}
		Relation* select_result = this->database->select(relation_name, values);
		this->database->addRelation("select_result", select_result);

		// project the necessary columns
		vector<int> indexes;
		for (unsigned int j = 0; j < parameter_list.size(); j++) {
			if (parameter_list[j]->type == ID) {
				indexes.push_back(j);
			}
		}
		Relation* project_result = this->database->project("select_result", indexes);
		this->database->addRelation("project_result", project_result);

		// rename the columns
		vector<string> scheme;
		for (unsigned int j = 0; j < parameter_list.size(); j++) {
			if (parameter_list[j]->type == ID) {
				scheme.push_back(parameter_list[j]->value->getExtracted());
			}
		}
		Relation* rename_result = this->database->rename("project_result", scheme);
		this->database->addRelation("rename_result", rename_result);


		output += queries[i]->toString() + "?";
		if (rename_result->rows.size() == 0 && select_result->rows.size() == 0) {
			output += " No\n";
		} else {
			stringstream ss;
			ss << select_result->rows.size();
			output += " Yes(" + ss.str() + ")\n";
		}
		if (rename_result->rows.size() > 0) {
			output += rename_result->toString();
		}
	}

	return output;
}