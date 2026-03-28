#ifndef SPELLPOSITIONINGSYSTEM_H
#define SPELLPOSITIONINGSYSTEM_H

#include <vector>
#include <utility>

class Player;
class GameField;

class SpellPositioningSystem {
public:
    static bool isValidSpellPosition(const Player& caster, const GameField& field, int x, int y, int range);
    static std::vector<std::pair<int, int>> getValidSpellPositions(const Player& caster, const GameField& field, int range, bool excludePlayerPosition = true);
    static int calculateDistance(const Player& caster, int x, int y);
};

#endif // SPELLPOSITIONINGSYSTEM_H