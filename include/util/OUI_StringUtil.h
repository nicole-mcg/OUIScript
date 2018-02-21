#ifndef OUI_STRING_UTIL_H
#define OUI_STRING_UTIL_H

#include "util/OUI_Types.h"

#include <iostream>//TODO remove

namespace oui {

#if _MSC_VER >= 1900

	inline std::string convertUTF16to8(std::u16string string) {
		std::string s = "";
		for (unsigned int i = 0; i < string.length(); i++) {
			s += (char) string[i];
		}
		return s;
	}

	inline String convertUTF8to16(std::string string) {
		String s = u"";
		for (unsigned int i = 0; i < string.length(); i++) {
			s += (char16_t) string[i];
		}
		return s;
	}

	inline String toLower(const String& string) {
		String lower = String(string);
		for (unsigned int i = 0; i < string.length(); i++) {
			if (string[i] < 255) {
				lower[i] = (char16_t) tolower((char) string[i]);
			}
		}
		return lower;
	}

#else

	/*#include <locale>
	#include <codecvt>

	inline std::string convertUTF16to8(String string) {
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
		return convert.to_bytes(string);
	}

	inline String convertUTF8to16(std::string string) {
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
		return  convert.from_bytes(string);
	}

	inline String toLower(const String& string) {
		String data(string);
		std::use_facet<std::ctype<char16_t>>(std::locale()).tolower(&data[0], &data[0] + data.size());
		return data;
	}*/

#endif

	

	inline String trimString(String& string) {
		if (string.length() == 0) {
			return u"";
		}

		int start, end;

		start = 0;
		for (unsigned int i = 0; i < string.length(); i++) {
			if (!isspace(string.at(i))) {
				start = i;
				break;
			}
		}

		end = string.length() - 1;
		for (unsigned int i = string.length() - 1; i >= 0; i--) {
			if (!isspace(string.at(i))) {
				end = i + 1;
				break;
			}
		}

		return string.substr(start, end - start);
	}

	inline bool isInteger(const String& s) {
		//https://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int

		if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) {
			return false;
		}

		unsigned int i = 0;
		if (s[i] == '-' || s[i] == '+') {
			i++;
		}

		for (; i < s.length(); i++) {
			if (!isdigit(s[i])) {
				return false;
			}
		}

		return true;
	}

	inline bool isDouble(const String& s) {

		if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+') && s[0] != '.')) {
			return false;
		}

		bool foundDecimal = false;

		unsigned int i = 0;
		if (s[i] == '-' || s[i] == '+') {
			i++;
		}

		for (; i < s.length(); i++) {

			//Look for decimal
			if (s[i] == '.') {
				if (foundDecimal) {
					return false;
				}

				foundDecimal = true;
			} else if (!isdigit(s[i])) {
				return false;
			}
		}

		return true;
	}

	inline bool isBool(const String& s) {
		String lower = toLower(s);
		return lower == u"true" || lower == u"false";
	}

	inline bool toBool(const String& s) {
		String lower = toLower(s);
		if (lower == u"true") {
			return true;
		} else if (lower == u"false") {
			return false;
		}
		throw std::runtime_error("String is not a bool value");
	}

	inline bool isCharValidForName(char16_t c) {
		return c != u'{' && c != u'}' 
			&& c != u',' && c != u'.'
			&& c != u';' && c != u':' 
			&& c != u'"' && c != u'\'' 
			&& c != u'\\';
	}
}

#endif