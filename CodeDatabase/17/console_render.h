#ifndef CONSOLE_RENDER_H
#define CONSOLE_RENDER_H
#include "game_map.h"
#include "entity_manager.h"
#include "player_hand.h"
#include "console_input_handler.h"
#include <string>

class ConsoleRenderer {
private:
    const ConsoleInputHandler* inputHandler;
    
public:
    ConsoleRenderer(const ConsoleInputHandler* handler = nullptr) 
        : inputHandler(handler) {}
    ~ConsoleRenderer() = default;
    void setInputHandler(const ConsoleInputHandler* handler) {
        inputHandler = handler;
    }
    
    void drawGame(const GameMap& map, const EntityManager& entities, int level);
    void drawTarget(const GameMap& map, const EntityManager& entities, 
                   const Position& cursor, int radius);
    void drawMainMenu();
    void drawLevelUpMenu();
    void showMessage(const std::string& msg);
    void showSpells(const PlayerHand& hand);
    void deadPlayer(); 
    
private:
    std::string getControlsString() const;
};

#endif