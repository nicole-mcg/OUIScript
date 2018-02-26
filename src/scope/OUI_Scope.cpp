#include "scope/OUI_Scope.h"
#include "scope/OUI_Linker.h"

#include <algorithm>

#include "util/OUI_StringUtil.h"//TODO remove?
#include <iostream>//TODO remove

oui::Scope::~Scope() {
	auto it = variables.begin();
	while (variables.size() > 0) {
		it->second->destroyValue();
		delete it->second;
		it = variables.erase(it);
	}
	variables.clear();
	parentScope = NULL;
}

oui::Scope::Scope(bool isStatic) {
	this->variables = VariableMap();
	this->isStatic = isStatic;
	this->parentScope = NULL;

	this->initialized = false;
}

oui::Scope::Scope(VariableMap variable, Scope* parentScope, bool isStatic) {
	this->variables = variable;
	this->parentScope = parentScope;
	this->isStatic = isStatic;

	if (parentScope != NULL && parentScope->isStatic) {
		//Static variables won't change their values so we can set them now
		setVariables(parentScope);
	}

	initialized = true;
}

void oui::Scope::setParent(Scope* parentScope) {
	this->parentScope = parentScope;
}

oui::Variable* oui::Scope::createVariable(int id, Variable value) {

	auto it = variables.find(id);
	if (it != variables.end()) {
		std::cout << "Variable already exists" << std::endl;
	}

	Variable* var = new Variable(value);
	variables.insert(variables.upper_bound(id), {id, var});
	return var;
}

oui::Variable* oui::Scope::setVariable(int id, Variable value) {
	if (isStatic && initialized) {
		//Internal error
		std::cout << "Tried to change variable on static sheet." << std::endl;
	}

	Variable* var = NULL;
	auto it = variables.find(id);
	if (it == variables.end()) {
		std::cout << "Variable doesn't exist" << std::endl;
	}

	var = it->second;
	var->setValue(value);

	return var;
}

oui::Variable* oui::Scope::getVariable(int id) {
	initialized = true;

	//Check if this scope has the variable set
	auto it = variables.lower_bound(id);
	if (it != variables.end() && it->first == id) {
		return it->second;
	}

	//Check if the parent scope has it
	if (parentScope != NULL) {
		Variable* defaultAttr = parentScope->tryGetVariable(id);

		if (defaultAttr != NULL) {
			return defaultAttr;
		}
	}

	std::cout << "Referenced non-existing variable: " << convertUTF16to8(getVariableName(id)) << std::endl;
	return NULL;
}

oui::Variable* oui::Scope::tryGetVariable(int id) {

	//Check if this scope has the variable set
	//Don't check if this scope is static (as the child scope should already have these variables)
	//if (!isStatic) {
		auto it = variables.find(id);
		if (it != variables.end()) {
			return it->second;
		}
	//}

	//Check if the parent scope has it
	if (parentScope != NULL) {
		Variable* defaultAttr = parentScope->tryGetVariable(id);

		if (defaultAttr != NULL) {
			return defaultAttr;
		}
	}

	return NULL;
}

bool oui::Scope::containsVariable(int variableId) {
	auto it = variables.lower_bound(variableId);
	return it != variables.end();
}

bool oui::Scope::getBool(int id) {
	return getVariable(id)->getBoolVal();
}
double oui::Scope::getNumber(int id) {
	return getVariable(id)->getNumberVal();
}
oui::String oui::Scope::getString(int id) {
	return getVariable(id)->getStringVal();
}
std::vector<oui::Variable*> oui::Scope::getArray(int id) {
	return getVariable(id)->getArrayVal();
}
oui::Function* oui::Scope::getFunction(int id) {
	return getVariable(id)->getFunctionVal();
}

void oui::Scope::setVariables(Scope* profile, bool overwrite) {
	setVariables(profile->variables, overwrite);
}

void oui::Scope::setVariables(VariableMap variableMap, bool overwrite) {

	for (auto it = variableMap.begin(); it != variableMap.end(); it++) {

		//Avoid lookup if we're overwriting anyway
		if (overwrite) {
			setVariable(it->first, *(it->second));
			continue;
		}

		//Set value if it doesn't already exist
		Variable* existingAttr = getVariable(it->first);
		if (existingAttr == NULL) {
			setVariable(it->first, *(it->second));
		}

	}
}

void oui::Scope::overwriteVariables(Scope* profile) {

	for (auto it = profile->variables.begin(); it != profile->variables.end(); it++) {

		Variable* existingAttr = getVariable(it->first);

		//Set value only if it already exist
		if (existingAttr != NULL) {
			setVariable(it->first, *(it->second));
		}

	}
}

void oui::Scope::clearVariables(std::vector<int> exceptions) {
	auto it = variables.begin();

	size_t numExceptions = 0;
	while (variables.size() > numExceptions) {
		
		if (std::find(exceptions.begin(), exceptions.end(), it->first) != exceptions.end()) {
			numExceptions++;
			it++;
		} else {
			it->second->destroyValue();
			delete it->second;
			it = variables.erase(it);
		}
	}
}