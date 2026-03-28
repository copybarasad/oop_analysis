#pragma once
#include "event_system.h"
#include <string>

class GameLogEvent : public GameEvent {
private:
    std::string type_;
    std::string message_;
    
public:
    GameLogEvent(const std::string& type, const std::string& message);
    std::string toString() const override;
    std::string getType() const override;
};


class DamageEvent : public GameEvent {
private:
    std::string attacker_;
    std::string target_;
    int damage_;
    int remaining_hp_;
    
public:
    DamageEvent(const std::string& attacker, const std::string& target,
                int damage, int remaining_hp);
    std::string toString() const override;
    std::string getType() const override { return "DAMAGE"; }
};


class PlayerMoveEvent : public GameEvent {
private:
    std::string player_;
    int x_, y_;
    std::string direction_;
    
public:
    PlayerMoveEvent(const std::string& player, int x, int y,
                   const std::string& direction);
    std::string toString() const override;
    std::string getType() const override { return "MOVE"; }
};


class SpellCastEvent : public GameEvent {
private:
    std::string caster_;
    std::string spell_name_;
    
public:
    SpellCastEvent(const std::string& caster, const std::string& spell_name);
    std::string toString() const override;
    std::string getType() const override { return "SPELL"; }
};

class DeathEvent : public GameEvent {
private:
    std::string character_;
    
public:
    explicit DeathEvent(const std::string& character);
    std::string toString() const override;
    std::string getType() const override { return "DEATH"; }
};

class GeneralEvent : public GameEvent {
private:
    std::string category_;
    std::string message_;
    
public:
    GeneralEvent(const std::string& category, const std::string& message);
    std::string toString() const override;
    std::string getType() const override { return category_; }
};