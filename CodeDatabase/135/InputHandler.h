#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <string>

enum class UserInput {
	CONTINUE,
	PAUSE,
	INVALID
};

class InputHandler {
public:
	InputHandler() = default;
	
	void waitForEnter() const;
	void clearInputBuffer() const;
	UserInput getUserChoice() const;
};

#endif
