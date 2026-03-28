#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

class StringUtils {
public:
	/**
	 * @param s String to trim
	 * @return Trimmed string
	 */
	std::string trim(const std::string& s) const;
	
	/**
	 * @param s String to convert
	 * @return Lowercase string
	 */
	std::string toLower(std::string s) const;
};

#endif





