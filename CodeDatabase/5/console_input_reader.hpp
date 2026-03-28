#ifndef CONSOLE_INPUT_READER_HPP
#define CONSOLE_INPUT_READER_HPP

#include "command.hpp"
#include <iostream>
#include <string>

class ConsoleInputReader {
public:
  MenuCommand readMenuCommand() const {
    std::string choice;
    std::cin >> choice;

    if (choice == "1") return MenuCommand{MenuCommandType::NewGame};
    if (choice == "2") return MenuCommand{MenuCommandType::LoadGame};
    if (choice == "3") return MenuCommand{MenuCommandType::Exit};
    return MenuCommand{MenuCommandType::Invalid};
  }

  GameOverCommand readGameOverCommand() const {
    std::string choice;
    std::cin >> choice;

    if (choice == "1") return GameOverCommand{GameOverCommandType::RestartGame};
    if (choice == "2") return GameOverCommand{GameOverCommandType::BackToMainMenu};
    if (choice == "3") return GameOverCommand{GameOverCommandType::Exit};
    return GameOverCommand{GameOverCommandType::Invalid};
  }

  LevelCommand readLevelCommand() const {
    LevelCommand cmd;
    std::string token;
    std::cin >> token;

    if (token == "w" || token == "a" || token == "s" || token == "d") {
      cmd.type = LevelCommandType::Move;
      cmd.direction = toDirection(token);
    } else if (token == "c") {
      cmd.type = LevelCommandType::SwitchStyle;
    } else if (token == "q") {
      cmd.type = LevelCommandType::QuitLevel;
    } else if (token == "b") {
      cmd.type = LevelCommandType::BuySpell;
    } else if (token == "save") {
      cmd.type = LevelCommandType::SaveGame;
    } else if (token == "m") {
      cmd.type = LevelCommandType::ShowSpells;
    } else if (token == "xw" || token == "xs" || token == "xa" || token == "xd") {
      cmd.type = LevelCommandType::RangedAttack;
      cmd.direction = toDirection(token.substr(1));
    } else if (token >= "1" && token <= "3") {
      cmd.type = LevelCommandType::UseSpell;
      cmd.spellIndex = std::stoi(token) - 1;
    } else {
      cmd.type = LevelCommandType::None;
    }

    return cmd;
  }

private:
  static Direction toDirection(const std::string &s) {
    if (s == "w") return Direction::Up;
    if (s == "s") return Direction::Down;
    if (s == "a") return Direction::Left;
    if (s == "d") return Direction::Right;
    return Direction::None;
  }
};

#endif