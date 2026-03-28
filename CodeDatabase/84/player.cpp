#include "player.h"
#include <iostream>
#include <cmath>
#include "directdamagespell.h"
#include "aoe_damagespell.h"
#include "trapspell.h"
#include "lightning_spell.h"

static Weapon sword(WeaponName::Sword, 15, WeaponType::Melee);
static Weapon bow(WeaponName::Bow, 8, WeaponType::Ranged, 3);

Player::Player(int health, int x, int y, int hand_size, int mana)
    : Entity(health, &sword, x, y), 
      score_(100), experience_(0), enemies_defeated_(0),
      mana_(mana), max_mana_(mana),
      current_weapon_(&sword),
      spell_hand_(hand_size) {
    
    learn_random_starting_spell();
}

void Player::add_score(int points) { score_ += points; }
void Player::add_experience(int exp) { experience_ += exp; }
void Player::add_enemy_defeated() { enemies_defeated_++; }
int Player::get_score() const { return score_; }
int Player::get_experience() const { return experience_; }
int Player::get_enemies_defeated() const { return enemies_defeated_; }
int Player::get_mana() const { return mana_; }
int Player::get_max_mana() const { return max_mana_; }

bool Player::switch_to_melee() {
    current_weapon_ = &sword;
    set_weapon(&sword);
    return true;
}

bool Player::switch_to_ranged() {
    current_weapon_ = &bow;
    set_weapon(&bow); 
    return true;
}

const Weapon* Player::get_current_weapon() const { return current_weapon_; }

bool Player::attack(Entity* target) const {
    if (!target || !target->is_alive() || !current_weapon_) {
        return false;
    }
    int distance = std::abs(get_x() - target->get_x()) + std::abs(get_y() - target->get_y());
    if (distance <= current_weapon_->get_range()) {
        target->take_damage(current_weapon_->get_damage());
        return true;
    }
    return false;
}

void Player::heal(int amount) {
    int new_health = get_health() + amount;
    if (new_health > get_max_health()) new_health = get_max_health();
    set_health(new_health);
}

bool Player::learn_spell(std::unique_ptr<Spell> spell) {
    return spell_hand_.add_spell(std::move(spell));
}

bool Player::cast_spell(int spell_index, Field& field, int target_x, int target_y) {
    Spell* spell = spell_hand_.get_spell(spell_index);
    if (!spell) {
        std::cout << "Неверный номер заклинания!" << std::endl;
        return false;
    }
    if (!can_cast_spell(spell->get_mana_cost())) {
        std::cout << "Недостаточно маны для применения " << spell->get_name() << "!" << std::endl;
        return false;
    }
    bool cast_success = spell->cast(field, target_x, target_y);
    if (cast_success) {
        spend_mana(spell->get_mana_cost());
    }
    return cast_success;
}

void Player::print_spells() const { spell_hand_.print_spells(); }
const SpellHand& Player::get_spell_hand() const { return spell_hand_; }
bool Player::can_cast_spell(int mana_cost) const { return mana_ >= mana_cost; }
void Player::spend_mana(int amount) { mana_ -= amount; if (mana_ < 0) mana_ = 0; }
void Player::restore_mana(int amount) { mana_ += amount; if (mana_ > max_mana_) mana_ = max_mana_; }

void Player::learn_random_starting_spell() {
    auto random_spell = spell_hand_.get_random_spell();
    if (random_spell) spell_hand_.add_spell(std::move(random_spell));
}

bool Player::acquire_random_spell() {
    if (spell_hand_.is_full()) return false;
    auto random_spell = spell_hand_.get_random_spell();
    if (random_spell) return spell_hand_.add_spell(std::move(random_spell));
    return false;
}

bool Player::can_buy_spell() const { return score_ >= 30 && !spell_hand_.is_full(); }

bool Player::buy_spell() {
    if (!can_buy_spell()) {
        if (score_ < 30) std::cout << "Недостаточно очков! Нужно 30 (у вас " << score_ << ")." << std::endl;
        else if (spell_hand_.is_full()) std::cout << "Рука полна заклинаний!" << std::endl;
        return false;
    }
    
    std::cout << "=== МАГАЗИН ЗАКЛИНАНИЙ ===" << std::endl;
    std::cout << "Доступные заклинания (стоимость: 30 очков):" << std::endl;
    std::cout << "1. Огненная стрела - Урон: 25, Дистанция: 4, Мана: 10" << std::endl;
    std::cout << "2. Огненный шар - Урон: 15 (AOE), Дистанция: 2, Мана: 15" << std::endl;
    std::cout << "3. Ловушка - Урон: 20, Дистанция установки: 1, Мана: 15" << std::endl;
    std::cout << "4. Удар молнии - Урон: 20 + Оглушение, Дистанция: 3, Мана: 20" << std::endl;
    std::cout << "Выберите заклинание (1-4) или нажмите Enter для выхода: ";
    
    std::string choice;
    std::getline(std::cin, choice);
    
    if (choice.empty()) return false;

    std::unique_ptr<Spell> spell;

    if (choice == "1") {
        spell = std::make_unique<DirectDamageSpell>(
            "Огненная стрела", "Наносит урон одному врагу", 25, 4, 10);
    } else if (choice == "2") {
        spell = std::make_unique<AoEDamageSpell>(
            "Огненный шар", "Наносит урон по области", 15, 2, 2, 15);
    } else if (choice == "3") {
        spell = std::make_unique<TrapSpell>(
            "Ловушка", "Устанавливает ловушку на поле", 20, 1, 15);
    } else if (choice == "4") {
        spell = std::make_unique<LightningSpell>(
            "Удар молнии", "Наносит урон и оглушает", 20, 3, 1, 20);
    } else {
        std::cout << "Неверный выбор!" << std::endl;
        return false;
    }

    for (int i = 0; i < spell_hand_.get_spell_count(); ++i) {
        Spell* existing_spell = spell_hand_.get_spell(i);
        if (existing_spell && existing_spell->get_name() == spell->get_name()) {
            std::cout << "У вас уже есть это заклинание!" << std::endl;
            return false;
        }
    }
    
    score_ -= 30;
    bool success = spell_hand_.add_spell(std::move(spell));
    
    if (success) {
        std::cout << "Заклинание успешно куплено!" << std::endl;
    } else {
        score_ += 30; 
        std::cout << "Ошибка: Не удалось добавить заклинание." << std::endl;
    }
    
    return success;
}

void Player::save(std::ostream& os) const {
    Entity::save(os);
    os << " " << score_ << " " << experience_ << " " << enemies_defeated_ << " " << mana_ << " " << max_mana_ << "\n";
    
    os << spell_hand_.get_spell_count() << "\n";
    for(int i=0; i < spell_hand_.get_spell_count(); ++i) {
        Spell* spell = spell_hand_.get_spell(i);
        os << static_cast<int>(spell->get_type()) << "\n";
    }
}

void Player::load(std::istream& is) {
    Entity::load(is);
    is >> score_ >> experience_ >> enemies_defeated_ >> mana_ >> max_mana_;
    
    current_weapon_ = &sword; 

    spell_hand_.clear_hand();
    int spell_count;
    is >> spell_count;
    
    for(int i=0; i < spell_count; ++i) {
        int type_int;
        is >> type_int;
        SpellType type = static_cast<SpellType>(type_int);
        
        std::unique_ptr<Spell> new_spell;
        switch(type) {
            case SpellType::DirectDamage:
                new_spell = std::make_unique<DirectDamageSpell>("Огненная стрела", "Восстановлено", 25, 4, 10);
                break;
            case SpellType::AoEDamage:
                new_spell = std::make_unique<AoEDamageSpell>("Огненный шар", "Восстановлено", 15, 2, 2, 15);
                break;
            case SpellType::Trap:
                new_spell = std::make_unique<TrapSpell>("Ловушка", "Восстановлено", 20, 1, 15);
                break;
            case SpellType::Lightning:
                new_spell = std::make_unique<LightningSpell>("Удар молнии", "Восстановлено", 20, 3, 1, 20);
                break;
        }
        if(new_spell) spell_hand_.add_spell(std::move(new_spell));
    }
}

void Player::prepare_for_next_level() {
    heal(max_health_); 
    
    int current_spells = spell_hand_.get_spell_count();
    int to_remove = current_spells / 2;
    if (to_remove > 0) {
        spell_hand_.remove_random_spells(to_remove);
        std::cout << "При переходе на новый уровень потеряно заклинаний: " << to_remove << std::endl;
    }
}