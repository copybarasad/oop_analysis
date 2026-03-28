#include "player.h"
#include "logger.h"
#include "direct_spell.h"
#include "area_spell.h"
#include "trap_spell.h"
#include "create_spell.h"
#include "levelup_spell.h"
#include <cstdlib>
#include <ctime>

Player::Player(int init_health, int close_dmg, int far_dmg, int hand_size)
    : health(init_health), 
      close_damage(close_dmg), 
      far_damage(far_dmg),
      score(0),
      current_mode(Mode::CLOSE),
      mode_switch_cooldown(false),
      is_slowed(false),
      slowed_this_turn(false),
      hand(hand_size),
      enemies_killed(0) {
        if(init_health <= 0){
            init_health = 1;
        }
        if (close_dmg < 0) {
            close_dmg = 20;
        }else if(far_dmg < 0){
            far_dmg = 10;
        }
        add_random_spell();
      }

Player::Player(const Player& other)
    : health(other.health),
      close_damage(other.close_damage),
      far_damage(other.far_damage),
      score(other.score),
      current_mode(other.current_mode),
      mode_switch_cooldown(other.mode_switch_cooldown),
      is_slowed(other.is_slowed),
      slowed_this_turn(other.slowed_this_turn),
      hand(other.hand),
      enemies_killed(other.enemies_killed){}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        health = other.health;
        close_damage = other.close_damage;
        far_damage = other.far_damage;
        score = other.score;
        current_mode = other.current_mode;
        mode_switch_cooldown = other.mode_switch_cooldown;
        is_slowed = other.is_slowed;
        slowed_this_turn = other.slowed_this_turn;
        hand = other.hand;
        enemies_killed = other.enemies_killed;
    }
    return *this;
}

Player::Player(Player&& other) noexcept
    : health(other.health),
      close_damage(other.close_damage),
      far_damage(other.far_damage),
      score(other.score),
      current_mode(other.current_mode),
      mode_switch_cooldown(other.mode_switch_cooldown),
      is_slowed(other.is_slowed),
      slowed_this_turn(other.slowed_this_turn),
      hand(std::move(other.hand)),
      enemies_killed(other.enemies_killed){}

Player& Player::operator=(Player&& other) noexcept {
    if (this != &other) {
        health = other.health;
        close_damage = other.close_damage;
        far_damage = other.far_damage;
        score = other.score;
        current_mode = other.current_mode;
        mode_switch_cooldown = other.mode_switch_cooldown;
        is_slowed = other.is_slowed;
        slowed_this_turn = other.slowed_this_turn;
        hand = std::move(other.hand);
        enemies_killed = other.enemies_killed;
    }
    return *this;
}

void Player::take_damage(int damage) {
    if (damage < 0) {
        damage = 0;
    }
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

bool Player::is_alive() const {
    return health > 0;
}

int Player::get_health() const {
    return health;
}

Mode Player::get_mode() const {
    return current_mode;
}

void Player::switch_mode() {
    if (!mode_switch_cooldown) {
        if (current_mode == Mode::CLOSE) {
            current_mode = Mode::FAR;
        } else {
            current_mode = Mode::CLOSE;
        }
        mode_switch_cooldown = true;
    }
}

bool Player::has_mode_cooldown() const {
    return mode_switch_cooldown;
}

void Player::clear_mode_cooldown() {
    mode_switch_cooldown = false;
}

int Player::get_current_damage() const {
    if(current_mode == Mode::CLOSE){
        return close_damage;
    }
    return far_damage;
}

void Player::apply_slowdown() {
    is_slowed = true;
    slowed_this_turn = false;
}

void Player::slowdown_turn() {
    if (is_slowed && !slowed_this_turn) {
        slowed_this_turn = true;
    } else if (is_slowed && slowed_this_turn) {
        is_slowed = false;
        slowed_this_turn = false;
    }
}

bool Player::is_currently_slowed() const {
    return is_slowed;
}

int Player::get_score() const {
    return score;
}

void Player::add_score(int points) {
    score += points;
}

void Player::increment_killed() {
    enemies_killed++;
    if (enemies_killed % 2 == 0) add_random_spell();
}

void Player::add_random_spell() {
    if (hand.is_full()) return;
    int type = rand() % 5;
    std::string name;

    if (type == 0) {
        hand.add_spell(make_unique<TrapSpell>());
        name = "TrapSpell";
    } else if (type == 1) {
        hand.add_spell(make_unique<DirectDamageSpell>());
        name = "DirectDamageSpell";
    } else if (type == 2){
        hand.add_spell(make_unique<AreaDamageSpell>());
        name = "AreaDamageSpell";
    }else if (type == 3){
        hand.add_spell(make_unique<CreateSpell>());
        name = "CreateSpell";
    }else{
        hand.add_spell(make_unique<LevelupSpell>());
        name = "LevelupSpell";
    }

    if (auto lg = global_logger()) {
        lg->log("Player received spell: " + name);
    }
}

const Hand& Player::get_hand() const {
    return hand;
}
Hand& Player::get_hand() {
    return hand;
}

void Player::add_level_up() {
    level_up++;
}

int Player::get_level() const {
    return level_up;
}

void Player::consume_levels() {
    level_up = 0;
}

void Player::upgrade_max_health() {
    health += 20;
}
void Player::upgrade_close_damage() {
    close_damage += 5;
}
void Player::upgrade_far_damage() {
    far_damage += 3;
}