#ifndef HAND_H_
#define HAND_H_

#include <vector>
#include <memory>
#include <string>
#include "spell.h"

class Player;
class Field;

class Hand {
public:
    Hand(int maxSize);
    bool IsFull() const { return spells_.size() >= maxSize_; }
    int GetSize() const { return spells_.size(); }
    void AddSpell(std::unique_ptr<Spell> spell);
    bool UseSpell(int index, Player& player, Field& field, int targetX, int targetY);
    void Display() const;
    void RemoveSpell(int index);
    void Clear() { spells_.clear(); }
    const std::vector<std::unique_ptr<Spell>>& GetSpells() const { return spells_; }
    std::vector<std::string> GetSpellNames() const {
        std::vector<std::string> names;
        for (const auto& s : spells_) {
            names.push_back(s->GetName());
        }
        return names;
    }
    std::vector<std::string> GetSpellTypes() const;
    void LoadFromTypes(const std::vector<std::string>& types);

private:
    int maxSize_;
    std::vector<std::unique_ptr<Spell>> spells_;
};

#endif // HAND_H_