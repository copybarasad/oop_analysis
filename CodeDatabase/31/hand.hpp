#include "spell.hpp"
#include <vector>
#include "entities/entity.hpp"
#include "entities/player.hpp"
#include <cstring>

enum class Spells {direct_damage, trap, area_damage, ally_call, upgrade, health, TotalCount};

struct SpellInfo {
    const char* name;
    int base_cost;
};

class Hand {
    int max_count {5};
    std::vector<Spell *> spells;
    Player &player;
    Field *field;
    int count_upgrades {0};
    std::vector<Entity*> last_pending_allies;
    
    static const SpellInfo spell_prices[6];
    int get_spell_cost(int index) const;
    Spell* create_random_spell();
    
public:
    static int base_spell_level;
    Hand(Field &field, Player &player, int max_count = 5, int initial_spells = 1);
    Hand(nlohmann::json &j, Field &field, Player &player, int max_count = 5);
    void show_hand();
    void show_spells_count();
    void show_spells_str(int index);
    void print_pricelist();
    void show_shop_str(int index);
    void buy_spell(int spell_number);
    bool spell_needs_cursor(int index);
    bool prepare_spell(int index);
    int get_spell_area_size(int index);
    int get_spell_radius(int index);
    Point get_player_pos();
    bool use_spell(int index, Point point);
    int get_spell_count() const;
    void remove_half_spells();
    void level_up_spells();
    void level_up_base_spells();
    std::vector<Entity*> get_pending_allies(int index);
    std::vector<Entity*> get_last_pending_allies();
    void update_field(Field &new_field);
    void transfer_spells_to(Hand &other_hand);
    void show_shop_spell_info(int index);
    void show_hand_spell_info(int index);

    nlohmann::json save();
};