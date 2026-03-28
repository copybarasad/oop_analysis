//
// Created by Artem on 24.10.2025.
//

#ifndef LABAOOP2_SPELLHAND_H
#define LABAOOP2_SPELLHAND_H

#include "../Spell.h"
#include <vector>
#include <memory>
#include "../../../../../Constants/Constants.h"
#include "../../../../../GameField//Field.h"
#include "../../../../../Factories/SpellFactory.h"

class SpellHand {
public:
    SpellHand() {}

    SpellHand(int gradeLevel, std::vector<std::unique_ptr<Spell>>&& spells) : gradeLevel(gradeLevel),
                                                                              spells(std::move(spells)) {}

    SpellHand(SpellHand&& other) noexcept: gradeLevel(other.gradeLevel), spells(std::move(other.spells)){}
    SpellHand(const SpellHand& other): gradeLevel(other.gradeLevel){
        for (const auto& spell : other.spells) {
            if (spell) {
                spells.push_back(spell->clone());
            }
        }
    }

    SpellHand& operator=(const SpellHand& other) {
        if (this != &other) {
            gradeLevel = other.gradeLevel;
            spells.clear();
            for(const std::unique_ptr<Spell> &spell : other.spells){
                spells.push_back(spell.get()->clone());
            }
        }
        return *this;
    }

    SpellHand& operator=(SpellHand&& other) noexcept {
        if (this != &other) {
            gradeLevel = other.gradeLevel;
            spells = std::move(other.spells);
        }
        return *this;
    }

    virtual void addSpell(std::unique_ptr<Spell> spell) noexcept final;
    virtual const std::vector<std::unique_ptr<Spell>>& getSpells() const final;
    virtual void useSpellWithoutAnyCoordsBinding(int position) final; //позиция от 0 до n-1, заклинание без привязки к чьему-либо местоположению
    virtual void useSpellWithAIMBinding(int position, Field& field, Constants::XYPair from, Constants::XYPair to) final; //позиция от 0 до n-1, использовать заклинание с привязкой к местоположению игрока и координатам цели (цель не факт что там есть)

    virtual std::string serialize() noexcept final;
    static SpellHand deserialize(std::map<std::string, std::string> json, SpellFactory& spellFactory) noexcept;


private:
    int gradeLevel;
    std::vector<std::unique_ptr<Spell>> spells;
    virtual bool checkSpellPositionAvailability(int position) const final;


};


#endif //LABAOOP2_SPELLHAND_H