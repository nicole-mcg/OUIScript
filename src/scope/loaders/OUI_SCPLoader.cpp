#include "scope/loaders/OUI_SCPLoader.h"

#include "scope/OUI_Linker.h"
#include "util/OUI_StringUtil.h"

#include <iostream> //TODO remove

std::unordered_map<oui::String, oui::Scope*> oui::loadScp(String scpString, bool loadAsStatic) {

	enum ParseState {
		NONE,
		BUILD_SCOPE_NAMES,
		LOOK_FOR_VAR_NAME,
		LOOK_FOR_VAR_VALUE,
		BUILD_STRING_LITERAL
	};

	std::unordered_map<String, Scope*> scopes;

	// Error/parsing info
	bool error = false;
	String errorMessage = u"";
	int currentLine = 1;
	int currentChar = 0;
	char16_t c;

	//Used in all states
	ParseState state = NONE;
	String token = u"";

	//Building a scope
	std::vector<String> scopeNames;
	std::unordered_map<String, Variable*> scopeVars;

	//Building a variable
	String varName;
	std::vector<Variable*> varValues;

	//String literal
	bool escapeStringLiteral = false;

	for (size_t i = 0; i < scpString.length(); i++) {
		c = scpString.at(i);

		if (c == u'\n' || c == u'\r') {
			currentLine++;
			currentChar = 0;
		}

		currentChar++;

		switch (state) {

			//Looking for anything (scope names)
			case NONE:
				if (!isCharValidForName(c)) {
					errorMessage = u"Unexpected '" + String(u"" + (char) c) + u"' while looking for scope name";
					error = true;
				} else if (!isspace(c)) {
					token = c;
					state = BUILD_SCOPE_NAMES;
				}
				break;

			//Adding scope names
			case BUILD_SCOPE_NAMES:
				{
					bool addName = false;

					if (c == u'{') {//Done getting names
						addName = true;
						state = LOOK_FOR_VAR_NAME;
					} else if (c == u',') {//Time for another name
						addName = true;
					} else if (!isCharValidForName(c)) {
						errorMessage = u"Unexpected '" + String(u"" + c) + u"' while building scope name";
						error = true;
					} else {
						token += c;
					}

					//Add the name and reset the token
					if (addName) {
						token = trimString(token);

						//Add name if it exists
						if (token != u"") {
							scopeNames.push_back(token);
							token = u"";
						} else {
							errorMessage = u"Expected a name";
							error = true;
						}

					}
				}
				break;

			//Getting the current var name
			case LOOK_FOR_VAR_NAME:
				if (c == u':') {
					token = trimString(token);

					//Set var name if it exists
					if (token != u"") {
						varName = token;
						state = LOOK_FOR_VAR_VALUE;
						token = u"";
					} else {
						errorMessage = u"Expected a variable name";
						error = true;
					}
					
				} else if (c == u'}') {
					token = trimString(token);

					if (token != u"") {
						errorMessage = u"Expected ':'";
						error = true;
					} else {

						VariableMap vars = createVariableMap(scopeVars);
						for (size_t i = 0; i < scopeNames.size(); i++) {
							Scope* scope = NULL;
							auto it = scopes.find(scopeNames[i]);

							if (it == scopes.end()) {
								scope = new Scope(loadAsStatic);
								scopes.insert({scopeNames[i], scope});
							} else {
								scope = it->second;
							}

							scope->setVariables(vars);
						}

						//Clean up VariableMap
						while (vars.size() > 0) {
							auto it = vars.begin();
							delete it->second;
							vars.erase(it);
						}
						vars.clear();

						scopeNames.clear();
						scopeVars.clear();
						state = NONE;

					}
				} else if (isspace(c)) {
					//Do nothing
				} else if (isCharValidForName(c)) {

					//Check if there is a space between characters
					if (token.length() != 0 && !isspace(c) && isspace(token[token.length() - 1])) {
						errorMessage = u"2Expected ':'";
						error = true;
					} else {
						token += c;
					}

				} else {
					errorMessage = String(u"Unexpected '").append(String(u"" + (char) c)).append(u"' while looking for variable name");
					error = true;
				}
				break;

			//Getting the current var value
			case LOOK_FOR_VAR_VALUE:
				if (c == u':') {
					errorMessage = u"Expected ';'";
					error = true;
				} else if (c == u';') {//We've collected all the values
					token = trimString(token);

					if (token != u"") {

						if (isDouble(token)) {
							varValues.push_back(new Variable(std::stod(convertUTF16to8(token))));
						} else if (isBool(token)) {
							varValues.push_back(new Variable(toBool(token)));
						} else {
							errorMessage = u"Cannot determine variable type";
							error = true;
						}
						token = u"";
					}

					Variable* var = NULL;

					if (varValues.size() == 0) {
						errorMessage = u"Expected a value";
						error = true;
					} else if (varValues.size() == 1) {
						scopeVars.insert({varName, new Variable(*varValues[0])});
					} else {
						scopeVars.insert({varName, new Variable(varValues)});
					}

					while (varValues.size() > 0) {
						auto it = varValues.begin();
						delete *it;
						varValues.erase(it);
					}
					varValues.clear();
					state = LOOK_FOR_VAR_NAME;
				} else if (c == u'"' && isspace(scpString[i - 1])) {
					state = BUILD_STRING_LITERAL;
					token = u"";
				} else if (isspace(c)) {
					token = trimString(token);

					if (token != u"") {

						if (isDouble(token)) {
							varValues.push_back(new Variable(std::stod(convertUTF16to8(token))));
						} else if (isBool(token)) {
							varValues.push_back(new Variable(toBool(token)));
						} else {
							errorMessage = u"Cannot determine variable type";
							error = true;
						}
						token = u"";
					}

				} else if (isCharValidForName(c) || c == u'.') {
					token += c;
				} else {
					errorMessage = u"Unexpected '" + String(u"" + (char) c) + u"' while looking for variable value";
					error = true;
				}
				break;

			case BUILD_STRING_LITERAL:
				if (escapeStringLiteral) {
					if (c == u'"') {
						token += u'"';
					} else if (c == u'n') {
						token += u'\n';
					} else if (c == u't') {
						token += u'\t';
					} else {
						errorMessage = u"Unknown escape sequence '\\" + String(u"" + (char) c) + u"'";
					}
				} else if (c == u'"') {
					state = LOOK_FOR_VAR_VALUE;
					Variable* var = new Variable(token);
					varValues.push_back(var);
					token = u"";
				} else if (c == u'\\') {
					escapeStringLiteral = true;
				} else {
					token += c;
				}
				break;
		
		}

		if (error) {
			std::cout << "Error: " << convertUTF16to8(errorMessage).c_str() << " Line " << std::to_string(currentLine) << " Col " << std::to_string(currentChar) << std::endl;
			return std::unordered_map<oui::String, oui::Scope*>();
		}

	}

	std::cout << "Successfully loaded scopes" << std::endl;
	return scopes;
}