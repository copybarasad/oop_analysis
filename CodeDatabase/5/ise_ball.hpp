#ifndef ICE_BALL_HPP
#define ICE_BALL_HPP

#include "spell.hpp"


class IseBall : public Spell {
private:
    int damage;
    int range;
    
public:
    IseBall();
    void cast_spell(Player& player, int targetX, int targetY, 
             std::vector<Enemy>& enemies, const GameField& field)override;
    std::string getType() const override;
};

#endif