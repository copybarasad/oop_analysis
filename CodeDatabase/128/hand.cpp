#include "hand.hpp"
#include "enhancementspell.hpp"
#include "spellfactory.hpp"
#include "spell_defaults.hpp"
#include "event_dispatcher.hpp"  
#include "events.hpp"   

Hand::Hand(size_t capacity) : capacity_(capacity) {}

bool Hand::AddSpell(std::unique_ptr<Spell> spell) {
    if (spells_.size() >= capacity_) {
        EventDispatcher::instance().notify(UiMessageEvent("❌ The hand is full! You cannot add a new spell ❌"));
        return false;
    }
    spells_.push_back(std::move(spell));
    return true;
}

bool Hand::UseSpell(size_t index, Game& game) 
{
    if (index < 1 || index > spells_.size()) {
        EventDispatcher::instance().notify(
            UiMessageEvent("⚠️  Invalid spell index.")
        );
        return false;
    }
    auto& spell = spells_[index - 1];
    if (spell->getType() == SpellType::kEnhancement) {
        spell->UseSpell(game, {});
        spells_.erase(spells_.begin() + index - 1);
        return true;
    }
    SpellType type = spell->getType();
    SpellParams base  = SpellParams::Default(type);
    SpellParams final = rules_.Apply(type, base, enhancement_stacks_);
    bool success = spell->UseSpell(game, final);
    if (success) 
    {
        if (enhancement_stacks_ > 0) 
        {
            EventDispatcher::instance().notify(UiMessageEvent("💥 Enhancement ×" + std::to_string(enhancement_stacks_) +" applied to " + spell->getName() + "!"));
        }
        ResetEnhancement();
        spells_.erase(spells_.begin() + index - 1);
    }
    return success;
}


void Hand::PrintHand() const {
    if (spells_.empty()) {
        EventDispatcher::instance().notify(UiMessageEvent("There are no spells in your hand…"));
        return;
    }
    EventDispatcher::instance().notify(UiMessageEvent("🪄 Spells in your hand:"));
    for (size_t i = 0; i < spells_.size(); ++i) {
        EventDispatcher::instance().notify(UiMessageEvent(std::to_string(i + 1) + ". " + spells_[i]->getName()));
    }
}

void Hand::GenerateRandomSpell() {
    if (isFull()) {
        EventDispatcher::instance().notify(UiMessageEvent("❌ The hand is full — cannot generate a random spell."));
        return;
    }
    SpellFactory factory;
    auto spell = factory.CreateRandomSpell();
    if (spell) {
        AddSpell(std::move(spell));
    }
}

void Hand::RemoveHalfCards() {
    size_t half = spells_.size() / 2;
    spells_.erase(spells_.begin(), spells_.begin() + half);
}
