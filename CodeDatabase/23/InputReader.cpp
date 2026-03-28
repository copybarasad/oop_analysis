#include "InputReader.h"
#include "GameManager.h"
#include <iostream>

ConsoleInputReader::ConsoleInputReader(GameManager *manager) { (void)manager; }

ConsoleInputReader::ConsoleInputReader(GameManager *manager,
                                       const std::string &configFile)
    : bindings(configFile) {
  (void)manager;
}

std::unique_ptr<Command> ConsoleInputReader::readCommand() {
  std::string input;
  std::cin >> input;

  if (std::cin.fail() || std::cin.eof()) {
    return nullptr;
  }

  CommandType cmdType = bindings.getCommandType(input);

  switch (cmdType) {
  case CommandType::MOVE_UP:
    return std::make_unique<MoveCommand>(0, -1);
  case CommandType::MOVE_LEFT:
    return std::make_unique<MoveCommand>(-1, 0);
  case CommandType::MOVE_DOWN:
    return std::make_unique<MoveCommand>(0, 1);
  case CommandType::MOVE_RIGHT:
    return std::make_unique<MoveCommand>(1, 0);
  case CommandType::ATTACK_LEFT:
    return std::make_unique<AttackCommand>(-1, 0);
  case CommandType::ATTACK_UP:
    return std::make_unique<AttackCommand>(0, -1);
  case CommandType::ATTACK_RIGHT:
    return std::make_unique<AttackCommand>(1, 0);
  case CommandType::ATTACK_DOWN:
    return std::make_unique<AttackCommand>(0, 1);
  case CommandType::SWITCH_ATTACK:
    return std::make_unique<SwitchAttackCommand>();
  case CommandType::CAST_SPELL:
    return std::make_unique<CastSpellCommand>();
  case CommandType::SAVE:
    return std::make_unique<SaveCommand>();
  case CommandType::SKIP:
    return std::make_unique<SkipCommand>();
  case CommandType::HELP:
    return std::make_unique<HelpCommand>();
  case CommandType::QUIT:
    return std::make_unique<QuitCommand>();
  default:
    std::cout << "Unknown command. Type 'help' for available commands."
              << std::endl;
    return nullptr;
  }
}
