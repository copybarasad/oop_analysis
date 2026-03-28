#ifndef RPG_GAME_H
#define RPG_GAME_H

#include "GameField.h"
#include "EntityManager.h"
#include "Shop.h"

enum States {
    KEEPS = false,
    WIN,
    LOSE
};

class Game {
    Shop shop;
    int level;
    GameField field;
    EntityManager entityManager;

    [[nodiscard]] std::pair<int, int> findClosestOpponent(const Entity *to, int distance) const;

    [[nodiscard]] std::pair<int, int> wayToClosestOpponent(const Entity *to, std::pair<int, int> enemyPosition);

    void killAllies();
public:
    Game();

    explicit Game(json& data);

    Game& operator=(Game&& other) noexcept;

    json to_json();

    bool moveEntity(std::pair<int,int>& direction, Entity* entity);

    bool checkAimBorders(std::pair<int, int> &direction, Entity *entity) const;

    bool shootEntity(const std::pair<int, int> &targetPosition, Entity *shooter);

    bool checkAimSpellBorders(std::pair<int, int> &direction, Entity *entity) const;

    bool castSpell(std::pair<int, int> &targetPosition, Entity *caster);

    [[nodiscard]] bool playerAlive() const;

    void spawnEnemies();

    void gameCycle();

    States isLevelEnded();

    [[nodiscard]] int getLevel() const;

    template<typename T>
    Entity* spawnEntity(T entity);

    Player& getPlayer();

    GameField& getGameField();

    EntityManager& getEntityManager();

    Shop& getShop();

    bool nextLevel();
};

template<typename T>
Entity* Game::spawnEntity(T entity) {
    auto coordinates = field.getFreeCellCoordinates();
    if (coordinates.first == -1) return nullptr;

    entity.setPosition(coordinates);
    entity.addPoints(field.getCell(coordinates).entityPlaced());

    return entityManager.addEntity(entity);;
}

#endif