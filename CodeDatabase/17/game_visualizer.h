#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H
#include "console_render.h"
#include "game_map.h"
#include "entity_manager.h"
#include "player_hand.h"

template<typename Renderer>
class GameVisualizer {
private:
    Renderer renderer;

public:
    GameVisualizer(const ConsoleInputHandler* handler = nullptr) 
        : renderer(handler) {}
    
    ~GameVisualizer() = default;
        
    void setInputHandler(const ConsoleInputHandler* handler) {
        renderer.setInputHandler(handler);
    }

    void drawMainMenu() {
        renderer.drawMainMenu();
    }

    void drawGame(const GameMap& map, const EntityManager& entities, int level) {
        renderer.drawGame(map, entities, level);
    }

    void drawTarget(const GameMap& map, const EntityManager& entities, Position cursor, int spellRadius) {
        renderer.drawTarget(map, entities, cursor, spellRadius);
    }

    Position selectTarget(const GameMap& map, const EntityManager& entities, Position cursor, int spellRadius) {
        return renderer.selectTarget(map, entities, cursor, spellRadius);
    }

    void showLevelUpMenu() {
        renderer.drawLevelUpMenu();
    }

    void showSpells(const PlayerHand& hand) {
        renderer.showSpells(hand);
    }

    void deadPlayer() {
        renderer.deadPlayer();
    }

    char getInput() { return renderer.getInput(); }
    void showMessage(const std::string& msg) { renderer.showMessage(msg); }

};

#endif
