#include "lang/OUI_Variable.h"

#include "util/OUI_StringUtil.h"

#include "lang/OUI_Function.h"

oui::Variable::~Variable() {
}

oui::Variable::Variable() {
	type = VariableType::NONE;
	value = NULL;
}

oui::Variable::Variable(const Variable& attr) {
	type = attr.type;
	value = attr.value;
	/*switch (attr.type) {

		case VariableType::NONE:
			value = NULL;
			break;

		case VariableType::BOOL:
			value = new bool(*static_cast<bool*>(attr.value));
			break;

		case VariableType::NUMBER:
			value = new double(*static_cast<double*>(attr.value));
			break;

		case VariableType::STRING:
			value = new String(*static_cast<String*>(attr.value));
			break;

		case VariableType::ARRAY:
			std::vector<Variable*>* vec = static_cast<std::vector<Variable*>*>(attr.value);
			std::vector<Variable*>* newVec = new std::vector<Variable*>();
			for (size_t i = 0; i < vec->size(); i++) {
				newVec->push_back(new Variable(*(vec->at(i))));
			}
			value = newVec;
			break;

	}*/
}

oui::Variable::Variable(bool value) {
	type = VariableType::BOOL;
	this->value = new bool(value);
}

oui::Variable::Variable(int value) {
	type = VariableType::NUMBER;
	this->value = new double(value);
}

oui::Variable::Variable(double value) {
	type = VariableType::NUMBER;
	this->value = new double(value);
}

oui::Variable::Variable(String value) {
	type = VariableType::STRING;
	this->value = new std::u16string(value);
}

oui::Variable::Variable(const char16_t* value) {
	type = VariableType::STRING;
	this->value = new std::u16string(value);
}

oui::Variable::Variable(std::string value) {
	type = VariableType::STRING;
	this->value = new std::u16string(convertUTF8to16(value));
}

oui::Variable::Variable(const char* value) {
	type = VariableType::STRING;
	this->value = new std::u16string(convertUTF8to16(std::string(value)));
}

oui::Variable::Variable(std::vector<Variable*> value) {
	type = VariableType::ARRAY;
	auto vec = new std::vector<Variable*>();

	for (size_t i = 0; i < value.size(); i++) {
		vec->push_back(new Variable(*value[i]));
	}

	this->value = vec;
}

oui::Variable::Variable(Function* value) {
	type = VariableType::FUNCTION;
	this->value = value;
}

void oui::Variable::setValue(Variable attr) {
	setValue(attr.type, attr.value);
}

void oui::Variable::setValue(VariableType type, void* value) {
	if (value != NULL) {
		destroyValue();
	}

	this->type = type;
	this->value = value;
}

oui::VariableType oui::Variable::getType() {
	return type;
}

bool& oui::Variable::getBoolVal() {
	return *static_cast<bool*>(value);
}
double& oui::Variable::getNumberVal() {
	return *static_cast<double*>(value);
}
oui::String& oui::Variable::getStringVal() {
	return *static_cast<String*>(value);
}
std::vector<oui::Variable*>& oui::Variable::getArrayVal() {
	return *static_cast<std::vector<Variable*>*>(value);
}
oui::Function* oui::Variable::getFunctionVal() {
	return static_cast<Function*>(value);
}

void oui::Variable::destroyValue() {
	if (type == VariableType::FUNCTION) {
		Function* func = static_cast<Function*>(value);
		delete func;
		value = NULL;
	}

	if (type == VariableType::STRING) {
		String* string = static_cast<String*>(value);
		delete string;//Delete it as a String to call destructors
		value = NULL;
	}

	if (type == VariableType::ARRAY) {
		std::vector<Variable*>* vars = static_cast<std::vector<Variable*>*>(value);
		while (vars->size() > 0) {
			auto it = vars->begin();
			delete *it;
			vars->erase(it);
		}

		//Delete the pointer here because deleting a void pointer won't call destructors
		delete vars;
		value = NULL;
	}

	type = VariableType::NONE;
	if (value != NULL) {
		delete value;
		value = NULL;
	}
}

bool oui::Variable::equals(Variable& other) {
	if (type != other.type) {
		return false;
	}

	switch (type) {

		case VariableType::NONE:
			return true;

		case VariableType::BOOL:
			return getBoolVal() == other.getBoolVal();

		case VariableType::NUMBER:
			return getNumberVal() == other.getNumberVal();

		case VariableType::STRING:
			return getStringVal() == other.getStringVal();

		case VariableType::ARRAY:
			{
				std::vector<Variable*> vars = getArrayVal();
				std::vector<Variable*> otherVars = other.getArrayVal();
				if (vars.size() != otherVars.size()) {
					return false;
				}
				for (size_t i = 0; i < vars.size(); i++) {
					if (!vars[i]->equals(*(otherVars[i]))) {
						return false;
					}
				}
				return true;
			}

		case VariableType::FUNCTION:
			return value == other.value;

	}

	return false;
}

oui::String oui::Variable::toString() {
	switch (type) {
		case VariableType::BOOL:
			return getBoolVal() ? u"true" : u"false";

		case VariableType::NUMBER:
			return convertUTF8to16(std::to_string(getNumberVal()));

		case VariableType::STRING:
			return getStringVal();

		case VariableType::ARRAY:
			{
				String s = u"[";
				std::vector<Variable*> vec = getArrayVal();
				for (size_t i = 0; i < vec.size(); i++) {
					s += vec[i]->toString() + u", ";
				}
				return s + u"]";
			}

		case VariableType::FUNCTION:
			return u"function";

		case VariableType::NONE:
			return u"null";

	}
	return u"{UNKNOWN}";
}

oui::Variable oui::Variable::clone() {
	switch (type) {

		case VariableType::NONE:
			return Variable();

		case VariableType::BOOL:
			return Variable(*static_cast<bool*>(value));

		case VariableType::NUMBER:
			return Variable(*static_cast<double*>(value));

		case VariableType::STRING:
			return Variable(*static_cast<String*>(value));

		case VariableType::ARRAY:
			return Variable(*static_cast<std::vector<Variable*>*>(value));

	}

	return Variable();
}

oui::String oui::Variable::typeToString(VariableType type) {
	switch (type) {
		case VariableType::BOOL:
			return u"bool";

		case VariableType::NUMBER:
			return u"number";

		case VariableType::STRING:
			return u"string";

		case VariableType::ARRAY:
			return u"array";

		case VariableType::FUNCTION:
			return u"function";

		case VariableType::NONE:
			return u"none";
			break;
	}

	return u"UNKOWN_TYPE";
}

oui::Variable& oui::Variable::operator=(const Variable& other) {
	this->setValue(other);
	return *this;
}