#pragma once
#include <functional>
#include <memory>
#include <iostream>
#include <limits>
#include <cctype> 

template<typename T>
class GameController;

struct GameCallbacksConfig {
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

    template<typename GameType>
    static GameCallbacksConfig createDefaultCallbacks(GameType& game, bool& gameRunning);
    
    template<typename InputProvider>
    void applyToController(GameController<InputProvider>& controller) const;
};

template<typename InputProvider>
void GameCallbacksConfig::applyToController(GameController<InputProvider>& controller) const {
    controller.setOnMoveUp(onMoveUp);
    controller.setOnMoveDown(onMoveDown);
    controller.setOnMoveLeft(onMoveLeft);
    controller.setOnMoveRight(onMoveRight);
    controller.setOnShowStats(onShowStats);
    controller.setOnShowInstructions(onShowInstructions);
    controller.setOnShowSpells(onShowSpells);
    controller.setOnBuySpells(onBuySpells);
    controller.setOnCastSpell(onCastSpell);
    controller.setOnChangeCombatRange(onChangeCombatRange);
    controller.setOnSaveGame(onSaveGame);
    controller.setOnLoadGame(onLoadGame);
    controller.setOnExit(onExit);
}

template<typename GameType>
GameCallbacksConfig GameCallbacksConfig::createDefaultCallbacks(GameType& game, bool& gameRunning) {
    GameCallbacksConfig config;
    
    config.onMoveUp = [&game]() {
        std::cout << "> Command: MOVE_UP" << std::endl;
        game.ProcessPlayerInputPublic('w');
    };
    
    config.onMoveDown = [&game]() {
        std::cout << "> Command: MOVE_DOWN" << std::endl;
        game.ProcessPlayerInputPublic('s');
    };
    
    config.onMoveLeft = [&game]() {
        std::cout << "> Command: MOVE_LEFT" << std::endl;
        game.ProcessPlayerInputPublic('a');
    };
    
    config.onMoveRight = [&game]() {
        std::cout << "> Command: MOVE_RIGHT" << std::endl;
        game.ProcessPlayerInputPublic('d');
    };
    
    config.onShowStats = [&game]() {
        std::cout << "> Command: SHOW_STATS" << std::endl;
        game.ProcessPlayerInputPublic('z');
    };
    
    config.onShowInstructions = [&game]() {
        std::cout << "> Command: SHOW_INSTRUCTIONS" << std::endl;

        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "                   GAME INSTRUCTIONS" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        
        std::cout << "\nMOVEMENT COMMANDS:" << std::endl;
        std::cout << "  W - Move Up" << std::endl;
        std::cout << "  S - Move Down" << std::endl;
        std::cout << "  A - Move Left" << std::endl;
        std::cout << "  D - Move Right" << std::endl;
        
        std::cout << "\nINFORMATION COMMANDS:" << std::endl;
        std::cout << "  Z - Show Player & Enemy Stats" << std::endl;
        std::cout << "  I - Show These Instructions" << std::endl;
        std::cout << "  H - Show Available Spells" << std::endl;
        
        std::cout << "\nSPELL COMMANDS:" << std::endl;
        std::cout << "  P - Buy New Spells" << std::endl;
        std::cout << "  C - Cast a Spell" << std::endl;
        std::cout << "  B - Change Combat Range (Melee/Ranged)" << std::endl;
        
        std::cout << "\nSYSTEM COMMANDS:" << std::endl;
        std::cout << "  V - Save Current Game" << std::endl;
        std::cout << "  L - Load Saved Game" << std::endl;
        std::cout << "  E - Exit Game (with confirmation)" << std::endl;
        
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "TIP: Use WASD to move, attack enemies in range!" << std::endl;
        std::cout << std::string(50, '=') << "\n" << std::endl;
    };
    
    config.onShowSpells = [&game]() {
        std::cout << "> Command: SHOW_SPELLS" << std::endl;
        game.ProcessPlayerInputPublic('h');
    };
    
    config.onBuySpells = [&game]() {
        std::cout << "> Command: BUY_SPELLS" << std::endl;
        game.ProcessPlayerInputPublic('p');
    };
    
    config.onCastSpell = [&game](int spellIndex) {
        game.ProcessPlayerInputPublic('h');
        game.ProcessPlayerInputPublic('c');
        if (spellIndex != -1) {
            std::cout << "  Spell index " << spellIndex << " selected" << std::endl;
        }
    };
    
    config.onChangeCombatRange = [&game](int range) {
        std::cout << "> Command: CHANGE_COMBAT_RANGE (range: " << range << ")" << std::endl;
        game.ProcessPlayerInputPublic('b');
        if (range != -1) {
            std::cout << "  Combat range set to: " << (range == 0 ? "MELEE" : "RANGED") << std::endl;
        }
    };
    
    config.onSaveGame = [&game]() {
        std::cout << "> Command: SAVE_GAME" << std::endl;
        game.ProcessPlayerInputPublic('v');
    };
    
    config.onLoadGame = [&game]() {
        std::cout << "> Command: LOAD_GAME" << std::endl;
        game.ProcessPlayerInputPublic('l');
    };
    
    config.onExit = [&gameRunning]() {
        std::cout << "\n> Command: EXIT" << std::endl;
        std::cout << "Exit? (y/n): ";
        char confirm;
        std::cin >> confirm;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (std::tolower(confirm) == 'y') {
            gameRunning = false;
            std::cin.clear();
            std::cin.sync();
        } else {
            std::cout << "Continuing game..." << std::endl;
        }
    };
    
    return config;
}