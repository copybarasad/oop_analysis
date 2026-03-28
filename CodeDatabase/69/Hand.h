#pragma once
#include "Position.h"
#include <memory>
#include <vector>
#include <string>


class Player;
class Game;
class Spell;

class Hand {
public:
    explicit Hand(size_t capacity);
    
    bool AddSpell(std::unique_ptr<Spell> s);
    bool UseSpell(size_t index, Player& caster, Game& game, Position target);
    
    std::vector<std::string> GetSpellNames() const;
    Spell* GetSpell(size_t index);

    size_t Size() const;
    size_t Capacity() const;

    static std::unique_ptr<Spell> CreateRandomSpell();

    void Clear();
    void RemoveAt(size_t index);

    //*****
    static std::unique_ptr<Spell> CreateSpellByName(const std::string& name);
private:
    size_t capacity_;
    std::vector<std::unique_ptr<Spell>> spells_;

    void GenerateRandomSpell();
};