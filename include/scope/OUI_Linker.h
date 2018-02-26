#ifndef OUI_ATTRIBUTE_LINKER_H
#define OUI_ATTRIBUTE_LINKER_H

#include "util/OUI_Types.h"
#include "lang/OUI_Variable.h"

namespace oui {

	//The next id for a new variable name
	static int nextVariableId;

	//The map of current variable ids/names
	static std::unordered_map<String, int> variableNameMap;

	//Gets the variable ID for the specified name
	int getVariableId(String name);

	//Gets the variable name for the specified ID
	//Should be avoided as it will be slowe
	String getVariableName(int id);

	VariableMap createVariableMap(std::unordered_map<String, Variable*>& inputMap);

}

#endif