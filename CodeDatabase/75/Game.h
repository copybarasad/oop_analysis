#pragma once
#include <SFML/Graphics.hpp>
#include "GameField.h"
#include "GameRender.h"
#include "Player.h"
#include "Enemy.h"
#include "LevelConfig.h"
#include "VisualEffects.h"
#include "Portal.h"
#include "Shop.h"
#include "WorldObject.h"
#include <vector>
#include <memory>

class Tower; class Shield; class Ally; class Trap; class ThornsArea;
enum class GameMode { STORY, FREE_PLAY };

struct GameCommand;
class Game {
public:
    void update(float dt);
    enum class State { PLAYING, GAME_OVER, VICTORY, GAME_COMPLETED, HUB };

    void executeCommand(const struct GameCommand& cmd);
    bool isRunning() const { return m_isRunning; }
    bool isPaused() const { return m_isPaused; }
    bool isShopOpen() const { return m_isShopOpen; }
    bool isTargeting() const { if(!m_player) return false; return m_player->isTargeting(); }
    int getTargetX() const { return m_targetCursorX; }
    int getTargetY() const { return m_targetCursorY; }
    State getState() const { return m_gameState; }
    int getLevelIndex() const { return m_currentLevelIndex; }
    
    // Getters for Visualizer
    const GameField* getField() const { return m_field; }
    const std::vector<ThornsArea*>& getThorns() const { return m_thornsAreas; }
    const std::vector<VisualEvent>& getVisualEvents() const { return m_visualEvents; }
    
    
    Game(const LevelConfig& config, GameMode mode, int currentLevelIndex = 1);
    ~Game();
    

    const LevelConfig& getConfig() const { return m_levelConfig; }
    Player* getPlayer() const { return m_player; }
    const std::vector<Enemy*>& getEnemies() const { return m_enemies; }
    const std::vector<Ally*>& getAllies() const { return m_allies; }
    const std::vector<Trap*>& getTraps() const { return m_traps; }
    const std::vector<Tower*>& getTowers() const { return m_towers; }
    const std::vector<Shield*>& getShields() const { return m_shields; }
    const std::vector<WorldObject*>& getWorldObjects() const { return m_worldObjects; }
    Portal* getPortal() const { return m_portal; }
    Shop* getShop() const { return m_shop; }
    
    void createShield(int x, int y, int duration, int radius);
    void createThornsArea(int x, int y, int radius, int duration, int dps);
    void summonAlly(int x, int y);
    void createTrap(int x, int y, int damage, int duration);
    void addVisualEvent(VisualEvent::Type type, float sx, float sy, float ex, float ey, sf::Color color = sf::Color::White);
    bool teleportPlayer(int distance);
    void castNova(int damage, int radius);
    void castLifeDrain(Character* target, int damage);

    friend class SaveManager; 

private:
     
    
    sf::Clock m_dtClock;
    GameField* m_field;
    Player* m_player;
    std::vector<Enemy*> m_enemies;
    std::vector<WorldObject*> m_worldObjects; 
    std::vector<ThornsArea*> m_thornsAreas;
    std::vector<Ally*> m_allies;
    std::vector<Trap*> m_traps; 
    std::vector<Tower*> m_towers;
    std::vector<Shield*> m_shields;
    Portal* m_portal = nullptr;
    Shop* m_shop = nullptr;
    std::vector<VisualEvent> m_visualEvents;
    bool m_isRunning;
    bool m_isPaused;
    bool m_isShopOpen = false; 
    State m_gameState;
    GameMode m_gameMode;
    int m_currentLevelIndex;
    Bullet::Direction m_lastDirection;
    int m_targetCursorX, m_targetCursorY;
    Enemy* m_lockedTarget = nullptr;
    float m_playerMoveCooldown = 0.0f;
    LevelConfig m_levelConfig;
    
        
    
    void handleNormalMode(sf::Keyboard::Key key);
    void handleTargetingMode(sf::Keyboard::Key key);
    void handleShopMode(sf::Keyboard::Key key);
    void handlePlayerMovement(int x, int y);
    void initializeFromConfig(const LevelConfig& config, bool preservePlayer);
    void loadNextLevel();
    void enterHub();
    void restartLevel();
    void spawnEnemies();
    void spawnWorld(); 
    void spawnTowers();
    void cleanup(bool keepPlayer);
    bool isValidMove(int x, int y) const;
    void togglePause();
    void handlePauseMenu(sf::Keyboard::Key key);
};