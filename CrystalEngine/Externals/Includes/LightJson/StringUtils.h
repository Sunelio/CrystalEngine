#pragma once

#include <string>
#include <sstream>

namespace LightJson
{
	class StringUtils {
	public:
		static std::string cleanup(const std::string& str) {
			std::ostringstream oss;
			for (size_t i = 0; i < str.size(); i++) {
				switch (str[i]) {
				case '\b':
				case '\f':
				case '\n':
				case '\r':
				case '\t':
					break;
				case ' ':
					if (i+1 < str.size() && str[i+1] == ' ')
						break;
					[[fallthrough]];
				default:
					oss << str[i];
					break;
				}
			}
			std::string string = oss.str();
			if (!string.empty() && string[0]               == ' ') string = string.substr(1, string.size()-1);
			if (!string.empty() && string[string.size()-1] == ' ') string = string.substr(0, string.size()-1);
			return string;
		}

		static std::string removeQuotes(std::string str)
		{
			str.erase(std::remove(str.begin(), str.end(), '\"'), str.end());
			return str;
		}
	};
}