#pragma once
#include <memory>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include "GameCommand.h" 

template<typename InputProvider>
class GameController {
private:
    InputProvider inputProvider;
    
    std::function<void()> onMoveUp;
    std::function<void()> onMoveDown;
    std::function<void()> onMoveLeft;
    std::function<void()> onMoveRight;
    std::function<void()> onShowStats;
    std::function<void()> onShowInstructions;
    std::function<void()> onShowSpells;
    std::function<void()> onBuySpells;
    std::function<void(int)> onCastSpell;
    std::function<void(int)> onChangeCombatRange;
    std::function<void()> onSaveGame;
    std::function<void()> onLoadGame;
    std::function<void()> onExit;

    void setupDefaultKeyMap() {
        std::unordered_map<char, std::string> keyMap = {
            {'w', "MOVE_UP"},
            {'s', "MOVE_DOWN"}, 
            {'a', "MOVE_LEFT"},
            {'d', "MOVE_RIGHT"},
            {'z', "SHOW_STATS"},
            {'i', "SHOW_INSTRUCTIONS"},
            {'h', "SHOW_SPELLS"},
            {'p', "BUY_SPELLS"},
            {'c', "CAST_SPELL"},
            {'b', "CHANGE_COMBAT_RANGE"},
            {'v', "SAVE_GAME"},
            {'l', "LOAD_GAME"},
            {'e', "EXIT"}
        };
        inputProvider.setKeyMap(keyMap);
    }

public:
    explicit GameController(const InputProvider& provider) 
        : inputProvider(provider) {
        setupDefaultKeyMap();
    }
    
    void processInput() {
        GameCommand command = inputProvider.getInput();
        processCommand(command);
    }
    
    void processCommand(const GameCommand& command) {
        switch (command.type) {
            case GameCommand::Type::MOVE_UP:
                if (onMoveUp) onMoveUp();
                break;
            case GameCommand::Type::MOVE_DOWN:
                if (onMoveDown) onMoveDown();
                break;
            case GameCommand::Type::MOVE_LEFT:
                if (onMoveLeft) onMoveLeft();
                break;
            case GameCommand::Type::MOVE_RIGHT:
                if (onMoveRight) onMoveRight();
                break;
            case GameCommand::Type::SHOW_STATS:
                if (onShowStats) onShowStats();
                break;
            case GameCommand::Type::SHOW_INSTRUCTIONS:
                if (onShowInstructions) onShowInstructions();
                break;
            case GameCommand::Type::SHOW_SPELLS:
                if (onShowSpells) onShowSpells();
                break;
            case GameCommand::Type::BUY_SPELLS:
                if (onBuySpells) onBuySpells();
                break;
            case GameCommand::Type::CAST_SPELL:
                if (onCastSpell) onCastSpell(command.spellIndex);
                break;
            case GameCommand::Type::CHANGE_COMBAT_RANGE:
                if (onChangeCombatRange) onChangeCombatRange(command.combatRange);
                break;
            case GameCommand::Type::SAVE_GAME:
                if (onSaveGame) onSaveGame();
                break;
            case GameCommand::Type::LOAD_GAME:
                if (onLoadGame) onLoadGame();
                break;
            case GameCommand::Type::EXIT:
                if (onExit) onExit();
                break;
            case GameCommand::Type::INVALID:
                std::cout << "Invalid command: " << command.data << std::endl;
                break;
        }
    }
    
    void displayCurrentControls() const {
        std::cout << "=== Current Controls ===" << std::endl;
        auto keyMap = inputProvider.getKeyMap();
        for (const auto& pair : keyMap) {
            std::cout << "'" << pair.first << "' -> " << pair.second << std::endl;
        }
        std::cout << "=======================" << std::endl;
    }
    
    void setOnMoveUp(std::function<void()> callback) { onMoveUp = callback; }
    void setOnMoveDown(std::function<void()> callback) { onMoveDown = callback; }
    void setOnMoveLeft(std::function<void()> callback) { onMoveLeft = callback; }
    void setOnMoveRight(std::function<void()> callback) { onMoveRight = callback; }
    void setOnShowStats(std::function<void()> callback) { onShowStats = callback; }
    void setOnShowInstructions(std::function<void()> callback) { onShowInstructions = callback; }
    void setOnShowSpells(std::function<void()> callback) { onShowSpells = callback; }
    void setOnBuySpells(std::function<void()> callback) { onBuySpells = callback; }
    void setOnCastSpell(std::function<void(int)> callback) { onCastSpell = callback; }
    void setOnChangeCombatRange(std::function<void(int)> callback) { onChangeCombatRange = callback; }
    void setOnSaveGame(std::function<void()> callback) { onSaveGame = callback; }
    void setOnLoadGame(std::function<void()> callback) { onLoadGame = callback; }
    void setOnExit(std::function<void()> callback) { onExit = callback; }
    
    InputProvider& getInputProvider() { return inputProvider; }
    const InputProvider& getInputProvider() const { return inputProvider; }

    void showSpellsIfAvailable() {
        if (onShowSpells) onShowSpells();
    }
};