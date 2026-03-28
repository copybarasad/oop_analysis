#include "ConsoleCommandReader.h"

char ConsoleCommandReader::GetCommandChar() {
    std::cout << "\nCommands: ";
    for (size_t i = 0; i < commandMap_.size(); ++i) {
        const auto& [key, name] = commandMap_[i];
        std::cout << static_cast<char>(key) << " - " << name;
        if (i + 1 < commandMap_.size()) std::cout << ", ";
    }
    std::cout << "\n> ";

    char cmd;
    if (!(std::cin >> cmd)) {
        std::cin.clear();
        std::string dummy;
        std::getline(std::cin, dummy);
        return '\0';
    }


    for (const auto& [key, name] : commandMap_) {
        if (cmd == key || std::tolower(cmd) == std::tolower(key)) {
            return key;
        }
    }

    return '\0'; 
}


std::optional<size_t> ConsoleCommandReader::GetSpellIndex(const std::vector<std::unique_ptr<ISpell>>& spells) {
    if (spells.empty()) {
      std::cout << "Hand is empty.\n";
      return std::nullopt;
    }

    std::string input;
    std::cout << "Choose spell index (or 'q' to cancel): ";
    if (!(std::cin >> input)) {
      std::cin.clear();
      std::getline(std::cin, input);
      return std::nullopt;
    }

    if (input == "q") return std::nullopt;

    try {
      size_t idx = std::stoul(input);
      if (idx >= spells.size()) {
        std::cout << "Index out of range.\n";
        return std::nullopt;
      }
      return idx;
    } catch (...) {
      std::cout << "Invalid input.\n";
      return std::nullopt;
    }
  }

std::optional<Position> ConsoleCommandReader::GetTargetPosition() {
    std::string xStr, yStr;
    std::cout << "Enter target x y ('q' to cancel): ";
    if (!(std::cin >> xStr >> yStr)) {
      std::cin.clear();
      std::getline(std::cin, xStr);
      return std::nullopt;
    }
    if (xStr == "q" || yStr == "q") return std::nullopt;

    try {
      int x = std::stoi(xStr);
      int y = std::stoi(yStr);
      return Position{x, y};
    } catch (...) {
      std::cout << "Invalid coordinates.\n";
      return std::nullopt;
    }
  }

std::optional<int> ConsoleCommandReader::GetShopChoice(int playerScore) {
    std::cout << "You have " << playerScore << " points.\n";
    std::cout << "Available spells:\n";
    std::cout << "1. Fireball (direct damage, cost 20)\n";
    std::cout << "2. Meteor (AoE, cost 35)\n";
    std::cout << "3. Trap (cost 15)\n";
    std::cout << "4. Summon Ally (cost 30)\n";
    std::cout << "5. Buff (cost 20)\n";
    std::cout << "Choose (or 'q' to cancel): ";

    std::string input;
    if (!(std::cin >> input)) {
      std::cin.clear();
      std::getline(std::cin, input);
      return std::nullopt;
    }

    if (input == "q") return std::nullopt;

    try {
      int choice = std::stoi(input);
      return choice;
    } catch (...) {
      std::cout << "Invalid input.\n";
      return std::nullopt;
    }
  }

bool ConsoleCommandReader::WantsToSave() {
    std::string input;
    std::cout << "Save game? (y/n): ";
    std::cin >> input;
    return input == "y" || input == "Y";
  }
bool ConsoleCommandReader::WantsToLoad() {
    std::string input;
    std::cout << "Load game? (y/n): ";
    std::cin >> input;
    return input == "y" || input == "Y";
  }
