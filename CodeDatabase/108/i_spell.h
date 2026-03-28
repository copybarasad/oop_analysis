#ifndef OOP_I_SPELL_H
#define OOP_I_SPELL_H

#include <string>

class Player;
class GameState;


class ISpell {
public:
    virtual ~ISpell() = default;

    virtual void cast(Player& caster, GameState& game_state) = 0;

    virtual std::string get_name() const = 0;

};



#endif //OOP_I_SPELL_H