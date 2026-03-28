#pragma once

#include "entity/Entity.h"
#include "entity/attack/AttackMode.h"
#include "spell/SpellHolder.h"

class Player : public Entity {
public:
    Player(int x, int y);

    void render(const IRenderer & renderer, const World &world, int center_x, int center_y) override;

    void switch_attack_mode() {
        if (attackMode_ == AttackMode::Melee) {
            attackMode_ = AttackMode::Ranged;
        } else {
            attackMode_ = AttackMode::Melee;
        }
    }

    AttackMode get_attack_mode() const {
        return attackMode_;
    }

    SpellHolder &get_spell_holder() {
        return spell_holder_;
    }

    int score() const {
        return score_;
    }

    void inc_score(int val) {
        score_ += std::max(val, 0);
    }

    void reset_score() {
        score_ = 0;
    }

    bool is_hostile() const override {
        return false;
    }

    Player &operator=(const Player &) = delete;

    Player &operator=(Player &&) = delete;


    json serialize() const override {
        json data = Entity::serialize();
        data["score"] = score_;
        data["attack_mode"] = attackMode_;
        data["spell_holder"] = spell_holder_.serialize();

        return data;
    }

    bool deserialize(json data) override {
        if (!data.contains("score") || !data.contains("attack_mode") || !data.contains("spell_holder")) {
            return false;
        }
        score_ = data["score"];
        attackMode_ = data["attack_mode"];
        if (!spell_holder_.deserialize(data["spell_holder"])) {
            return false;
        }

        return Entity::deserialize(data);
    }

    int move_priority() const override {
        return 0;
    }
private:
    AttackMode attackMode_ = AttackMode::Melee;
    SpellHolder spell_holder_ = SpellHolder(5);
    int score_ = 0;
};
