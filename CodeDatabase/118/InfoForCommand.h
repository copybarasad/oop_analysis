#ifndef INFO_FOR_COMMAND
#define INFO_FOR_COMMAND

#include <iostream>

struct InfoForCommand
{
public:
	std::string inputMessage;
	std::pair<int, int> rangeOfCommandParam;

	InfoForCommand(std::string msg = "", std::pair<int, int> range = std::pair(0, 0))
		: inputMessage(msg), rangeOfCommandParam(range) {
	}
};

#endif
