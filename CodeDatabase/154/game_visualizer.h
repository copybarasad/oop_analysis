#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

#include "renderer.h"
#include "field.h"
#include "player.h"
#include <iostream>
#include <memory>

template<typename Renderer>
class GameVisualizer {
private:
    Renderer renderer_;
    Field* field_;
    Player* player_;

public:
    GameVisualizer() : field_(nullptr), player_(nullptr) {}

    void setGameState(Field* field, Player* player) {
        field_ = field;
        player_ = player;
    }

    void render() {
        if (field_ && player_) {
            renderer_.clearScreen();
            renderer_.renderField(field_);
            renderer_.renderPlayerInfo(player_);
        }
    }

    void displayMessage(const std::string& message) {
        renderer_.renderMessage(message);
    }

    void displayGameOver(int score, int level) {
        renderer_.renderGameOver(score, level);
    }

    void displayMainMenu() {
        renderer_.clearScreen();
        renderer_.renderMessage("\n====================================");
        renderer_.renderMessage("     WELCOME TO THE GAME!");
        renderer_.renderMessage("====================================");
        renderer_.renderMessage("\n1. New Game");
        renderer_.renderMessage("2. Load Game");
        renderer_.renderMessage("3. Exit");
        renderer_.renderMessage("\nYour choice: ");
    }

    void displayLevelComplete(int level) {
        renderer_.clearScreen();
        renderer_.renderMessage("\n====================================");
        renderer_.renderMessage("     LEVEL " + std::to_string(level) + " COMPLETE!");
        renderer_.renderMessage("====================================");
        renderer_.renderMessage("\n1. Continue to next level");
        renderer_.renderMessage("2. Return to menu");
        renderer_.renderMessage("3. Exit game");
        renderer_.renderMessage("\nYour choice: ");
    }

    void displayError(const std::string& errorMessage) {
        renderer_.renderMessage("\n[ERROR] " + errorMessage);
    }

    void displaySuccess(const std::string& message) {
        renderer_.renderMessage("\n[OK] " + message);
    }
};

#endif
