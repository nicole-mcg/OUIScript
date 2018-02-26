#ifndef OUI_ATTRIBUTE_H
#define OUI_ATTRIBUTE_H

#include <string>
#include <vector>

#include "util/OUI_Types.h"

namespace oui {

	class Function;

	enum VariableType {
		NONE, BOOL, NUMBER, STRING, ARRAY, FUNCTION
	};

	//This class represents a variable
	struct Variable {

		//The type of variable
		private: VariableType type;
		//A pointer to the value of the variable
		private: void* value;

		public: ~Variable();

		public: Variable(Variable&&) = default;
		public: Variable(const Variable& attr);

		public: Variable();
		public: Variable(bool value);
		public: Variable(int value);
		public: Variable(double value);
		public: Variable(String value);
		public: Variable(const char16_t* value);
		public: Variable(std::string value);
		public: Variable(const char* value);
		public: Variable(std::vector<Variable*> value);
		public: Variable(Function* value);

		//Sets a value from a generic variable
		//Can be used with primitives
		public: void setValue(Variable attr);

		//Sets a value based on a type and a pointer to a value
		public: void setValue(VariableType type, void* value);

		public: VariableType getType();

		public: bool& getBoolVal();
		public: double& getNumberVal();
		public: String& getStringVal();
		public: std::vector<Variable*>& getArrayVal();
		public: Function* getFunctionVal();

		public: void destroyValue();

		public: bool equals(Variable& other);
		public: String toString();
		public: Variable clone();

		public: static String typeToString(VariableType type);

		public:	Variable& operator=(const Variable&);
	};

	typedef std::map<int, Variable*> VariableMap;

}

#endif