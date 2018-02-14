#include "scope/OUI_ScopeLoader.h"

#include <fstream>
#include <streambuf>

#include "scope/loaders/OUI_SCPLoader.h"

#include <iostream>

std::unordered_map<oui::String, oui::Scope*> oui::loadScopes(std::string fileName, bool loadAsStatic) {

	std::basic_ifstream<wchar_t, std::char_traits<wchar_t>> stream(fileName);
	if (!stream.good()) {//File doesnt exist
		return std::unordered_map<oui::String, oui::Scope*>();
	}
	String str((std::istreambuf_iterator<wchar_t>(stream)),
					std::istreambuf_iterator<wchar_t>());//Load file
	stream.close();

	std::cout << "len=" << str.length() << std::endl;
	for (int i = 0; i < str.length(); i++) {
		std::cout << (char) str[i];
	}
	std::cout << std::endl;

	return loadScp(str);
}