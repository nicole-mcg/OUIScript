#include "OUI_Linker.h"

std::vector<int, std::string> oui::variableNameMap = std::vector<int, std::string>();

int oui::getVariableId(std::string name) {
	auto it = variableNameMap.find(name);

	if (it != variableNameMap.end()) {
		return it->second;
	}

	variableNameMap.insert({nextVariableId, name});

	return nextVariableId++;
}

std::string oui::getVariableName(int id) {
	for (auto it = variableNameMap.begin(); it != variableNameMap.end(); it++) {
		if (it->second == id) {
			return it->first;
		}
	}

	return "Unknown variable";
}