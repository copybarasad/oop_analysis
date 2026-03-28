#pragma once
#include <vector>
#include <memory>
#include "SpellCard.h"

class PlayerHand {
private:
    std::vector<SpellCard> hand;
    int maxSize;
    
public:
    PlayerHand(int maxHandSize = 5);
    
    PlayerHand(const PlayerHand&) = delete;
    PlayerHand& operator=(const PlayerHand&) = delete;
    
    PlayerHand(PlayerHand&& other) noexcept;
    PlayerHand& operator=(PlayerHand&& other) noexcept;
    ~PlayerHand() = default;
    
    bool addSpell(std::unique_ptr<Spell> spell);
    bool removeSpell(int index);
    SpellCard* getSpell(int index);
    int getHandSize() const;
    int getMaxSize() const;
    bool isHandFull() const;
    std::vector<SpellCard>& getHand();
    const std::vector<SpellCard>& getHand() const;
};