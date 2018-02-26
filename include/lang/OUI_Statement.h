#ifndef OUI_STATEMENT_H
#define OUI_STATEMENT_H

#include "lang/OUI_Variable.h"
#include "scope/OUI_Scope.h"

namespace oui {

	enum Operator {
		ADD, SUBTRACT, MULTIPLY, DIVIDE, COMPARE
	};

	class Statement {

		public: virtual ~Statement();
		protected: Statement();

		public: virtual Variable evaluate(Scope* scope, bool needsReturnVal);
		//Returns true if the statement has served it's purpose at compile time (E.g variable declaration)
		public: virtual bool prepareStatement(Scope* scope);

		public: virtual bool containsExternalVar();

		public: virtual bool isFunction();
		public: virtual bool isVariable();
	};

	class ConstantStatement : public Statement {
		private: Variable* value;

		public: ~ConstantStatement();
		public: ConstantStatement(Variable value);

		public: virtual Variable evaluate(Scope* scope, bool needsReturnVal);
	};

	class VariableStatement : public Statement {
		private: int variableId;
		private: Variable* variable;
		private: bool externalVar;

		//creationScope is needed if the variable wasn't created in the scope this statement is used
		public: VariableStatement(int variableId);

		public: virtual Variable evaluate(Scope* scope, bool needsReturnVal);
		public: virtual bool prepareStatement(Scope* scope);

		public: virtual bool containsExternalVar();
		public: virtual bool isVariable();
		public: virtual bool isFunction();
	};

	class AssignmentStatement : public Statement {
		private: int variableId;
		private: Variable* variable;
		private: Statement* assignmentStatement;
		private: bool externalVar;
		
		public: ~AssignmentStatement();
		//creationScope is needed if the variable wasn't created in the scope this statement is used
		public: AssignmentStatement(int variableId, Statement* assignmentStatement);

		public: virtual Variable evaluate(Scope* scope, bool needsReturnVal);
		public: virtual bool prepareStatement(Scope* scope);

		public: virtual bool containsExternalVar();
	};

	class DeclarationStatement : public Statement {
		private: int variableId;

		public: DeclarationStatement(int variableId);

		public: virtual bool prepareStatement(Scope* scope);
	};

	class FunctionCallStatement : public Statement {
		private: int functionVariableId;
		private: std::vector<Statement*> argStatements;
		private: bool externalVar;

		public: ~FunctionCallStatement();
		public: FunctionCallStatement(int functionVariableId, std::vector<Statement*> argStatements);

		public: virtual Variable evaluate(Scope* scope, bool needsReturnVal);
		public: virtual bool prepareStatement(Scope* scope);

		public: virtual bool containsExternalVar();
		public: virtual bool isFunction();
	};

	class ArithmaticStatement : public Statement {
		private: std::vector<Statement*> statements;
		private: std::vector<Operator> operators;
		private: bool externalVar;

		public: ~ArithmaticStatement();
		public: ArithmaticStatement(std::vector<Statement*> statements, std::vector<Operator> operators);

		public: virtual Variable evaluate(Scope* scope, bool needsReturnVal);
		public: virtual bool prepareStatement(Scope* scope);

		public: virtual bool containsExternalVar();
	};

}

#endif