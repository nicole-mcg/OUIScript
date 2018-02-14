#ifndef OUI_ATTRIBUTE_H
#define OUI_ATTRIBUTE_H

#include <string>
#include <vector>

#include "util/OUI_Types.h"

namespace oui {

	enum VariableType {
		BOOL, INT, DOUBLE, STRING, ARRAY
	};

	//This class represents a variable
	struct Variable {

		//The type of variable
		private: VariableType type;
		//A pointer to the value of the variable
		private: void* value;

		public: ~Variable();

		public: Variable(Variable&&) = default;
		public: Variable(Variable& attr);
		public: Variable(bool value);
		public: Variable(int value);
		public: Variable(double value);
		public: Variable(String value);
		public: Variable(const char16_t* value);
		public: Variable(std::string value);
		public: Variable(const char* value);
		public: Variable(std::vector<Variable*> value);

		//Sets a value from a generic variable
		//Can be used with primitives
		public: void setValue(Variable attr);

		//Sets a value based on a type and a pointer to a value
		public: void setValue(VariableType type, void* value);

		public: VariableType getType();

		public: bool getBoolVal();
		public: int getIntVal();
		public: double getDoubleVal();
		public: String getStringVal();
		public: std::vector<Variable*> getArrayVal();

		public: String toString();
	};

	typedef std::unordered_map<int, Variable*> VariableMap;

}

#endif