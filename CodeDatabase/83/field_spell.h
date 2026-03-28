#ifndef FIELD_SPELL_H
#define FIELD_SPELL_H

#include <string>

class FieldSpell {
public:
    FieldSpell(int x, int y, const std::string& spellName) 
        : x(x), y(y), spellName(spellName), collected(false) {}
    
    int getX() const { return x; }
    int getY() const { return y; }
    const std::string& getSpellName() const { return spellName; }
    bool isCollected() const { return collected; }
    void collect() { collected = true; }
    
private:
    int x, y;
    std::string spellName;
    bool collected;
};

#endif