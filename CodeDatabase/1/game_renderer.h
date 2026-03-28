#pragma once
#include "game.h"
#include "game_context.h"
#include "command_registry.h"
#include "key_bindings_config.h"
#include "player_upgrade_system.h"
#include <string>

class GameRenderer {
public:
    void render(const Game& game);
    void renderGameState(const ConstGameContext& context);
    
    void displayField(const ConstGameContext& context);
    void displayStatus(const ConstGameContext& context);
    void displaySpells(const VampireHunter& hunter);
    
    void displayHelp(const CommandRegistry& commandRegistry);
    void displayHelp(const KeyBindingsConfig& keyBindings);
    void displayHelp(const std::map<std::string, 
                    std::vector<std::pair<std::string, std::string>>>& commandsByCategory);
    
    void displayMessage(const std::string& message);
    void displayTurnInfo(const ConstGameContext& context);
    
    void displayLevelInfo(const ConstGameContext& context, int currentLevel, 
                         const LevelConfig& config);
    
    void displayWelcome();
    void displayQuickHelp();
    void displayLibraryChoice(const Library& library);
    void displayLevelUpOptions(const std::vector<PlayerUpgradeSystem::UpgradeOption>& upgrades);
    
    std::string getPlayerChoice();
    Position getSpellTarget();
    int getSpellIndex(int maxIndex);
    int getUpgradeChoice(int maxChoice);
    
private:
    char getCellSymbol(const Position& pos, const ConstGameContext& context) const;
    void printHealthBar(int current, int max, int width = 20) const;
};