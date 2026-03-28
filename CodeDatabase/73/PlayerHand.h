// PlayerHand.h
#ifndef GAMELB1_PLAYERHAND_H
#define GAMELB1_PLAYERHAND_H

#include <vector>
#include <memory>
#include "ISpell.h"
#include <iostream>

class PlayerHand {
public:
    PlayerHand(size_t max_size);

    bool addSpell(std::unique_ptr<ISpell> spell);
    void useSpell(int index, Game& game, int target_x, int target_y);
    const std::vector<std::unique_ptr<ISpell>>& getSpells() const;
    bool isFull() const;
    void removeRandomSpell();
    void save(std::ostream& os) const;
    void load(std::istream& is);

private:
    size_t max_size_;
    std::vector<std::unique_ptr<ISpell>> spells_;
};

#endif //GAMELB1_PLAYERHAND_H