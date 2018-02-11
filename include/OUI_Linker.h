#ifndef OUI_ATTRIBUTE_LINKER_H
#define OUI_ATTRIBUTE_LINKER_H

#include <unordered_map>

namespace oui {

	//The next id for a new variable name
	int nextVariableId;

	//The map of current variable ids/names
	std::unordered_map<std::string, int> variableNameMap;

	//Gets the variable ID for the specified name
	int getVariableId(std::string name);

	//Gets the variable name for the specified ID
	//Should be avoided as it will be slowe
	std::string getVariableName(int id);

}

#endif