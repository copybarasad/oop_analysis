#ifndef PLAYER_HAND_H
#define PLAYER_HAND_H

#include "spell.h"
#include <vector>
#include <memory>

class PlayerHand {
private:
    std::vector<SpellPtr> spells_;
    size_t maxSize_;

public:
    PlayerHand(size_t maxSize = 3);
    void addRandomSpell();
    bool addSpell(SpellPtr spell);
    bool useSpell(size_t index, GameController& game, int x, int y);
    bool removeSpell(size_t index);
    void upgradeMaxSize(int delta); 

    size_t size() const;
    size_t getMaxSize() const;
    const std::vector<SpellPtr>& getSpells() const;

    void save(std::ostream& os) const;
    void load(std::istream& is);

    PlayerHand(const PlayerHand&) = delete;
    PlayerHand& operator=(const PlayerHand&) = delete;
    PlayerHand(PlayerHand&&) = default;
    PlayerHand& operator=(PlayerHand&&) = default;
};

#endif