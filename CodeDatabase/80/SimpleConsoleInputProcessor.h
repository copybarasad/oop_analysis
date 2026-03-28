#ifndef SIMPLECONSOLEINPUTPROCESSOR_H
#define SIMPLECONSOLEINPUTPROCESSOR_H

#include "InputProcessor.h"
#include "CommandFactory.h"
#include "GameCommand.h"
#include <iostream>
#include <string>
#include <limits>
#include <functional>
#include <cctype>
#include <algorithm>

class SimpleConsoleInputProcessor : public InputProcessor {
private:
    CommandFactory commandFactory;
    bool isInMenuMode;
    bool isInLevelUpMode;
    bool isInLoadMenuMode;
    bool isInSaveMenuMode;

    std::string toLower(const std::string& str) const {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    std::string cleanInput(const std::string& input) const {
        size_t start = input.find_first_not_of(" \t");
        if (start == std::string::npos) {
            return "";
        }
        size_t end = input.find_last_not_of(" \t");
        return input.substr(start, end - start + 1);
    }

public:
    SimpleConsoleInputProcessor(const std::string& configFile = "controls.cfg")
        : commandFactory(configFile),
        isInMenuMode(true),
        isInLevelUpMode(false),
        isInLoadMenuMode(false),
        isInSaveMenuMode(false) {

        std::cout << "\n=== INPUT PROCESSOR INITIALIZED ===" << std::endl;
        std::cout << "Loading controls from: " << configFile << std::endl;

        std::string helpText = commandFactory.getControlHelp();
        if (helpText.find("CONTROLS") != std::string::npos) {
            std::cout << "Controls loaded successfully!" << std::endl;
        }
        else {
            std::cout << "Warning: Using default controls." << std::endl;
        }

        std::cout << "=========================" << std::endl;
    }

    void setMenuMode(bool menuMode) {
        isInMenuMode = menuMode;
        isInLevelUpMode = false;
        isInLoadMenuMode = false;
        isInSaveMenuMode = false;

        if (menuMode) {
            std::cout << "\n=== ENTERING MENU MODE ===" << std::endl;
        }
    }

    void setLevelUpMode(bool levelUpMode) {
        isInLevelUpMode = levelUpMode;
        isInMenuMode = false;
        isInLoadMenuMode = false;
        isInSaveMenuMode = false;

        if (levelUpMode) {
            std::cout << "\n=== LEVEL UP MENU ===" << std::endl;
            std::cout << "Choose an upgrade:" << std::endl;
            std::cout << "1. +10 Max Health" << std::endl;
            std::cout << "2. +5 Base Damage" << std::endl;
            std::cout << "3. Upgrade Random Spell" << std::endl;
            std::cout << "4. Add New Random Spell" << std::endl;
            std::cout << "=====================" << std::endl;
            std::cout << "Enter choice (1-4): ";
        }
    }

    void setLoadMenuMode(bool loadMenuMode) {
        isInLoadMenuMode = loadMenuMode;
        isInMenuMode = false;
        isInLevelUpMode = false;
        isInSaveMenuMode = false;
    }

    void setSaveMenuMode(bool saveMenuMode) {
        isInSaveMenuMode = saveMenuMode;
        isInMenuMode = false;
        isInLevelUpMode = false;
        isInLoadMenuMode = false;
    }

    std::unique_ptr<ICommand> getNextCommand() override {
        std::string input;
        std::getline(std::cin, input);

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return nullptr;
        }

        input = cleanInput(input);

        if (input.empty()) {
            return nullptr;
        }

        if (isInLevelUpMode) {
            if (input == "1" || input == "2" || input == "3" || input == "4") {
                return std::make_unique<GameCommand>(
                    input, 
                    "Level upgrade choice: " + input,
                    [input](Player& p, Map& m, Game& g) -> bool {
                        return true;
                    }
                );
            }
            else {
                std::cout << "Invalid choice: '" << input << "'. Please enter 1, 2, 3, or 4." << std::endl;
                return createDummyCommand("invalid_level_up");
            }
        }

        if (isInMenuMode) {
            if (input == "1") {
                return std::make_unique<GameCommand>(
                    "1",
                    "Start new game",
                    [](Player& p, Map& m, Game& g) -> bool {
                        return true;
                    }
                );
            }
            else if (input == "2") {
                return std::make_unique<GameCommand>(
                    "2",
                    "Load game",
                    [](Player& p, Map& m, Game& g) -> bool {
                        return true;
                    }
                );
            }
            else if (input == "3") {
                return std::make_unique<GameCommand>(
                    "3",
                    "Exit game",
                    [](Player& p, Map& m, Game& g) -> bool {
                        return true;
                    }
                );
            }
            else if (input == "h" || toLower(input) == "help") {
                return std::make_unique<GameCommand>(
                    "help",
                    "Show menu help",
                    [](Player& p, Map& m, Game& g) -> bool {
                        return true;
                    }
                );
            }
            else {
                auto command = commandFactory.createCommandFromInput(input);
                if (command) {
                    std::string cmdName = command->getName();

                    if (cmdName == "help" || cmdName == "show_stats") {
                        return command;
                    }
                }

                std::cout << "\nInvalid menu input: '" << input
                    << "'. Please use:\n"
                    << "  1 - Start New Game\n"
                    << "  2 - Load Game\n"
                    << "  3 - Exit\n"
                    << "  h - Show Help\n"
                    << std::endl;
                return createDummyCommand("invalid_menu_input");
            }
        }

        if (isInLoadMenuMode) {
            if (input == "0") {
                return std::make_unique<GameCommand>(
                    "cancel_load",
                    "Cancel load and return to menu",
                    [](Player& p, Map& m, Game& g) -> bool {
                        g.returnToMainMenu(); 
                        return true;
                    }
                );
            }

            bool isNumber = true;
            for (char c : input) {
                if (!std::isdigit(c)) {
                    isNumber = false;
                    break;
                }
            }

            if (isNumber) {
                int choice = std::stoi(input);

                return std::make_unique<GameCommand>(
                    "load_save_" + input,
                    "Load save slot " + input,
                    [choice](Player& p, Map& m, Game& g) -> bool {
                        auto saves = g.getSaveList();

                        if (choice > 0 && choice <= static_cast<int>(saves.size())) {
                            std::string saveName = saves[choice - 1];
                            std::cout << "Loading save: " << saveName << std::endl;

                            if (g.loadGame(saveName)) {
                                std::cout << "Game loaded successfully!" << std::endl;
                                return true;  
                            }
                            else {
                                std::cout << "Failed to load game!" << std::endl;
                                g.showMainMenu();  
                                return false;
                            }
                        }
                        else {
                            std::cout << "Invalid save slot!" << std::endl;
                            g.showMainMenu();  
                            return false;
                        }
                    }
                );
            }
            else {
                std::cout << "Please enter a number to select save slot, or 0 to cancel." << std::endl;
                return createDummyCommand("invalid_load_input");
            }
        }

        if (isInSaveMenuMode) {
            if (toLower(input) == "cancel") {
                return std::make_unique<GameCommand>(
                    "cancel",
                    "Cancel save",
                    [](Player& p, Map& m, Game& g) -> bool {
                        return true;
                    }
                );
            }
            return std::make_unique<GameCommand>(
                "save_" + input,
                "Save as: " + input,
                [input](Player& p, Map& m, Game& g) -> bool {
                    return true;
                }
            );
        }

        auto command = commandFactory.createCommandFromInput(input);

        if (!command) {
            return std::make_unique<GameCommand>(
                "unknown",
                "Unknown command: " + input,
                [input](Player& p, Map& m, Game& g) -> bool {
                    std::cout << "\nUnknown command: '" << input
                        << "'. Type 'h' for help." << std::endl;
                    return true;
                }
            );
        }

        return command;
    }

    std::unique_ptr<ICommand> createDummyCommand(const std::string& name) {
        return std::make_unique<GameCommand>(
            name,
            "Dummy command",
            [](Player& p, Map& m, Game& g) -> bool {
                return true;
            }
        );
    }

    void showPrompt(const std::string& prompt) override {
        std::cout << prompt;
        std::cout.flush();
    }

    void showMessage(const std::string& message) override {
        std::cout << message << std::endl;
    }

    void showGameHelp() {
        std::cout << commandFactory.getControlHelp();
    }

    bool reloadConfig(const std::string& configFile = "controls.cfg") {
        return commandFactory.reloadConfig(configFile);
    }

    std::string getControlHelp() const {
        return commandFactory.getControlHelp();
    }

    CommandFactory& getCommandFactory() {
        return commandFactory;
    }

    std::string getCurrentMode() const {
        if (isInLevelUpMode) return "LEVEL_UP_MODE";
        if (isInMenuMode) return "MENU_MODE";
        if (isInLoadMenuMode) return "LOAD_MENU_MODE";
        if (isInSaveMenuMode) return "SAVE_MENU_MODE";
        return "GAME_MODE";
    }
};

#endif 