#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include <memory>

#include "darkbuilding.h"
#include "darktower.h"
#include "gameboard.h"
#include "enemy.h"
#include "archer.h"
#include "player.h"
#include "eventdispatcher.h"

class GameModel
{
public:
    GameModel();
    ~GameModel() = default;

    std::vector<std::unique_ptr<Player>>& getPlayers() {return m_players;}
    std::vector<std::unique_ptr<Enemy>>& getEnemies() {return m_enemies;}
    std::vector<std::unique_ptr<Archer>>& getArchers() {return m_archers;}
    GameBoard& getBoard() {return *m_board;}
    DarkTower& getTower() { return *m_tower; }
    DarkBuilding& getBuilding() { return *m_building; }
    int& getLevel() { return m_currentLevel; }
    bool& isLogicActive() { return m_logicActive; }
    bool& inLevel() { return m_inGame; }
    void spawnPlayer(int health=CharacterConfig::Health, int countMana=CharacterConfig::Mana, int xPos=0, int yPos=BoardConfig::Size-1, int meleeDamage=CharacterConfig::MeleeDamage, int rangedDamage=CharacterConfig::RangedDamage);
    void spawnBuilding(int xPos, int yPos);
    void spawnEnemy(int health=CharacterConfig::Health, int xPos=BoardConfig::Size-1, int yPos=2, int meleeDamage=CharacterConfig::MeleeDamage);
    void spawnArcher(int xPos, int yPos, int health=CharacterConfig::Health, int meleeDamage=CharacterConfig::MeleeDamage);
    bool moveCharacter(Entity& entity, GameBoard& board, int dx, int dy);
    void attackEnemies();
    void updateBuildings();
    void updateGameState();
    void updateEnemies();
    void updateArchers();
    bool isLevelComplete();
    bool isLevelFailed();
    void loadLevel(int healthPlayer, int countMana);
    void newLevel(int healthPlayer, int countMana);

private:
    int m_currentLevel{0};
    int damageEnemy;
    bool m_inGame{false};
    bool m_logicActive{false};

    std::vector<std::unique_ptr<Player>> m_players;
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::vector<std::unique_ptr<Archer>> m_archers;
    std::unique_ptr<GameBoard> m_board;
    std::unique_ptr<DarkBuilding> m_building;
    std::unique_ptr<DarkTower> m_tower;
};

#endif // GAMEMODEL_H
