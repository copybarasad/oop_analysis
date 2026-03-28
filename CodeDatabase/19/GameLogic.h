#pragma once


#include <unordered_set>

#include "entity/Entity.h"
#include "entity/types/Player.h"
#include "LevelConfiguration.h"
#include "core/render/particle/ParticleHelper.h"

class Game;

inline constexpr double PLAYER_ACTION_DELAY = 0.1;

class GameLogic {
    friend class Game;
    friend class GameSave;

public:
    explicit GameLogic(Game &game)
        : game_(game), particle_helper_(game) {
    }

    void damage_entity(Entity *entity, int damage);

    bool move_entity(Entity *entity, int target_x, int target_y);

    bool add_entity(Entity *entity);

    void handle_player_click(int x, int y);

    void toggle_spell_focus();

    void handle_next_spell() const;

    void handle_prev_spell() const;

    void handle_select_spell(int index) const;

    void switch_attack_mode();

    int get_wave() const {
        return wave_;
    }

    void set_wave(int wave);

    const World &world() const;

    Player &player() const;

    bool spell_focus() const {
        return spell_focus_;
    }

    bool is_upgrade_purchased(const int idx) const {
        return current_purchased_upgrades_.contains(idx);
    }

    void set_upgrade_purchased(const int idx) {
        current_purchased_upgrades_.insert(idx);
    }

    void clear_purchased_upgrades() {
        current_purchased_upgrades_.clear();
    }

    std::vector<std::string> available_upgrades() const {
        return current_available_upgrades_;
    }

    void roll_available_upgrades();

    int get_upgrade_level(const std::string &id);

    void set_upgrade_level(const std::string &id, int level);

    int get_next_spell_level() {
        return 1 + player().get_spell_holder().get_spell_buff() + get_upgrade_level("spell_upgrade");
    }

    ParticleHelper &particle_helper() {
        return particle_helper_;
    }

private:
    Game &game_;
    std::vector<Entity *> to_delete_;
    float since_last_action_ = 0;
    bool spell_focus_ = false;
    size_t wave_ = 1;

    std::map<std::string, int> upgrades_;
    std::vector<std::string> current_available_upgrades_;
    std::unordered_set<int> current_purchased_upgrades_;
    ParticleHelper particle_helper_;

    GameLogic &operator=(const GameLogic &) = delete;

    GameLogic &operator=(GameLogic &&) = delete;

    void update(float dt);

    void init_level();

    LevelConfiguration get_level_config(int level) const;

public:
    virtual ~GameLogic();
};
