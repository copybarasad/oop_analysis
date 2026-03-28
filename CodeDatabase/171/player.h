#ifndef PLAYER_H
#define PLAYER_H

#include "01_position.h"
#include "05_spell_hand.h"
#include <memory>

class Player {
public:
    Player(int maxHealth, int damage, Position startPosition, int handSize = 5);

    void takeDamage(int damage);
    void switchCombatMode();
    void move(Position newPosition);

    // Методы для системы заклинаний
    bool castSpell(int spellIndex, Position targetPos);
    void addSpell(std::shared_ptr<SpellCard> spell);
    SpellHand& getSpellHand();

    int getHealth() const;
    Position getPosition() const;
    bool isMeleeMode() const;
    int getDamage() const;
    bool isAlive() const;

    // Методы для получения заклинаний за убийства
    void addKill();
    int getKillCount() const;
    int getKillsForNextSpell() const;

private:
    int maxHealth_;
    int health_;
    int meleeDamage_;
    int rangedDamage_;
    Position position_;
    bool fightMode_;

    // Система заклинаний
    std::unique_ptr<SpellHand> spellHand_;
    int killCount_;
    static const int KILLS_PER_SPELL = 3; // Каждые 3 убийства - новое заклинание
};

#endif