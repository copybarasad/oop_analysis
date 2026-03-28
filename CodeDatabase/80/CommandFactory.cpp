#include "CommandFactory.h"
#include "GameCommand.h"
#include "Player.h"
#include "Map.h"
#include "Game.h"
#include <iostream>

bool moveUp(Player& player, Map& map, Game& game) {
    player.Step('u', map);
    std::cout << "Moved up" << std::endl;
    return true;
}

bool moveLeft(Player& player, Map& map, Game& game) {
    player.Step('l', map);
    std::cout << "Moved left" << std::endl;
    return true;
}

bool moveDown(Player& player, Map& map, Game& game) {
    player.Step('d', map);
    std::cout << "Moved down" << std::endl;
    return true;
}

bool moveRight(Player& player, Map& map, Game& game) {
    player.Step('r', map);
    std::cout << "Moved right" << std::endl;
    return true;
}

bool attack(Player& player, Map& map, Game& game) {
    player.Attack(map);
    std::cout << "Attacked!" << std::endl;
    return true;
}

bool useSpell(Player& player, Map& map, Game& game) {
    player.UseSpell(map);
    std::cout << "Spell used!" << std::endl;
    return true;
}

bool changeMode(Player& player, Map& map, Game& game) {
    player.ChangeMode();
    std::cout << "Mode changed!" << std::endl;
    return true;
}

bool saveGame(Player& player, Map& map, Game& game) {
    game.SaveGameMenu();
    return true;
}


bool showHelp(Player& player, Map& map, Game& game) {
    std::cout << "Available commands:" << std::endl;
    std::cout << "  w - Move up" << std::endl;
    std::cout << "  a - Move left" << std::endl;
    std::cout << "  s - Move down" << std::endl;
    std::cout << "  d - Move right" << std::endl;
    std::cout << "  e - Attack" << std::endl;
    std::cout << "  q - Use spell" << std::endl;
    std::cout << "  r - Change attack mode" << std::endl;
    std::cout << "  p - Save game" << std::endl;
    std::cout << "  0 - Exit to menu" << std::endl;
    std::cout << "  h - Show this help" << std::endl;
    std::cout << "  u - Show player stats" << std::endl;
    return true;
}

bool showStats(Player& player, Map& map, Game& game) {
    std::cout << "=== PLAYER STATS ===" << std::endl;
    std::cout << "HP: " << player.heallpoints << "/"
        << player.GetMaxHealth() << std::endl;
    std::cout << "Damage: " << player.GetBaseDamage() << std::endl;
    std::cout << "Spells: " << player.GetSpellCount() << std::endl;
    std::cout << "Defeated: " << player.GetDefeatedCount() << std::endl;
    return true;
}
bool exitToMenu(Player& player, Map& map, Game& game) {
    game.setInMainMenu(true);
    game.setGameRunning(false);
    return false;  
}


CommandFactory::CommandFactory() {
    if (!controlConfig.loadFromFile("controls.cfg")) {
        std::cout << "Using default control configuration." << std::endl;
    }

    registerAllCommands();
}

CommandFactory::CommandFactory(const std::string& configFile) {
    if (!controlConfig.loadFromFile(configFile)) {
        std::cout << "Using default control configuration." << std::endl;
    }
    registerAllCommands();
}

void CommandFactory::registerAllCommands() {
    registerCommand("move_up", "Move character up", moveUp);
    registerCommand("move_left", "Move character left", moveLeft);
    registerCommand("move_down", "Move character down", moveDown);
    registerCommand("move_right", "Move character right", moveRight);

    registerCommand("attack", "Attack enemy", attack);
    registerCommand("cast_spell", "Cast a spell", useSpell);
    registerCommand("change_mode", "Change attack mode", changeMode);

    registerCommand("exit_to_menu", "Exit to main menu", exitToMenu);
    registerCommand("save_game", "Save current game", saveGame);
    registerCommand("help", "Show help", showHelp);
    registerCommand("show_stats", "Show player statistics", showStats);
}

void CommandFactory::registerCommand(std::string name,
    std::string description,
    bool (*action)(Player&, Map&, Game&)) {
    commands[name] = std::make_unique<GameCommand>(name, description, action);
    actionMap[name] = action;
}

std::unique_ptr<ICommand> CommandFactory::createCommand(std::string name) {
    auto it = commands.find(name);
    if (it != commands.end()) {
        return std::make_unique<GameCommand>(name,
            it->second->getDescription(),
            actionMap[name]);
    }
    return nullptr;
}

std::unique_ptr<ICommand> CommandFactory::createCommandFromInput(const std::string& input) {
    std::string commandName = controlConfig.getCommand(input);

    if (!commandName.empty()) {
        return createCommand(commandName);
    }

    return createCommand(input);
}

CommandFactory::CommandAction CommandFactory::getActionByName(const std::string& name) {
    auto it = actionMap.find(name);
    if (it != actionMap.end()) {
        return it->second;
    }
    return nullptr;
}

bool CommandFactory::hasCommand(std::string name) {
    return commands.find(name) != commands.end();
}

void CommandFactory::showAllCommands() {
    std::cout << "\n=== AVAILABLE COMMANDS ===" << std::endl;
    for (const auto& [name, cmd] : commands) {
        std::cout << name << " - " << cmd->getDescription() << std::endl;
    }
    std::cout << "==========================" << std::endl;
}

std::string CommandFactory::getControlHelp() const {
    return controlConfig.getHelpText();
}

bool CommandFactory::reloadConfig(const std::string& configFile) {
    return controlConfig.loadFromFile(configFile);
}

bool exitGame(Player& player, Map& map, Game& game) {
    return false;
}