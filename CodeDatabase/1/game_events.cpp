#include "game_events.h"
#include <sstream>

GameLogEvent::GameLogEvent(const std::string& type, const std::string& message)
    : type_(type), message_(message) {}

std::string GameLogEvent::toString() const {
    return message_;
}

std::string GameLogEvent::getType() const {
    return type_;
}

DamageEvent::DamageEvent(const std::string& attacker, const std::string& target,
                         int damage, int remaining_hp)
    : attacker_(attacker), target_(target), 
      damage_(damage), remaining_hp_(remaining_hp) {}

std::string DamageEvent::toString() const {
    std::stringstream ss;
    ss << "[Урон] " << attacker_ << " → " << target_ 
       << ": -" << damage_ << " HP (осталось: " << remaining_hp_ << ")";
    return ss.str();
}

PlayerMoveEvent::PlayerMoveEvent(const std::string& player, int x, int y,
                               const std::string& direction)
    : player_(player), x_(x), y_(y), direction_(direction) {}

std::string PlayerMoveEvent::toString() const {
    std::stringstream ss;
    ss << "[Движение] " << player_ << " → (" << x_ << "," << y_ 
       << ") направление: " << direction_;
    return ss.str();
}

SpellCastEvent::SpellCastEvent(const std::string& caster, 
                               const std::string& spell_name)
    : caster_(caster), spell_name_(spell_name) {}

std::string SpellCastEvent::toString() const {
    std::stringstream ss;
    ss << "[Заклинание] " << caster_ << " применяет " << spell_name_;
    return ss.str();
}

DeathEvent::DeathEvent(const std::string& character)
    : character_(character) {}

std::string DeathEvent::toString() const {
    std::stringstream ss;
    ss << "[Смерть] " << character_ << " погиб";
    return ss.str();
}

GeneralEvent::GeneralEvent(const std::string& category, 
                           const std::string& message)
    : category_(category), message_(message) {}

std::string GeneralEvent::toString() const {
    std::stringstream ss;
    ss << "[" << category_ << "] " << message_;
    return ss.str();
}