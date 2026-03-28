#pragma once
#include "character.h"

class Dracula : public Character {
private:
    int fear_level_;
    std::pair<int, int> calculateMoveDirection(const Position& hunter_position) const;
    //lb3
    bool is_invulnerable_;  // Состояние неуязвимости
public:
    Dracula(const Position& position);
    
    void takeDamage(int amount) override;
    
    void applyCellEffect(CellType cell_type) override;
    bool makeMove(const Position& hunter_pos, int hunter_research, Field& field);
    void calculateFearLevel(int hunter_research);

    int getFearLevel() const;
    //lb3
    void setFearLevel(int fearLevel) { fear_level_ = fearLevel; }
    void setInvulnerable(bool invulnerable) { is_invulnerable_ = invulnerable; }
    bool isInvulnerable() const { return is_invulnerable_; }
};