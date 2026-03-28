#pragma once

#include "IHasHand.hpp"
#include "hand.hpp"

class HasHand : public virtual IHasHand {
protected:
    Hand hand;
public:
    HasHand(int capacity);
    HasHand(int capacity, std::shared_ptr<ISpell> initial_spell);

    HasHand(HandImage hand_image);

    std::shared_ptr<ISpell> get_spell(int index) override;
    bool add_spell(std::shared_ptr<ISpell> spell) override;
};