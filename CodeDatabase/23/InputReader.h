#ifndef INPUT_READER_H
#define INPUT_READER_H

#include "Command.h"
#include "KeyBindings.h"
#include <memory>
#include <string>

class GameManager;

class ConsoleInputReader {
private:
  KeyBindings bindings;

public:
  ConsoleInputReader(GameManager *manager);
  ConsoleInputReader(GameManager *manager, const std::string &configFile);

  std::unique_ptr<Command> readCommand();

  const KeyBindings &getBindings() const { return bindings; }
};

#endif 
