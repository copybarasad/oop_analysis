#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

#include "GameBoard.h"
#include "Hero.h"
#include "Foe.h"
#include "EnemyTower.h"
#include <string>
#include <vector>

class IVisualizationStrategy {
public:
    virtual ~IVisualizationStrategy() = default;
    virtual void displayGameState(const GameBoard& board, 
                                  const Hero& hero, 
                                  const std::vector<Foe>& enemies,
                                  const std::vector<EnemyTower>& towers) = 0;
    virtual void showMessage(const std::string& message) = 0;
    virtual void showMenu() = 0;
    virtual void showHelp() = 0;
    virtual void showPlayerTurn(const Hero& hero) = 0;
    virtual void showVictoryScreen() = 0;
    virtual void showDefeatScreen() = 0;
};

class ConsoleVisualization : public IVisualizationStrategy {
public:
    void displayGameState(const GameBoard& board, 
                         const Hero& hero, 
                         const std::vector<Foe>& enemies,
                         const std::vector<EnemyTower>& towers) override;
    void showMessage(const std::string& message) override;
    void showMenu() override;
    void showHelp() override;
    void showPlayerTurn(const Hero& hero) override;
    void showVictoryScreen() override;
    void showDefeatScreen() override;
    
private:
    void displayBoard(const GameBoard& board, 
                     const Hero& hero, 
                     const std::vector<Foe>& enemies,
                     const std::vector<EnemyTower>& towers) const;
    void displayStats(const Hero& hero) const;
    void displayEnemiesInfo(const std::vector<Foe>& enemies) const;
    void displayTowersInfo(const std::vector<EnemyTower>& towers) const;
    void displaySpells(const Hero& hero); 
};

template<typename T = ConsoleVisualization>
class GameVisualizer {
private:
    T visualizationStrategy;
    
public:
    void update(const GameBoard& board, 
                const Hero& hero, 
                const std::vector<Foe>& enemies,
                const std::vector<EnemyTower>& towers) {
        visualizationStrategy.displayGameState(board, hero, enemies, towers);
    }
    
    void showMessage(const std::string& message) {
        visualizationStrategy.showMessage(message);
    }
    
    void showMenu() {
        visualizationStrategy.showMenu();
    }
    
    void showHelp() {
        visualizationStrategy.showHelp();
    }
    
    void showPlayerTurn(const Hero& hero) {
        visualizationStrategy.showPlayerTurn(hero);
    }
    
    void showVictoryScreen() {
        visualizationStrategy.showVictoryScreen();
    }
    
    void showDefeatScreen() {
        visualizationStrategy.showDefeatScreen();
    }
};

#endif 