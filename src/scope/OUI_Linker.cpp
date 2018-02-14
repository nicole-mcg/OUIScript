#include "scope/OUI_Linker.h"

int oui::getVariableId(String name) {
	auto it = variableNameMap.find(name);

	if (it != variableNameMap.end()) {
		return it->second;
	}

	variableNameMap.insert({name, nextVariableId});

	return nextVariableId++;
}

oui::String oui::getVariableName(int id) {
	auto it = variableNameMap.begin();
	for (; it != variableNameMap.end(); it++) {
		if (it->second == id) {
			return it->first;
		}
	}

	return u"Unknown variable";
}

oui::VariableMap oui::createVariableMap(std::unordered_map<String, Variable*>& inputMap) {
	VariableMap variableMap;

	while (inputMap.size() > 0) {
		auto it = inputMap.begin();
		variableMap.insert({getVariableId(it->first), new Variable(*(it->second))});
		delete it->second;
		inputMap.erase(it);
	}

	return variableMap;
}