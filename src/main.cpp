#include "scope/OUI_ScopeLoader.h"
#include "scope/OUI_Linker.h"

#include "util/OUI_StringUtil.h"

#include <iostream>

#include "lang/OUI_Function.h"

#include <thread>

double addInts(double var1, double var2) {
	return (var1 + var2) * 2;
}

int main() {

	long long start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	/*for (int i = 0; i < 1; i++) {
		std::unordered_map<oui::String, oui::Scope*> scopes = oui::loadScopes("./data/test.scp");

		std::cout << "Loaded scopes... null ? " << (scopes.size() == 0) << std::endl;

		for (auto it = scopes.begin(); it != scopes.end(); it++) {
			std::cout << "Scope: " << oui::convertUTF16to8(it->first).c_str() << std::endl;

			for (auto it2 = it->second->variables.begin(); it2 != it->second->variables.end(); it2++) {
				std::cout << "\tid= " << it2->first << " name=" << oui::convertUTF16to8(oui::getVariableName(it2->first)).c_str() << " val=" << oui::convertUTF16to8(it2->second->toString()).c_str() << std::endl;
			}
		}
	}
	std::cout << "Took " << (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - start) << "ms" << std::endl;
	*/
	int num1Id = oui::getVariableId(u"num1");
	int num2Id = oui::getVariableId(u"num2");
	int addId = oui::getVariableId(u"add");
	int addMultiplyId = oui::getVariableId(u"addMultiply");

	int resultId = oui::getVariableId(u"result");

	oui::Scope* globalScope = new oui::Scope(false);

	globalScope->createVariable(num1Id);
	globalScope->createVariable(num2Id);
	globalScope->createVariable(addId);
	globalScope->createVariable(addMultiplyId);

	globalScope->setVariable(num1Id, 5);
	globalScope->setVariable(num2Id, 10);

	std::cout << "var1=" << globalScope->getNumber(num1Id) << " var2=" << globalScope->getNumber(num2Id) << std::endl;

	globalScope->setVariable(addId,
		new oui::Function(2, {num1Id, num2Id}, {
			new oui::ArithmaticStatement({
				new oui::VariableStatement(num1Id),
				new oui::VariableStatement(num2Id)
			}, {oui::ADD})
		}
	));

	globalScope->setVariable(addMultiplyId, 
		new oui::Function(2, {num1Id, num2Id}, {
			new oui::DeclarationStatement(resultId),
			new oui::AssignmentStatement(resultId, new oui::FunctionCallStatement(addId, {
				new oui::VariableStatement(num1Id),//This may not work later when variables are found at "compile time"
				new oui::VariableStatement(num2Id)
			})),
			new oui::ArithmaticStatement({
				new oui::VariableStatement(resultId),
				new oui::ConstantStatement(2)
			}, {oui::MULTIPLY})
		})
	);

	oui::Function* add = globalScope->getVariable(addId)->getFunctionVal();
	oui::Function* addMultiply = globalScope->getVariable(addMultiplyId)->getFunctionVal();

	oui::Variable num1 = 1.75;
	oui::Variable num2 = 2.25;

	oui::Variable result = add->execute({num1, num2}, globalScope, true);

	start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	for (int i = 0; i < 100000; i++) {
		addMultiply->execute({num1, num2}, globalScope);
	}
	std::cout << "Took " << (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - start) << "ms" << std::endl;

	std::cout << "result=" << oui::convertUTF16to8(result.toString()).c_str() << std::endl;

	result.destroyValue();
	num1.destroyValue();
	num2.destroyValue();

	delete globalScope;

	system("pause");
	return 0;
}