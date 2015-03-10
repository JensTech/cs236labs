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
	this->runRules();
}

void Interpreter::addSchemes() {
	vector<Predicate*> schemes = this->program->schemes;
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
	vector<Predicate*> facts = this->program->facts;
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

void Interpreter::runRules() {
	vector<Rule*> rules = this->program->rules;

	// keep track of the passes through rules
	int passes = 0;
	while (true) {
		bool change = false;
		for (unsigned int i = 0; i < rules.size(); i++) {
			// check if the rule changed the db
			if (runRule(rules[i])) {
				change = true;
			}
		}

		passes++;
		// break if there was no change this iteration
		if (!change) break;
	}

	// output the number of passes
	cout << "Schemes populated after " << passes << " passes through the Rules." << endl;
}

bool Interpreter::runRule(Rule* rule) {
	Predicate* head_predicate = rule->headPredicate;
	vector<Predicate*> predicate_list = rule->predicateList;

	// the result of relation joins
	Relation* join = NULL;

	for (unsigned int i = 0; i < predicate_list.size(); i++) {
		string relation_name = predicate_list[i]->id->getExtracted();
		// make sure the relation exists at this point
		if (this->database->getRelation(relation_name) == NULL) continue;

		this->runQuery(relation_name, predicate_list[i]);
		if (join == NULL) {
			join = this->database->getRelation("rename_result");
		} else {
			join = this->database->join("join", "rename_result");
		}
		this->database->addRelation("join", join);
	}

	// you're going to have to get this one on another pass
	if (join == NULL) return false;

	// project the joined relation to the headPredicate's parameters
	vector<Parameter*> parameter_list = head_predicate->parameterList;
	vector<int> indexes;
	for (unsigned int i = 0; i < join->scheme.size(); i++) {
		for (unsigned int j = 0; j < parameter_list.size(); j++) {
			// check if the value in the joined scheme is also in the head predicate scheme
			if (join->scheme[i] == parameter_list[j]->value->getExtracted()) {
				indexes.push_back(i);
			};
		}
	}
	Relation* project_result = this->database->project("join", indexes);
	this->database->addRelation("project_result", project_result);

	// rename the columns
	vector<string> scheme;
	for (unsigned int i = 0; i < head_predicate->parameterList.size(); i++) {
		scheme.push_back(head_predicate->parameterList[i]->value->getExtracted());
	}
	Relation* rename_result = this->database->rename("project_result", scheme);
	this->database->addRelation("rename_result", rename_result);

	// track rows before union
	string relation_name = head_predicate->id->getExtracted();
	Relation* original_relation = this->database->getRelation(relation_name);
	set<vector<string>> before_rows;
	if (original_relation != NULL) {
		before_rows = original_relation->rows;
	}

	Relation* union_result = NULL;
	if (original_relation == NULL) {
		union_result = rename_result;
	} else {
		union_result = this->database->relation_union(relation_name, "rename_result");
	}
	this->database->addRelation(relation_name, union_result);

	// something changed
	if (union_result->rows != before_rows) return true;

	// nothing changed
	return false;
}

void Interpreter::runQuery(string relation_name, Predicate* query) {
	// make sure that the relation exists in the database
	Relation* relation = this->database->getRelation(relation_name);
	if (relation == NULL) return;

	// select with the query parameters
	vector<Parameter*> parameter_list = query->parameterList;
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
}

string Interpreter::runQueries() {
	string output = "";

	vector<Predicate*> queries = this->program->queries;
	// loop over queries
	for (unsigned int i = 0; i < queries.size(); i++) {
		string relation_name = queries[i]->id->getExtracted();
		Predicate* query = queries[i];

		// make sure that the relation exists in the database
		Relation* relation = this->database->getRelation(relation_name);
		if (relation == NULL) continue;

		this->runQuery(relation_name, query);

		Relation* select_result = this->database->getRelation("select_result");
		// Relation* project_result = this->database->getRelation("project_result"); // unused
		Relation* rename_result = this->database->getRelation("rename_result");


		// output the result
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