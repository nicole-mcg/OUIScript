#include "OUI_Scope.h"

#include "OUI_Linker.h"

oui::Scope::Scope(VariableMap variable, Scope* parentScope = NULL, bool isStatic) {
	this->variables = variable;
	this->parentScope = parentScope;
	this->isStatic = isStatic;

	if (parentScope != NULL && parentScope->isStatic) {
		setVariables(parentScope);
	}
}

//Must be a valid id or an error is thrown
oui::Variable* oui::Scope::setVariable(int id, Variable value) {
	if (isStatic) {
		throw std::runtime_error("Tried to change variable on static sheet.");
	}

	getVariable(id)->setValue(value);
}

oui::Variable* oui::Scope::getVariable(int id) {

	//Check if this scope has the variable set
	auto it = variables.find(id);
	if (it != variables.end()) {
		return it->second;
	}

	//Check if the parent scope has it
	if (parentScope != NULL) {
		Variable* defaultAttr = parentScope->tryGetVariable(id);

		if (defaultAttr != NULL) {
			return defaultAttr;
		}
	}

	throw std::runtime_error("Referenced non-existing variable: " + getVariableName(id));
}

oui::Variable* oui::Scope::tryGetVariable(int id) {

	//Check if this scope has the variable set
	if (!isStatic) {
		auto it = variables.find(id);
		if (it != variables.end()) {
			return it->second;
		}
	}

	//Check if the parent scope has it
	if (parentScope != NULL) {
		Variable* defaultAttr = parentScope->tryGetVariable(id);

		if (defaultAttr != NULL) {
			return defaultAttr;
		}
	}

	return NULL;
}

bool oui::Scope::getBool(int id) {
	return getVariable(id)->getBoolVal();
}
int oui::Scope::getInt(int id) {
	return getVariable(id)->getIntVal();
}
double oui::Scope::getDouble(int id) {
	return getVariable(id)->getDoubleVal();
}
String oui::Scope::getString(int id) {
	return getVariable(id)->getStringVal();
}

void oui::Scope::setVariables(Scope* profile, bool overwrite) {

	for (auto it = profile->variables.begin; it != profile->variables.end(); it++) {

		//Avoid lookup if we're overwriting anyway
		if (overwrite) {
			setVariable(it->first, it->second);
			continue;
		}

		//Set value if it doesn't already exist
		Variable* existingAttr = getVariable(it->first);
		if (existingAttr == NULL) {
			setVariable(it->first, it->second);
		}

	}
}

void oui::Scope::overwriteVariables(Scope* profile) {

	for (auto it = profile->variables.begin; it != profile->variables.end(); it++) {

		//Set value if it doesn't already exist
		Variable* existingAttr = getVariable(it->first);
		if (existingAttr != NULL) {
			setVariable(it->first, it->second);
		}

	}
}
