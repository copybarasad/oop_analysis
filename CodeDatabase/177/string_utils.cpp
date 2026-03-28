#include "string_utils.h"
#include <algorithm>
#include <cctype>

std::string StringUtils::trim(const std::string& s) const {
	size_t start = 0;
	while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) start++;
	size_t end = s.size();
	while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) end--;
	return s.substr(start, end - start);
}

std::string StringUtils::toLower(std::string s) const {
	std::transform(s.begin(), s.end(), s.begin(),
	               [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
	return s;
}





