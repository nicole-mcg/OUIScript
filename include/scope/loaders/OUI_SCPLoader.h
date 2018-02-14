#ifndef OUI_SCP_LOADER_H
#define OUI_SCP_LOADER_H

#include "scope/OUI_Scope.h"

namespace oui {

	std::unordered_map<String, Scope*> loadScp(String scpString, bool loadAsStatic = true);
}

#endif