#ifndef ISPELL_CARD_H
#define ISPELL_CARD_H

#include <string>
#include <cmath>

class GameField;
class Player;

enum class SpellType {
    DIRECT_DAMAGE,
    AREA_DAMAGE,
    TRAP,
    SUMMON,
    ENHANCEMENT,
    COUNT
};

class ISpell {
public:
    ISpell(SpellType type, int range, int mana_cost, int level = 1) 
        : type_(type), range_(range), mana_cost_(mana_cost), level_(level) {}
    
    virtual ~ISpell() = default;
    
    bool can_cast(int dx, int dy, int player_mana) const {
        double distance = std::sqrt(dx*dx + dy*dy);
        if(distance > std::sqrt(range_*range_ + 1)) return false;
        return player_mana >= mana_cost_;
    }
    
    virtual bool cast_by_player(GameField& field, Player& player, int target_x = -1, int target_y = -1) = 0;
    virtual bool needs_target() const { return true; }
    virtual std::string get_name() const = 0;
    virtual std::string get_description() const = 0;
    virtual void reset_to_base() = 0;
    virtual bool upgrade() = 0;
    virtual std::string get_upgrade_info() const = 0;
    
    int get_level() { return level_; }
    int get_mana_cost() const { return mana_cost_; }
    int get_range() const { return range_; }
    SpellType get_type() const { return type_; }

protected:
    SpellType type_;
    int range_;
    int mana_cost_;
    int level_;
};

#endif