#ifndef OUI_SCOPE_LOADER_H
#define OUI_SCOPE_LOADER_H

#include "OUI_Scope.h"

namespace oui {

	std::unordered_map<oui::String, oui::Scope*> loadScopes(std::string fileName, bool loadAsStatic = true);

}

#endif