#include "scope/OUI_Scope.h"
#include "scope/OUI_Linker.h"

#include "util/OUI_StringUtil.h"

#include <iostream>//TODO remove

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

oui::Variable* oui::Scope::setVariable(int id, Variable value) {
	if (isStatic && initialized) {
		//Internal error
		std::cout << "Tried to change variable on static sheet." << std::endl;
	}

	Variable* var = tryGetVariable(id);

	if (var == NULL) {
		var = new Variable(value);
		variables.insert({id, var});
	} else {
		var->setValue(value);
	}

	return var;
}

oui::Variable* oui::Scope::getVariable(int id) {
	initialized = true;

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

	std::cout << "Referenced non-existing variable: " << convertUTF16to8(getVariableName(id)) << std::endl;
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

bool oui::Scope::getBool(int id) {
	return getVariable(id)->getBoolVal();
}
int oui::Scope::getInt(int id) {
	return getVariable(id)->getIntVal();
}
double oui::Scope::getDouble(int id) {
	return getVariable(id)->getDoubleVal();
}
oui::String oui::Scope::getString(int id) {
	return getVariable(id)->getStringVal();
}
std::vector<oui::Variable*> oui::Scope::getArray(int id) {
	return getVariable(id)->getArrayVal();
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