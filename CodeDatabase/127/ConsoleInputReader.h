#ifndef CONSOLEINPUTREADER_H
#define CONSOLEINPUTREADER_H

#include "InputReader.h"
#include "ControlConfig.h"
#include <string>
#include <memory>

// Реализация InputReader для консольного ввода
class ConsoleInputReader : public InputReader {
private:
	ControlConfig* controlConfig_;  

	std::unique_ptr<Command> parseCommand(const std::string& input);
	char translateKey(char key) const;

public:
	ConsoleInputReader(ControlConfig* controlConfig = nullptr);
	std::unique_ptr<Command> readCommand() override;
};

#endif 





