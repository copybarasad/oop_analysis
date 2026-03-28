#ifndef SPELL_H
#define SPELL_H

#include "Position.h"
#include <memory>
#include <string>

class GameField;
class Player;

class Spell {
public:
    virtual ~Spell() = default;
    virtual std::string getName() const = 0;
    virtual int getManaCost() const = 0;
    virtual bool cast(const Position& target, GameField& field, const Position& casterPos) = 0;
    virtual std::string getDescription() const = 0;
    virtual void enhance() = 0;
    virtual bool isEnhancement() const { return false; }
    
    virtual int getEnhancementLevel() const = 0;
    virtual void setEnhancementLevel(int level) = 0;
    virtual int getEnhancedRadius() const = 0;
    virtual int getEnhancedAreaSize() const = 0;
    virtual int getEnhancedDamage() const = 0;
    virtual int getEnhancedSummonCount() const = 0;
};

class DirectDamageSpell : public Spell {
public:
    DirectDamageSpell(int damage = 30, int radius = 3);
    virtual ~DirectDamageSpell() = default;
    
    std::string getName() const override;
    int getManaCost() const override;
    bool cast(const Position& target, GameField& field, const Position& casterPos) override;
    std::string getDescription() const override;
    void enhance() override;
    
    int getEnhancementLevel() const override;
    void setEnhancementLevel(int level) override;
    int getEnhancedRadius() const override;
    int getEnhancedAreaSize() const override;
    int getEnhancedDamage() const override;
    int getEnhancedSummonCount() const override;

private:
    std::string name;
    int manaCost;
    int enhancementLevel;
    int damage;
    int radius;
};

class AreaDamageSpell : public Spell {
public:
    AreaDamageSpell(int damage = 20, int radius = 2, int areaSize = 2);
    virtual ~AreaDamageSpell() = default;
    
    std::string getName() const override;
    int getManaCost() const override;
    bool cast(const Position& target, GameField& field, const Position& casterPos) override;
    std::string getDescription() const override;
    void enhance() override;
    
    int getEnhancementLevel() const override;
    void setEnhancementLevel(int level) override;
    int getEnhancedRadius() const override;
    int getEnhancedAreaSize() const override;
    int getEnhancedDamage() const override;
    int getEnhancedSummonCount() const override;

private:
    std::string name;
    int manaCost;
    int enhancementLevel;
    int damage;
    int radius;
    int areaSize;
};

class TrapSpell : public Spell {
public:
    TrapSpell(int damage = 25, int radius = 2);
    virtual ~TrapSpell() = default;
    
    std::string getName() const override;
    int getManaCost() const override;
    bool cast(const Position& target, GameField& field, const Position& casterPos) override;
    std::string getDescription() const override;
    void enhance() override;
    
    int getEnhancementLevel() const override;
    void setEnhancementLevel(int level) override;
    int getEnhancedRadius() const override;
    int getEnhancedAreaSize() const override;
    int getEnhancedDamage() const override;
    int getEnhancedSummonCount() const override;

private:
    std::string name;
    int manaCost;
    int enhancementLevel;
    int damage;
    int radius;
};

class SummonSpell : public Spell {
public:
    SummonSpell(int summonCount = 1, int radius = 1);
    virtual ~SummonSpell() = default;
    
    std::string getName() const override;
    int getManaCost() const override;
    bool cast(const Position& target, GameField& field, const Position& casterPos) override;
    std::string getDescription() const override;
    void enhance() override;
    
    int getEnhancementLevel() const override;
    void setEnhancementLevel(int level) override;
    int getEnhancedRadius() const override;
    int getEnhancedAreaSize() const override;
    int getEnhancedDamage() const override;
    int getEnhancedSummonCount() const override;

private:
    std::string name;
    int manaCost;
    int enhancementLevel;
    int summonCount;
    int radius;
};

class EnhancementSpell : public Spell {
public:
    EnhancementSpell();
    virtual ~EnhancementSpell() = default;
    
    std::string getName() const override;
    int getManaCost() const override;
    bool cast(const Position& target, GameField& field, const Position& casterPos) override;
    std::string getDescription() const override;
    void enhance() override;
    bool isEnhancement() const override;
    
    int getEnhancementLevel() const override;
    void setEnhancementLevel(int level) override;
    int getEnhancedRadius() const override;
    int getEnhancedAreaSize() const override;
    int getEnhancedDamage() const override;
    int getEnhancedSummonCount() const override;

private:
    std::string name;
    int manaCost;
    int enhancementLevel;
    int enhancementPower;
};

#endif
