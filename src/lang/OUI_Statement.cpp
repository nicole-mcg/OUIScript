#include "lang/OUI_Statement.h"

#include "lang/OUI_Function.h"

#include "util\/OUI_StringUtil.h"//TODO remove

//BASE CLASS

oui::Statement::~Statement() {
	
}
oui::Statement::Statement() {
	//TODO error?
}
oui::Variable oui::Statement::evaluate(Scope* scope, bool needsReturnVal) {
	return Variable();
}
bool oui::Statement::prepareStatement(Scope* scope) {
	return false;
}
bool oui::Statement::containsExternalVar() {
	return false;
}
bool oui::Statement::isVariable() {
	return false;
}
bool oui::Statement::isFunction() {
	return false;
}

//CONSTANT STATEMENT

oui::ConstantStatement::~ConstantStatement() {
	value->destroyValue();
	delete value;
}
oui::ConstantStatement::ConstantStatement(Variable value) {
	this->value = new Variable(value);
}
oui::Variable oui::ConstantStatement::evaluate(Scope* scope, bool needsReturnVal) {
	return value->clone();
}

//VARIABLE STATEMENT

oui::VariableStatement::VariableStatement(int variableId) {
	this->variableId = variableId;
	this->variable = NULL;
}
oui::Variable oui::VariableStatement::evaluate(Scope* scope, bool needsReturnVal) {
	return variable->clone();
}
bool oui::VariableStatement::prepareStatement(Scope* scope) {
	this->variable = scope->getVariable(variableId);
	this->externalVar = !scope->containsVariable(variableId);

	if (variable->getType() == VariableType::FUNCTION) {
		this->externalVar |= variable->getFunctionVal()->containsExternalVars();
	}

	return false;
}
bool oui::VariableStatement::containsExternalVar() {
	return externalVar;
}
bool oui::VariableStatement::isVariable() {
	return true;
}
bool oui::VariableStatement::isFunction() {
	if (variable == NULL) {
		return false;
	}
	return variable->getType() == VariableType::FUNCTION;
}

//DECLARATION STATEMENT

oui::DeclarationStatement::DeclarationStatement(int variableId) {
	this->variableId = variableId;
}
bool oui::DeclarationStatement::prepareStatement(Scope* scope) {
	scope->createVariable(variableId);
	return true;
}

//ASSIGNMENT STATEMENT

oui::AssignmentStatement::~AssignmentStatement() {
	delete assignmentStatement;
}
oui::AssignmentStatement::AssignmentStatement(int variableId, Statement* assignmentStatement) {
	this->variableId = variableId;
	this->variable = NULL;
	this->assignmentStatement = assignmentStatement;
	this->externalVar = assignmentStatement->containsExternalVar(); //???
}
oui::Variable oui::AssignmentStatement::evaluate(Scope* scope, bool needsReturnVal) {
	Variable value = assignmentStatement->evaluate(scope, true);
	scope->setVariable(variableId, value);
	return value.clone();
}
bool oui::AssignmentStatement::prepareStatement(Scope* scope) {
	assignmentStatement->prepareStatement(scope);
	//Variable var = assignmentStatement->
	if (assignmentStatement->isVariable() && assignmentStatement->isFunction() && assignmentStatement->containsExternalVar()) {
		//TODO error
		std::cout << "Cannot assign a function that contains external variables." << std::endl;
	}

	this->variable = scope->getVariable(variableId);
	this->externalVar = !scope->containsVariable(variableId);

	return false;
}
bool oui::AssignmentStatement::containsExternalVar() {
	return externalVar;
}

//FUNCTION CALL STATEMENT

oui::FunctionCallStatement::~FunctionCallStatement() {
	auto it = argStatements.begin();
	while (argStatements.size() > 0) {
		delete *it;
		it = argStatements.erase(it);
	}
}
oui::FunctionCallStatement::FunctionCallStatement(int functionVariableId, std::vector<Statement*> argStatements) {
	this->functionVariableId = functionVariableId;
	this->argStatements = argStatements;
	this->externalVar = false;
	for (size_t i = 0; i < argStatements.size(); i++) {
		if (argStatements[i]->containsExternalVar()) {
			this->externalVar = true;
		}
	}
}
oui::Variable oui::FunctionCallStatement::evaluate(Scope* scope, bool needsReturnVal) {
	std::vector<Variable> args;
	for (size_t i = 0; i < argStatements.size(); i++) {
		args.push_back(argStatements[i]->evaluate(scope, true));
	}

	Function* func = scope->getFunction(functionVariableId);
	Variable ret = func->execute(args, scope, true);

	auto it = args.begin();
	while (args.size() > 0) {
		it->destroyValue();
		it = args.erase(it);
	}

	return ret;
}
bool oui::FunctionCallStatement::prepareStatement(Scope* scope) {
	for (size_t i = 0; i < argStatements.size(); i++) {
		argStatements[i]->prepareStatement(scope);
	}
	return false;
}
bool oui::FunctionCallStatement::containsExternalVar() {
	return externalVar;
}
bool oui::FunctionCallStatement::isFunction() {
	return true;
}

//ARITHMATIC STATEMENT

oui::ArithmaticStatement::~ArithmaticStatement() {
	auto it = statements.begin();
	while (statements.size() > 0) {
		delete *it;
		it = statements.erase(it);
	}
}
oui::ArithmaticStatement::ArithmaticStatement(std::vector<Statement*> statements, std::vector<Operator> operators) {
	if (statements.size() < 2 || operators.size() == 0) {
		//TODO error
		std::cout << "Arithmatic statement must have at least 2 statements and 1 operator";
	}
	this->statements = statements;
	this->operators = operators;

	this->externalVar = false;
	for (size_t i = 0; i < statements.size(); i++) {
		if (statements[i]->containsExternalVar()) {
			this->externalVar = true;
		}
	}
}
oui::Variable oui::ArithmaticStatement::evaluate(Scope* scope, bool needsReturnVal) {
	size_t opI = 0;
	size_t stI = 0;

	Variable nextValue = statements[stI++]->evaluate(scope, true);
	Variable currValue = nextValue.clone();
	
	if (currValue.getType() == VariableType::FUNCTION) {
		//TODO error
		std::cout << "Cannot perform operations on functions" << std::endl;
	}

	while (stI < statements.size()) {
		nextValue.setValue(statements[stI++]->evaluate(scope, true));
		Operator op = operators[opI++];

		if (nextValue.getType() == VariableType::FUNCTION
			|| (op != Operator::COMPARE
				&& (currValue.getType() == VariableType::BOOL || currValue.getType() == VariableType::NONE
					|| nextValue.getType() == VariableType::BOOL || nextValue.getType() == VariableType::NONE))) {
			//TODO error
			std::cout << "Cannot perform mathmatic operations on null, bool, or functions. " << std::endl;
		}
	

		
		switch (op) {

			case ADD:
				
				if (currValue.getType() == VariableType::STRING || nextValue.getType() == VariableType::STRING) {
					currValue.setValue(currValue.getStringVal().append(nextValue.getStringVal()));
				} else if (currValue.getType() == VariableType::NUMBER && nextValue.getType() == VariableType::NUMBER) {
					currValue.setValue(currValue.getNumberVal() + nextValue.getNumberVal());
				} else {
					std::cout << "Cannot add " 
						<< convertUTF16to8(Variable::typeToString(currValue.getType())).c_str() 
						<< " and " 
						<< convertUTF16to8(Variable::typeToString(nextValue.getType())).c_str()
						<< std::endl;
				}

				break;

			case SUBTRACT:
				if (currValue.getType() == VariableType::NUMBER && nextValue.getType() == VariableType::NUMBER) {
					currValue.setValue(currValue.getNumberVal() - nextValue.getNumberVal());
				} else {
					std::cout << "Cannot subtract "
						<< convertUTF16to8(Variable::typeToString(nextValue.getType())).c_str()
						<< " from "
						<< convertUTF16to8(Variable::typeToString(currValue.getType())).c_str()
						<< std::endl;
				}
				break;

			case MULTIPLY:
				if (currValue.getType() == VariableType::NUMBER && nextValue.getType() == VariableType::NUMBER) {
					//Multiple numbers
					currValue.setValue(currValue.getNumberVal() * nextValue.getNumberVal());
				} else {
					//We're either multiplying strings or we've got an error

					int amount = 0;
					String string;
					if (currValue.getType() == VariableType::NUMBER && nextValue.getType() == VariableType::STRING) {
						amount = (int) currValue.getNumberVal();
						string = nextValue.getStringVal();
					} else if (currValue.getType() == VariableType::STRING && nextValue.getType() == VariableType::NUMBER) {
						amount = (int) nextValue.getNumberVal();
						string = currValue.getStringVal();
					} else {
						std::cout << "Cannot subtract "
							<< convertUTF16to8(Variable::typeToString(nextValue.getType())).c_str()
							<< " from "
							<< convertUTF16to8(Variable::typeToString(currValue.getType())).c_str()
							<< std::endl;
						//return; //TODO this must return when an error system is created because it will attempt to multiply a string otherwise
					}

					//Multiply strings
					String newString = u"";
					for (int i = 0; i < amount; i++) {
						newString += string;
					}
					currValue.setValue(newString);
				}
				break;

			case DIVIDE:
				if (currValue.getType() == VariableType::NUMBER && currValue.getType() == VariableType::NUMBER) {
					currValue.setValue(currValue.getNumberVal() / nextValue.getNumberVal());
				} else {
					std::cout << "Cannot divide "
						<< convertUTF16to8(Variable::typeToString(currValue.getType())).c_str()
						<< " by "
						<< convertUTF16to8(Variable::typeToString(nextValue.getType())).c_str()
						<< std::endl;
				}
				break;
		}
	}


	nextValue.destroyValue();
	//TODO this should be uncommented, there is a memory leak
	//currValue.destroyValue();

	return currValue;
}
bool oui::ArithmaticStatement::prepareStatement(Scope* scope) {
	for (size_t i = 0; i < statements.size(); i++) {
		statements[i]->prepareStatement(scope);
	 }
	return false;
}
bool oui::ArithmaticStatement::containsExternalVar() {
	return externalVar;
}