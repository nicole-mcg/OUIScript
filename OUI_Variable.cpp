#include "OUI_Variable.h"

oui::Variable::~Variable() {
	delete value;
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
			value = new std::string(*static_cast<std::string*>(attr.value));
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

oui::Variable::Variable(std::string value) {
	type = VariableType::STRING;
	this->value = new std::string(value);
}

oui::Variable::Variable(const char* value) {
	type = VariableType::STRING;
	this->value = new std::string(value);
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
String oui::Variable::getStringVal() {
	return *static_cast<String*>(value);
}