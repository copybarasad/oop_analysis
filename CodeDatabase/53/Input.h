#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <limits>
#include <memory>
#include <unordered_map>
#include "GameField.h"
#include "Player.h"
#include "HandSpell.h"
#include "Constants.h"
#include "InputInterface.h"
#include "InputConfig.h"

using namespace std;
using namespace GameConstants;

class Input : public InputInterface {
private:
    InputConfig config; 
    unordered_map<string, char> keyBindings;  
    unordered_map<char, string> reverseBindings;  
    
    void loadConfig();
    
public:
    Input();
    
    bool validateCoordinates(int x, int y, const GameField& field) override;
    bool validateDirection(char direction) override;
    void clearInputBuffer() override;
    std::unique_ptr<Command> processSpellInput(SpellID spellID, Player& player, GameField& field) override;
    std::unique_ptr<Command> processShopInput(Player& player) override; 

    int getLevelChoice() override;
    int getMainMenuChoice() override; 
    std::string getSaveName() override;
    std::string getPlayerName() override;
    int getLoadChoice(const std::vector<std::string>& saves) override;
    char getLevelTransitionChoice() override;
    int getUpgradeChoice() override;

    char getPlayerInput() override;
    std::unique_ptr<Command> processInput(char input, Player& player, GameField& field, Game& game) override;
    
    void reloadConfig();
    void saveConfig(const std::string& filename = "keyboardControlConfig.txt");
    std::string getCommandForChar(char c) const;
    char getCharForCommand(const std::string& command) const;
};

#endif