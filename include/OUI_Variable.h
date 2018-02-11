#ifndef OUI_ATTRIBUTE_H
#define OUI_ATTRIBUTE_H

#include <string>

#include "OUI_includes.h"

namespace oui {

	enum VariableType {
		BOOL, INT, DOUBLE, STRING
	};

	//This class represents a variable
	struct Variable {

		//The type of variable
		private: VariableType type;
		//A pointer to the value of the variable
		private: void* value;

		public: ~Variable();

		public: Variable(Variable& attr);
		public: Variable(bool value);
		public: Variable(int value);
		public: Variable(double value);
		public: Variable(std::string value);
		public: Variable(const char* value);

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
	};

}

#endif