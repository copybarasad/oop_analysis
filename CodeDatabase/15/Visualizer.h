#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "Game.h"
#include "Renderer.h"
#include "Logger.h"
#include <vector>
#include <iostream>
#include <limits>
#include <cstdlib>

template <typename RendererT>
class GameVisualizer : public ILogger {
public:
    GameVisualizer(Game& game, RendererT& renderer) 
        : game_(game), renderer_(renderer) {
        game_.addObserver(this);
    }

    ~GameVisualizer() {
        game_.removeObserver(this);
    }

    void log(const std::string& message) override {
        log_buffer_.push_back(message);
    }
    
    void updateAndRender() {
        clearScreen();

        renderer_.render(game_.getField(), game_.getEntityManager(), game_.getCurrentState(), game_.getCastingCursor());
        
        for (const auto& msg : log_buffer_) {
            std::cout << "[LOG]: " << msg << std::endl;
        }
        
        log_buffer_.clear();
    }

    void showStartupScreen(const std::string& controlsDesc) {
        clearScreen();

        std::cout << "========================================\n";
        std::cout << "         RPG GAME - TACTICS             \n";
        std::cout << "========================================\n\n";

        std::cout << "--- MAP LEGEND ---\n";
        std::cout << " P : Player (You)\n";
        std::cout << " E : Enemy\n";
        std::cout << " B : Enemy Spawner (Building)\n";
        std::cout << " W : Enemy Tower (Shoots magic)\n";
        std::cout << " A : Ally\n";
        std::cout << " T : Trap\n";
        std::cout << " # : Wall (Impassable)\n";
        std::cout << " S : Slow Trap\n";
        std::cout << " . : Empty Ground\n";
        std::cout << " X : Spell Cursor\n\n";

        std::cout << "--- CONTROLS (Current Config) ---\n";
        std::cout << controlsDesc << "\n";

        std::cout << "========================================\n";
        std::cout << "Press ENTER to start the game...";
        
        if (std::cin.peek() == '\n') std::cin.ignore();
        std::cin.get(); 
        
        clearScreen();
    }

private:
    void clearScreen() const {
        #ifdef _WIN32
            std::system("cls");
        #else
            std::system("clear");
        #endif
    }

    Game& game_;
    RendererT& renderer_;
    std::vector<std::string> log_buffer_;
};

#endif