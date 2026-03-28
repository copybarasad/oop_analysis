#ifndef OOP_DIRECT_DAMAGE_SPELL_H
#define OOP_DIRECT_DAMAGE_SPELL_H

#include "i_spell.h"
#include <string>


// для того чтобы контролировать, кто использует заклинание игрок или башня, чтобы не создавать два разных заклинания
enum class AttackDirection {
    PLAYER_TO_ENEMY,  // игрок атакует врагов
    TOWER_TO_PLAYER   // башня атакует игрока
};


class DirectDamageSpell : public ISpell {
    std::string name;
    int damage;
    int range;

    AttackDirection direction;

public:

    // в конструкторе установили значения по умолчанию - для случая игрок атакует противника (enemy / tower / spawner)
    explicit DirectDamageSpell(std::string name = "Fireball", int damage = 20, int range = 3,
        AttackDirection dir = AttackDirection::PLAYER_TO_ENEMY);

    void cast(Player& caster, GameState& game_state) override;
    std::string get_name() const override;
};

#endif // OOP_DIRECT_DAMAGE_SPELL_H