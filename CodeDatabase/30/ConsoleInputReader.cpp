#include "include/ConsoleInputReader.h"
#include "commands/GameCommand.h"
#include "commands/SwitchModeCommand.h"
#include "commands/SaveCommand.h"
#include "commands/LoadCommand.h"
#include "commands/ExitCommand.h"
#include "commands/UpgradeHealthCommand.h"
#include "commands/UpgradeDamageCommand.h"
#include "include/GameException.h"
#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <set>

ConsoleInputReader::ConsoleInputReader() {
    loadConfig("controls.txt");
}

void ConsoleInputReader::setDefaultConfig() {
    commandToKey_.clear();
    keyToCommand_.clear();

    commandToKey_["move_up"] = "w";
    commandToKey_["move_down"] = "s";
    commandToKey_["move_left"] = "a";
    commandToKey_["move_right"] = "d";
    commandToKey_["attack"] = "k";
    commandToKey_["use_spell"] = "u";
    commandToKey_["switch_mode"] = "m";
    commandToKey_["skip"] = "skip";
    commandToKey_["save"] = "save";
    commandToKey_["load"] = "load";
    commandToKey_["quit"] = "q";

    for (const auto& pair : commandToKey_) {
        keyToCommand_[pair.second] = pair.first;
    }
}

void ConsoleInputReader::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // Попытка открыть в родительской директории
        file.open("../" + filename);
        if (!file.is_open()) {
            std::cerr << "Warning: Could not open " << filename << ". Using default controls.\n";
            setDefaultConfig();
            return;
        }
    }

    std::map<std::string, std::string> tempCmdToKey;
    std::map<std::string, std::string> tempKeyToCmd;
    std::set<std::string> requiredCommands = {
        "move_up", "move_down", "move_left", "move_right",
        "attack", "use_spell", "switch_mode", "skip",
        "save", "load", "quit"
    };

    std::string cmd, key;
    while (file >> cmd >> key) {
        if (tempKeyToCmd.count(key)) {
            if (tempKeyToCmd[key] != cmd) {
                std::cerr << "Error: Key '" << key << "' assigned to multiple commands (" << tempKeyToCmd[key] << " and " << cmd << "). Using default controls.\n";
                setDefaultConfig();
                return;
            }
        }
        
        if (tempCmdToKey.count(cmd)) {
             if (tempCmdToKey[cmd] != key) {
                 std::cerr << "Error: Command '" << cmd << "' assigned to multiple keys (" << tempCmdToKey[cmd] << " and " << key << "). Using default controls.\n";
                 setDefaultConfig();
                 return;
             }
        }

        tempCmdToKey[cmd] = key;
        tempKeyToCmd[key] = cmd;
    }

    for (const auto& req : requiredCommands) {
        if (tempCmdToKey.find(req) == tempCmdToKey.end()) {
            std::cerr << "Error: Missing required command '" << req << "' in config. Using default controls.\n";
            setDefaultConfig();
            return;
        }
    }

    commandToKey_ = tempCmdToKey;
    keyToCommand_ = tempKeyToCmd;
}

std::unique_ptr<Command> ConsoleInputReader::readCommand(const GameCycle& game) {
    if (game.state() == GameCycle::GameState::WaitingForUpgrade) {
        std::cout << "\n=== LEVEL COMPLETED! ===\n";
        std::cout << "Choose your upgrade:\n";
        std::cout << "1. Upgrade Max HP (+20)\n";
        std::cout << "2. Upgrade Damage (+5)\n";
        std::cout << "> ";
        
        int choice;
        if (std::cin >> choice) {
            if (choice == 1) return std::make_unique<UpgradeHealthCommand>();
            if (choice == 2) return std::make_unique<UpgradeDamageCommand>();
        } else {
            if (std::cin.eof()) throw QuitGameException();
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cout << "Invalid choice.\n";
        PlayerCommand cmd; cmd.action = PlayerAction::Skip;
        return std::make_unique<GameCommand>(cmd);
    }

    std::cout << "\nControls:\n"
              << " [" << commandToKey_["move_up"] << "," << commandToKey_["move_left"] << "," << commandToKey_["move_down"] << "," << commandToKey_["move_right"] << "] Move\n"
              << " [" << commandToKey_["switch_mode"] << "] Switch Mode | [" << commandToKey_["skip"] << "] Skip\n"
              << " [" << commandToKey_["attack"] << " <r> <c>] Attack\n"
              << " [" << commandToKey_["use_spell"] << " <idx> <r> <c>] Use Spell\n"
              << " [" << commandToKey_["save"] << " <file>] Save | [" << commandToKey_["load"] << " <file>] Load | [" << commandToKey_["quit"] << "] Quit\n"
              << "> ";

    std::string input;
    std::cin >> input;

    if (std::cin.eof()) throw QuitGameException();

    auto it = keyToCommand_.find(input);
    if (it == keyToCommand_.end()) {
        PlayerCommand cmd; cmd.action = PlayerAction::Skip;
        return std::make_unique<GameCommand>(cmd);
    }

    std::string commandName = it->second;
    PlayerCommand cmd;
    cmd.action = PlayerAction::Skip;

    if (commandName == "move_up") { cmd.action = PlayerAction::Move; cmd.direction = Direction::Up; return std::make_unique<GameCommand>(cmd); }
    if (commandName == "move_down") { cmd.action = PlayerAction::Move; cmd.direction = Direction::Down; return std::make_unique<GameCommand>(cmd); }
    if (commandName == "move_left") { cmd.action = PlayerAction::Move; cmd.direction = Direction::Left; return std::make_unique<GameCommand>(cmd); }
    if (commandName == "move_right") { cmd.action = PlayerAction::Move; cmd.direction = Direction::Right; return std::make_unique<GameCommand>(cmd); }
    
    if (commandName == "attack") {
        cmd.action = PlayerAction::Attack;
        int r, c;
        if (std::cin >> r >> c) {
            cmd.target = Position{r, c};
            return std::make_unique<GameCommand>(cmd);
        } else {
            if (std::cin.eof()) throw QuitGameException();
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid attack arguments!\n";
            return std::make_unique<GameCommand>(cmd);
        }
    }
    
    if (commandName == "switch_mode") { 
        return std::make_unique<SwitchModeCommand>();
    }
    
    if (commandName == "use_spell") {
        cmd.action = PlayerAction::UseSpell;
        int idx, r, c;
        if (std::cin >> idx >> r >> c) {
            cmd.spellIndex = idx;
            cmd.target = Position{r, c};
            return std::make_unique<GameCommand>(cmd);
        } else {
            if (std::cin.eof()) throw QuitGameException();
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid spell arguments!\n";
            return std::make_unique<GameCommand>(cmd);
        }
    }
    
    if (commandName == "save") {
        std::string file; std::cin >> file;
        if (std::cin.eof()) throw QuitGameException();
        return std::make_unique<SaveCommand>(file);
    }
    
    if (commandName == "load") {
        std::string file; std::cin >> file;
        if (std::cin.eof()) throw QuitGameException();
        return std::make_unique<LoadCommand>(file);
    }
    
    if (commandName == "quit") {
        return std::make_unique<ExitCommand>();
    }
    
    if (commandName == "skip") {
        cmd.action = PlayerAction::Skip;
        return std::make_unique<GameCommand>(cmd);
    }

    return std::make_unique<GameCommand>(cmd);
}
