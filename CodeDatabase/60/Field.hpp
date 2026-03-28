#pragma once
#include <Ceil.hpp>
#include <vector>
#include <memory>
#include <ScreenSize.hpp>

class Entity;
class LivingEntity;
class Player;

class Field {
    std::vector<std::vector<Ceil>> ceils;
    std::vector<std::shared_ptr<LivingEntity>> entities;
    std::shared_ptr<Player> player_ptr;
    const ScreenSize* screen;
    int height;
    int width;
    size_t turnCounter;
    int currentLevel;

public:
    Field();
    Field(const Field& other);
    Field(Field&& other);
    Field& operator=(const Field& other);
    Field& operator=(Field&& other);

    int getCurrentLevel() const;
    int getH() const;
    int getW() const;
    size_t getTurnCounter() const;
    const std::vector<std::shared_ptr<LivingEntity>>& getEntities() const;
    std::shared_ptr<Player> getPlayer() const;

    void setScreen(const ScreenSize* screen);
    void init();
    void draw() const;

    bool updatePlayer(int ch);      
    void updateAllies();
    void updateEnemies();
    void updateStructures();        
    void cleanupDeadEntities();
    std::shared_ptr<Entity> getEntityAt(int worldY, int worldX) const;
    void advanceToNextLevel();
    void initLevel(bool spawnNewEntities = true);
    bool isLevelClear() const;
    const ScreenSize* getScreen() const;

    bool screenToWorld(int screenX, int screenY, int& worldX, int& worldY) const;

    void setTurnCounter(size_t count);
    void setCurrentLevel(int level);
    void clearAndCreateBaseLayout();
    void addPlayer(std::shared_ptr<Player> player);
    void addEntity(std::shared_ptr<LivingEntity> entity);
    void createPlayerTrapAt(int x, int y, int damage);
    void createSpikedTrapAt(int x, int y);
};