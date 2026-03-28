#ifndef PLAYER_HAND_H
#define PLAYER_HAND_H

#include "Spell.h"
#include <vector>
#include <memory>

class PlayerHand {
public:
    explicit PlayerHand(int max_size);
    ~PlayerHand();

    bool AddSpell(Spell* spell);
    bool RemoveSpell(int index);
    Spell* GetSpell(int index) const;
    int GetSpellCount() const;
    int GetMaxSize() const;

    void AddEnhancementStack();
    int GetEnhancementStacks() const;
    void ResetEnhancementStacks();

    void RemoveRandomSpells(int count);
    void Clear();

private:
    std::vector<std::unique_ptr<Spell>> spells_;
    int max_size_;
    int enhancement_stacks_;
};

#endif // PLAYER_HAND_H
