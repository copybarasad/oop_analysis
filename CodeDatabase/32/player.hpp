#ifndef PLAYER_H
#define PLAYER_H

#include "hero.hpp"
#include "potion.hpp"
#include "field.hpp"
#include "hand.hpp"

#include <string>
#include <iostream>
#include <vector>

#define RANGE_MODE_OFF 0


class Enemy;
class Player: public Hero{
private:
    int score_;
    int level_;
    std::vector<Potion> bag_;
    int ranged_mode_ = RANGE_MODE_OFF;
    int x_;
    int y_;
    bool slowed_ = false;
    Hand hand_;
    int kills_ = 0;
    bool enhanced_ = false;
    int max_health_;
public:
    explicit Player(const std::string& name, int health, int damage, size_t hand_capacity = 5);
    void ToggleMode();
    bool IsRanged() const;
    void TakeDamage(int amount) override;
    void Attack(Hero& target) override;

    void SetPosition(int x, int y);
    int GetX() const;
    int GetY() const;

    bool Move(int dx, int dy, Field& field, const std::vector<Enemy>& enemies);

    void PickUpPotion(const Potion& potion); 
    void UsePotion(); 
    void UsePotion(int index);
    void PrintInventory() const;
    void PrintStatus() const;

    bool IsSlowed() const;
    void SetSlowed(bool value);

    Hand& GetHand() noexcept {return hand_;}
    const Hand& GetHand() const noexcept {return hand_; }

    void AddKill();

    void ActivateEnhancement() { enhanced_ = true; }
    bool HasEnhancement() const { return enhanced_; }
    void ConsumeEnhancement() { enhanced_ = false; }

    void SetRangedMode(bool on) {
    ranged_mode_ = on ? 1 : 0;
    }

    int GetMaxHealth() const { return max_health_; }
    void SetMaxHealth(int mh) { max_health_ = mh; }

    bool skip_next_turn_ = false;
    bool ShouldSkipTurn() const { return skip_next_turn_; }
    void SetSkipTurn(bool v) { skip_next_turn_ = v; }
};

#endif