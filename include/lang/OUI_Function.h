#ifndef OUI_FUNCTION_H
#define OUI_FUNCTION_H

#include "util/OUI_Types.h"
#include "lang/OUI_Statement.h"

namespace oui {

	class Function {
		private: Scope* scope;

		private: size_t minArgs;
		private: std::vector<int> argIds;

		private: std::vector<Statement*> statements;

		private: bool externalVars;

		public: ~Function();
		public: Function(size_t minArgs, std::vector<int> argIds, const std::vector<Statement*> statements);

		public: Variable execute(std::vector<Variable> args, Scope* parentScope, bool needsReturnVal = false);

		public: bool containsExternalVars();

	};

}

#endif