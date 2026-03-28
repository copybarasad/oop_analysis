#ifndef RPG_ENTITY_H
#define RPG_ENTITY_H

#include <memory>
#include <utility>
#include "SpellsHand.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define SUCCESSFUL_STEP {0, 0}
#define UNCOMPLETED_STEP {-1, -1}

enum class EntityTeam {
    ALLY,
    ENEMY
};

enum class StepType {
    WALK,
    SPELL,
};

typedef struct {
    std::pair<int, int> enemyPosition;
    std::pair<int, int> wayToEnemy;
} StepRequirements;

typedef struct {
    StepType type;
    std::pair<int, int> target;
} StepInfo;

class Entity {
    unsigned int maxHealth;
    unsigned int health;
    EntityTeam team;
    char symbol;
    unsigned int viewDiameter;
    unsigned int points;
    bool slowed;
    std::pair<int,int> position;
    std::pair<int,int> target;
protected:
    unsigned int damage;
    SpellsHand spellsHand;
public:
    Entity(unsigned int h, unsigned int d, EntityTeam type, char symbol, unsigned int viewDiameter, unsigned int spellsHandSize);

    Entity(json& data, EntityTeam team);

    virtual ~Entity();

    [[nodiscard]] virtual std::unique_ptr<Entity> clone() const = 0;

    [[nodiscard]] virtual json to_json() const;

    void setPosition(const std::pair<int,int> &new_position);

    void setSlowed(bool state);

    void setTarget(std::pair<int, int>& newTarget);

    void setTarget(std::pair<int, int> newTarget);

    void setPoints(unsigned int amount);

    void addPoints(unsigned int amount);

    bool takeDamage(unsigned int amount);

    virtual StepInfo move(StepRequirements& arguments);

    virtual void onEnter(Entity* aggressor);

    [[nodiscard]] unsigned int getHealth() const;

    void setHealth(unsigned int value);

    void setMaxHealth(unsigned int value);

    [[nodiscard]] unsigned int getMaxHealth() const;

    void restoreHealth();

    [[nodiscard]] bool isAlive() const;

    [[nodiscard]] virtual unsigned int getActionDiameter();

    void setViewDiameter(unsigned int newDistance);

    [[nodiscard]] unsigned int getViewDiameter() const;

    [[nodiscard]] const std::pair<int,int>& getPosition() const;

    [[nodiscard]] virtual unsigned int getDamage() const;

    [[nodiscard]] EntityTeam getTeam() const;

    [[nodiscard]] bool isSlowed() const;

    [[nodiscard]] char getSymbol() const;

    [[nodiscard]] unsigned int getPoints() const;

    std::pair<int, int> getTarget();

    [[nodiscard]] SpellsHand& getSpellsHand();

    void reloadSpells();

    [[nodiscard]] std::string getSpellsInfo() const;
};

#endif
