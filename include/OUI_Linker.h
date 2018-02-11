#ifndef OUI_ATTRIBUTE_LINKER_H
#define OUI_ATTRIBUTE_LINKER_H

#include "OUI_includes.h"

namespace oui {

	int nextVariableId;
	std::unordered_map<std::string, int> variableNameMap;

	int getVariableId(std::string name);
	std::string getVariableName(int id);

}

#endif
