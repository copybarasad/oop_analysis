#include "hand.h"
#include "game_field.h"
#include "entity.h"
#include "prints.h"

bool PlayerHand::addSpell(std::unique_ptr<ISpell> newSpell)
{
    if (spells.size() < maxSize)
    {

        spells.push_back(std::move(newSpell));
        Prints::print_spell_add(spells.back()->getName());
        return true;
    }
    else
    {
        Prints::print_hand_is_full_string();
        return false;
    }
}

void PlayerHand::useSpell(size_t index, int targetX, int targetY, Entity* gamer) {
    if (index < spells.size())
    {
        Prints::print_spell_cast(spells[index]->getName());
        CastContext context = {targetX , targetY, gamer};
        spells[index]->cast(context);
        spells.erase(spells.begin() + index);
    }
    else
    {
        Prints::print_incorrect_spell_string();
    }
}

void PlayerHand::display() const
{
    Prints::print_top_spell(spells.size(), maxSize);
    if (spells.empty())
    {
        Prints::print_hand_is_empty_string();
    }
    else
    {
        for (size_t i = 0; i < spells.size(); ++i)
        {
            std::cout << i << ": " << spells[i]->getName() << std::endl;
        }
    }
}

void PlayerHand::increaseMaxSize() {
    maxSize++;
}

void PlayerHand::upgradeSpell(size_t index)
{
    if (index < spells.size())
    {
        spells[index]->upgrade(); // Делегируем улучшение самому заклинанию
        Prints::print_spell_upgraded(spells[index]->getName());
    }
    else
    {
        Prints::print_incorrect_spell_string();
    }
}

int PlayerHand::getMaxSize(){ return maxSize; }

int PlayerHand::getSpellCount(){
    return spells.size();
}

void PlayerHand::removeSpell(){
    spells.pop_back();
}

