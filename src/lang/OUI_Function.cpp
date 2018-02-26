#include "lang/OUI_Function.h"


#include "util/OUI_StringUtil.h"
#include <iostream> //TODO remove

oui::Function::~Function() {
	delete scope;

	auto it = statements.begin();
	while (statements.size() > 0) {
		delete *it;
		it = statements.erase(it);
	}
}

oui::Function::Function(size_t minArgs, std::vector<int> argIds, std::vector<Statement*> statements) {
	this->minArgs = minArgs;
	this->argIds = argIds;

	this->statements = statements;

	this->scope = new Scope(VariableMap(), NULL, false);

	for (size_t i = 0; i < argIds.size(); i++) {
		scope->createVariable(argIds[i]);
	}

	this->externalVars = false;
	for (auto it = this->statements.begin(); it != this->statements.end();) {
		if ((*it)->prepareStatement(scope)) {
			delete *it;
			it = this->statements.erase(it);
			continue;
		}
		if ((*it)->containsExternalVar()) {
			externalVars = true;
		}
		it++;
	}
}

oui::Variable oui::Function::execute(std::vector<Variable> args, Scope* parentScope, bool needsReturnVal) {
	scope->setParent(parentScope);

	if (args.size() < minArgs) {
		//TODO error
	}
	
	for (size_t i = 0; i < args.size(); i++) {
		scope->setVariable(argIds[i], args[i].clone());
	}

	Variable returnVal;
	for (size_t i = 0; i < statements.size(); i++) {
		Variable var = statements[i]->evaluate(scope, needsReturnVal);
		if (needsReturnVal) {
			returnVal.setValue(var);
		} else {
			var.destroyValue();
		}
	}

	if (!needsReturnVal) {
		//TODO	This won't always clear variables resulting in
		//		Being able to use a value that hasn't been defined yet.
		//		This will be fixed when variables are made sure to be declared at compile time
		//scope->clearVariables(argIds);
	}
	return returnVal;
}

bool oui::Function::containsExternalVars() {
	return externalVars;
}
