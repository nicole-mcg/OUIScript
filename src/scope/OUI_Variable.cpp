#include "scope/OUI_Variable.h"

#include "util/OUI_StringUtil.h"

#include <iostream>

oui::Variable::~Variable() {

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

	if (value != NULL) {
		delete value;
	}
}

oui::Variable::Variable(Variable& attr) {
	type = attr.type;
	switch (attr.type) {
		case VariableType::BOOL:
			value = new bool(*static_cast<bool*>(attr.value));
			break;

		case VariableType::INT:
			value = new int(*static_cast<int*>(attr.value));
			break;

		case VariableType::DOUBLE:
			value = new double(*static_cast<double*>(attr.value));
			break;

		case VariableType::STRING:
			value = new String(*static_cast<String*>(attr.value));
			break;

		case VariableType::ARRAY:
			std::vector<Variable*>* vec = static_cast<std::vector<Variable*>*>(attr.value);
			std::vector<Variable*>* newVec = new std::vector<Variable*>();
			for (unsigned int i = 0; i < vec->size(); i++) {
				newVec->push_back(new Variable(*(vec->at(i))));
			}
			value = newVec;
			break;
	}
}

oui::Variable::Variable(bool value) {
	type = VariableType::BOOL;
	this->value = new bool(value);
}

oui::Variable::Variable(int value) {
	type = VariableType::INT;
	this->value = new int(value);
}

oui::Variable::Variable(double value) {
	type = VariableType::DOUBLE;
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

	for (unsigned int i = 0; i < value.size(); i++) {
		vec->push_back(new Variable(*value[i]));
	}

	this->value = vec;
}

void oui::Variable::setValue(Variable attr) {
	setValue(attr.type, attr.value);
}

void oui::Variable::setValue(VariableType type, void* value) {
	delete value;

	this->type = type;
	this->value = value;
}

oui::VariableType oui::Variable::getType() {
	return type;
}

bool oui::Variable::getBoolVal() {
	return *static_cast<bool*>(value);
}
int oui::Variable::getIntVal() {
	return *static_cast<int*>(value);
}
double oui::Variable::getDoubleVal() {
	return *static_cast<double*>(value);
}
oui::String oui::Variable::getStringVal() {
	return *static_cast<String*>(value);
}
std::vector<oui::Variable*> oui::Variable::getArrayVal() {
	return *static_cast<std::vector<Variable*>*>(value);
}

oui::String oui::Variable::toString() {
	switch (type) {
		case VariableType::BOOL:
			return getBoolVal() ? u"true" : u"false";

		case VariableType::INT:
			return convertUTF8to16(std::to_string(getIntVal()));

		case VariableType::DOUBLE:
			return convertUTF8to16(std::to_string(getDoubleVal()));

		case VariableType::STRING:
			return getStringVal();

		case VariableType::ARRAY:
			{
				String s = u"[";
				std::vector<Variable*> vec = getArrayVal();
				for (unsigned int i = 0; i < vec.size(); i++) {
					s += vec[i]->toString() + u", ";
				}
				return s + u"]";
			}

	}
	return u"{UNKNOWN}";
}