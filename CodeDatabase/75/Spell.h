#pragma once
#include <string>
#include <functional>

class Character;
class Game;

enum class SpellTarget {
    SELF,
    ENEMY,
    AREA,
    NO_TARGET,
    SHIELD
};

enum class SpellType {
    FIREBALL,
    ICE_SHARD,
    HEAL,
    LIGHTNING,
    SHIELD,
    TRAP,   
    THORNS,
    TELEPORT, 
    NOVA,     
    LIFEDRAIN,
    
    // NEW
    SUMMON_ALLY,
    POISON_CLOUD,
    TIME_STOP,
    METEOR,
    RAGE
};

namespace SpellConstants {
    constexpr int SHIELD_MANA_COST = 5; 
    constexpr int SHIELD_BASE_DURATION_MS = 20000;
    constexpr int SHIELD_DURATION_PER_LEVEL_MS = 5000;
}

class Spell {
private:
    static constexpr int MIN_MANA_COST = 1;
    static constexpr float MANA_COST_REDUCTION_PER_LEVEL = 0.9f;
    static constexpr int BASE_UPGRADE_COST = 50;

public:
    static constexpr int MAX_LEVEL = 5;

    Spell(SpellType type, int level = 1);
    Spell(const std::string& name, const std::string& description, int manaCost,
          SpellTarget targetType, std::function<void(Character* caster, Character* target, Game* game)> effect,
          int level = 1);

    const std::string& getName() const;
    const std::string& getDescription() const;
    int getManaCost() const;
    SpellTarget getTargetType() const;
    int getLevel() const;
    SpellType getType() const { return m_type; }

    bool cast(Character* caster, Character* target, Game* game) const;
    void upgrade();
    bool canUpgrade() const;
    int getUpgradeCost() const;

private:
    SpellType m_type;
    std::string m_name;
    std::string m_description;
    int m_manaCost;
    SpellTarget m_targetType;
    std::function<void(Character* caster, Character* target, Game* game)> m_effect;
    int m_level;
};