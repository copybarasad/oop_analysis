#ifndef SPELL_H
#define SPELL_H

#include "GameField.h"
#include <string>
#include <memory>

enum class SpellType {
    DIRECT_DAMAGE,
    AREA_DAMAGE,
    TRAP
};

class ISpell {
public:
    virtual ~ISpell() = default;
    virtual void cast(Position target, GameField& field) = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual int getManaCost() const = 0;
    virtual int getRange() const = 0;
    virtual SpellType getType() const = 0;
};

class SpellCard {
private:
    std::unique_ptr<ISpell> spell;
    bool used;

public:
    SpellCard(std::unique_ptr<ISpell> spell);
    ~SpellCard() = default;
    
    SpellCard(const SpellCard&) = delete;
    SpellCard& operator=(const SpellCard&) = delete;
    SpellCard(SpellCard&&) = default;
    SpellCard& operator=(SpellCard&&) = default;

    bool cast(Position target, GameField& field);
    bool isUsed() const;
    void reset();
    
    std::string getName() const;
    std::string getDescription() const;
    int getManaCost() const;
    int getRange() const;
    SpellType getType() const;
};

#endif