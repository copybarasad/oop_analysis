#ifndef INPUTREADER_H
#define INPUTREADER_H

#include <memory>
#include "Command.h"

// Интерфейс для считывания ввода пользователя и преобразования в команды
class InputReader {
public:
	virtual ~InputReader() = default;
	virtual std::unique_ptr<Command> readCommand() = 0;
};

#endif 





