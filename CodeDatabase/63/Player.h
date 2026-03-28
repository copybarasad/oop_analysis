#ifndef RPG_PLAYER_H
#define RPG_PLAYER_H

#include "Entity.h"
#include "Game.h"

enum class GameControl {
    SPELL1 = 0,
    SPELL2 = 1,
    SPELL3 = 2,
    SPELL4 = 3,
    SPELL5 = 4,
    SPELL6 = 5,
    SPELL7 = 6,
    SPELL8 = 7,
    SPELL9 = 8,
    SPELL10 = 9,
    CANCEL,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SHOOT,
    CHANGE_ATTACK_MODE,
    BUY_SPELL,
    LOAD,
    SAVE,
    QUIT
};

enum class PlayerModes {
    MELEE,
    RANGE,
    SPELL,
    SHOP,
    UPGRADE
};

class Player : public Entity {
    unsigned int shootDamage;
    unsigned int shootDiameter;
    PlayerModes mode;
    PlayerModes previousMode;
    unsigned int upgradePoints;
    std::vector<int> protectedSpells;
public:
    Player(unsigned int h, unsigned int d, unsigned int shootDamage, unsigned int shootDiameter, unsigned int spellsHandSize);

    explicit Player(json& data);

    [[nodiscard]] std::unique_ptr<Entity> clone() const override;

    [[nodiscard]] json to_json() const override;

    void setMode(PlayerModes mode);

    [[nodiscard]] PlayerModes getPreviousMode() const;

    [[nodiscard]] int getShootRange() const;

    [[nodiscard]] PlayerModes getMode() const;

    void addHealth(int value);

    void addMaxHealth(int value);

    void addDamage(int value);

    [[nodiscard]] unsigned int getDamage() const override;

    void addUpgradePoints(unsigned int value);

    [[nodiscard]] bool hasUpgradePoints() const;

    [[nodiscard]] std::string upgradeInfo() const;

    void upgrade(std::mt19937 &gen);

    void upgradeHealth();

    void upgradeDamage();

    bool upgradeSpell(int index);
};

#endif