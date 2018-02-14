#ifndef OUI_ATTRIBUTE_PROFILE_H
#define OUI_ATTRIBUTE_PROFILE_H

#include "OUI_Variable.h"

namespace oui {

	

	//This class can be use by any container that will store sets of variables
	//
	//Class Template:
	//		parentScope should be the global scope, or the parent class template for inheritence
	//		isStatic should be true
	//
	//Class Instance:
	//		parentScope should be the template for the class
	//		isStatic should be false
	//
	//Generic Scope:
	//		parentScope should be the global scope, or the scope this was created in
	//		isStatic should be false
	//		When a child scope is destroyed, parent.overwriteVariables(child) should be called so parent variables are updated
	//		
	struct Scope {

		private: bool initialized;

		//If a variable doesn't exist in this scope, the parent scope is checked recursively
		private: Scope* parentScope;

		//If true, the variables in this scope can't be changed
		//Useful for templates, just as a classes type
		private: bool isStatic;

		//The variables in this scope
		public: VariableMap variables;

		//Create a static scope that can be changed until the first value is read
		public: Scope(bool isStatic = true);

		//attributeMap:	The starting attributes of the scope
		//parentScope:	The scope to look for variables if they don't exist. Values are applied instantly if parentScope is static
		//isStatic:		If true, the variables in this scope cannot be changed. E.g: A class type with it's default variable values
		public: Scope(VariableMap variableMap, Scope* parentScope = NULL, bool isStatic = true);

		//Must be a valid id or an error is thrown
		public: Variable* setVariable(int id, Variable value);

		//Must be a valid id or an error is thrown
		public: Variable* getVariable(int id);

		//Ignores existing variable checks in static scopes
		private: Variable* tryGetVariable(int id);
		
		public: bool getBool(int id);
		public: int getInt(int id);
		public: double getDouble(int id);
		public: String getString(int id);
		public: std::vector<Variable*> getArray(int id);
		
		//Used to apply variables from one scope to another
		public: void setVariables(Scope* profile, bool overwrite = true);
		//Used to apply variables from one scope to another
		public: void setVariables(VariableMap variableMap, bool overwrite = true);

		//Used to overwrite only with variables that exist in both in both scopes
		public: void overwriteVariables(Scope* profile);
	};

}

#endif