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
	Relation* join_result = NULL;

	for (unsigned int i = 0; i < predicate_list.size(); i++) {
		string relation_name = predicate_list[i]->id->getExtracted();
		Relation* operand = this->database->getRelation(relation_name);
		// make sure the relation exists at this point
		if (operand == NULL) continue;

		pair<unsigned int, Relation*> query_result = this->runQuery(operand, predicate_list[i]);
		if (join_result == NULL) {
			join_result = get<1>(query_result);
		} else {
			// join the two results together
			join_result = this->database->join(join_result, get<1>(query_result));
		}
	}

	// you're going to have to get this one on another pass
	if (join_result == NULL) return false;

	// project the joined relation to the headPredicate's parameters
	vector<Parameter*> parameter_list = head_predicate->parameterList;
	vector<string> new_scheme;
	for (unsigned int i = 0; i < parameter_list.size(); i++) {
		if (parameter_list[i]->type == ID) {
			new_scheme.push_back(parameter_list[i]->value->getExtracted());
		}
	}
	Relation* project_result = this->database->project(join_result, new_scheme);

	// rename the columns to the predefined scheme
	string relation_name = head_predicate->id->getExtracted();
	Relation* original_relation = this->database->getRelation(relation_name);
	if (original_relation == NULL) {
		cout << "uninitialized relation; will result in segfault" << endl;
	}
	Relation* rename_result = this->database->rename(project_result, original_relation->scheme);

	// track rows before union
	unsigned int size_before = 0;
	if (original_relation != NULL) {
		size_before = original_relation->rows.size();
	}

	Relation* union_result = this->database->relation_union(original_relation, rename_result);
	this->database->addRelation(relation_name, union_result);

	// something changed
	if (union_result->rows.size() != size_before) return true;

	// nothing changed
	return false;
}

pair<unsigned int, Relation*> Interpreter::runQuery(Relation* relation, Predicate* query) {
	// select with the query parameters
	vector<Parameter*> parameter_list = query->parameterList;
	vector<string> values;
	for (unsigned int i = 0; i < parameter_list.size(); i++) {
		values.push_back(parameter_list[i]->value->getExtracted());
	}
	Relation* select_result = this->database->select(relation, values);

	// project the necessary columns
	vector<string> new_scheme;
	for (unsigned int i = 0; i < parameter_list.size(); i++) {
		if (parameter_list[i]->type == ID) {
			new_scheme.push_back(relation->scheme[i]);
		}
	}
	Relation* project_result = this->database->project(select_result, new_scheme);

	// rename the columns
	vector<string> scheme;
	for (unsigned int i = 0; i < parameter_list.size(); i++) {
		// only include columns of type ID
		if (parameter_list[i]->type == ID) {
			scheme.push_back(parameter_list[i]->value->getExtracted());
		}
	}
	Relation* rename_result = this->database->rename(project_result, scheme);

	return pair<unsigned int, Relation*>(select_result->rows.size(), rename_result);
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

		pair<unsigned int, Relation*> query_result = this->runQuery(relation, query);
		unsigned int select_size = get<0>(query_result);
		Relation* result = get<1>(query_result);

		// output the result
		output += queries[i]->toString() + "?";
		if (select_size == 0) {
			output += " No\n";
		} else {
			stringstream ss;
			ss << select_size;
			output += " Yes(" + ss.str() + ")\n";
		}
		if (result->rows.size() > 0) {
			output += result->toString();
		}
	}

	return output;
}