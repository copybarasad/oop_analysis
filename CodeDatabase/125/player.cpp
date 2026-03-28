#include "player.h"
#include "enemy_manager.h"  
#include "game_field.h"     
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <random> 

Player::Player() : Player("Герой", {0, 0}) {}

Player::Player(const std::string& name, Position start)
    : name_(name), position_(start), health_(100), max_health_(100),
      score_(0), melee_damage_(15), ranged_damage_(5), combat_mode_(CombatMode::Melee),
      hand_(3) {}

bool Player::ProcessAction(char command, GameField& field, EnemyManager& em) {
    if (command == 'm') {
        SwitchCombatMode();
        return true;  
    }

    Position new_pos = position_;
    switch (command) {
        case 'w': new_pos.y--; break;
        case 's': new_pos.y++; break;
        case 'a': new_pos.x--; break;
        case 'd': new_pos.x++; break;
        default: 
            std::cout << "Неизвестная команда!\n";
            return false;  
    }

    if (field.IsWalkable(new_pos)) {
        if (CheckEnemyCollision(em, new_pos)) {
            return true;
        } else {
            SetPosition(new_pos);
            return true;
        }
    }
    
    std::cout << "Невозможно переместиться в эту клетку!\n";
    return true; 
}

void Player::Attack(EnemyManager& em, GameField& field) {
    char dir;
    std::cout << "Направление атаки (w/a/s/d): ";
    std::cin >> dir;

    Position target = position_;
    switch (dir) {
        case 'w': target.y--; break;
        case 's': target.y++; break;
        case 'a': target.x--; break;
        case 'd': target.x++; break;
        default:
            std::cout << "Неверное направление!\n";
            return;
    }

    bool hit = false;

    for (auto& e : em.GetEnemies()) {
        if (!e.IsAlive()) continue;

        int dist = abs(e.GetPosition().x - position_.x) + abs(e.GetPosition().y - position_.y);

        if ((combat_mode_ == CombatMode::Melee && e.GetPosition() == target)
            || (combat_mode_ == CombatMode::Ranged && dist <= 4)) {
            e.TakeDamage(GetCurrentDamage());
            std::cout << "Вы нанесли " << GetCurrentDamage() << " урона врагу!\n";
            hit = true;

            if (!e.IsAlive()) {
                std::cout << "Враг уничтожен! +10 очков!\n";
                AddScore(10);
            }
        }
    }

    auto& towers = em.GetTowers();  
    for (auto& tower : towers) {
        if (!tower.IsAlive()) continue;

        int dist = abs(tower.GetPosition().x - position_.x) + abs(tower.GetPosition().y - position_.y);

        if ((combat_mode_ == CombatMode::Melee && tower.GetPosition() == target)
            || (combat_mode_ == CombatMode::Ranged && dist <= 4)) {
            tower.TakeDamage(GetCurrentDamage());
            std::cout << "Вы нанесли " << GetCurrentDamage() << " урона башне!\n";
            hit = true;

            if (!tower.IsAlive()) {
                std::cout << "Башня уничтожена! +20 очков!\n";
                AddScore(20);
            }
        }
    }

    if (!hit) {
        std::cout << "Атака не достигла цели!\n";
    }
}

bool Player::CheckEnemyCollision(EnemyManager& em, Position pos) {
    for (auto& e : em.GetEnemies()) {
        if (e.IsAlive() && e.GetPosition() == pos) {
            e.TakeDamage(GetCurrentDamage());
            std::cout << "Вы нанесли " << GetCurrentDamage() << " урона врагу!\n";
            
            if (!e.IsAlive()) {
                std::cout << "Враг уничтожен!\n";
                AddScore(10);
            }
            return true;
        }
    }
    return false;
}

Position Player::GetPosition() const { return position_; }
void Player::SetPosition(const Position& pos) { position_ = pos; }

int Player::GetCurrentDamage() const {
	return (combat_mode_ == CombatMode::Melee) ? melee_damage_ : ranged_damage_;
}

void Player::SwitchCombatMode() {
	combat_mode_ = (combat_mode_ == CombatMode::Melee) ? CombatMode::Ranged : CombatMode::Melee;
	std::cout << "Вы переключились на "
			  << (combat_mode_ == CombatMode::Melee ? "БЛИЖНИЙ" : "ДАЛЬНИЙ")
			  << " бой. Это заняло один ход.\n";
}

void Player::TakeDamage(int dmg) {
	health_ = std::max(0, std::min(max_health_, health_ - dmg));
}

bool Player::BuySpell(SpellType type) {
    const int SPELL_COST = 10;
    
    if (score_ >= SPELL_COST) {
        if (hand_.AddSpecificSpell(type)) {
            score_ -= SPELL_COST;
            return true;
        }
    }
    return false;
}

void Player::ResetHealth() { health_ = max_health_; }

void Player::RemoveRandomHalfSpells() {
    auto& hand = GetHand();
    size_t size = hand.GetSize();
    if (size <= 1) return;

    size_t to_remove = size / 2;
    std::vector<size_t> indices;
    for (size_t i = 0; i < size; ++i) indices.push_back(i);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);

    for (size_t i = 0; i < to_remove; ++i) {
        size_t idx = indices[i]; 
        
        const Spell* spell = hand.GetSpell(idx);
        if (!spell) continue;
        
        std::string spell_name = spell->GetName();
        hand.RemoveSpell(idx);
        std::cout << "Удалено заклинание: " << spell_name << "\n";
    }
}

void Player::ClearHand() {
    hand_.Clear();  
}

void Player::AddScore(int points) { score_ += points; }

int Player::GetHealth() const { return health_; }
int Player::GetMaxHealth() const { return max_health_; }
int Player::GetScore() const { return score_; }
CombatMode Player::GetCombatMode() const { return combat_mode_; }
const std::string& Player::GetName() const { return name_; }
bool Player::IsAlive() const { return health_ > 0; }
const PlayerHand& Player::GetHand() const { return hand_; }
PlayerHand& Player::GetHand() { return hand_; } 
