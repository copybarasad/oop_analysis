#include "player.h"
#include "game_field.h"
#include "constants.h"
#include "hand.h"
#include "enums.h"
#include "spell_area_damage.h"
#include "spell_direct_damage.h"
#include "spell_trap.h" 
#include "spell_summon.h"
#include "spell_buff.h"
#include "event_system.h"
#include <iostream>
#include <random>

Player::Player(const std::string& player_name, Game_field* game_field)
    : name(player_name), health(DEFAULT_PLAYER_HEALTH), 
      max_health(DEFAULT_PLAYER_HEALTH), 
      melee_damage(DEFAULT_PLAYER_MELEE_DAMAGE), 
      ranged_damage(DEFAULT_PLAYER_RANGED_DAMAGE), 
      score(0), level(1), 
      combat_mode(Combat_mode::MELEE), 
      is_slowed(false),
      mana(PLAYER_START_MANA),
      max_mana(PLAYER_MAX_MANA),
      spell_hand(SPELL_HAND_SIZE),
      field(game_field){ 
}

std::string Player::get_name() const{
    return name;
}

int Player::get_health() const{
    return health;
}

int Player::get_max_health() const{
    return max_health;
}

int Player::get_damage() const{
    return (combat_mode == Combat_mode::MELEE) ? melee_damage : ranged_damage;
}

int Player::get_score() const{
    return score;
}

int Player::get_level() const{
    return level;
}

Combat_mode Player::get_combat_mode() const{
    return combat_mode;
}

bool Player::get_is_slowed() const{
    return is_slowed;
}

int Player::get_melee_damage() const{
    return melee_damage;
}

int Player::get_ranged_damage() const{
    return ranged_damage;
}

void Player::set_health(int new_health){
    if (new_health <= max_health){
        health = new_health;
    } else{
        health = max_health;
    }
    if (health <= 0){
        health = 0;
    }
}

void Player::set_slowed(bool slowed){
    is_slowed = slowed;
}

Move_status Player::move(int delta_x, int delta_y){
    if (is_slowed){
        std::cout << name << " не может двигаться (замедлен)!\n";
        return Move_status::BLOCKED;
    }
    if (field){
        return field->move_player(delta_x, delta_y);
    }
    return Move_status::BLOCKED; 
}

void Player::take_damage(int damage_amount){
    if (damage_amount > 0){
        health -= damage_amount;
        Event_system::get_instance().notify(
            Game_event(Event_type::PLAYER_DAMAGED, name, "враг", damage_amount));
        std::cout << name << " получает " << damage_amount  << " урона! ";
        std::cout << "Здоровье: " << health << "/" << max_health << "\n";
        if (health <= 0){
            health = 0;
            std::cout << name << " погиб!\n";
        }
    }
}

void Player::heal(){
    if (health < max_health){ 
        health += HEAL_AMOUNT;
        if (health > max_health){
            health = max_health;
        }
        Event_system::get_instance().notify(Game_event(Event_type::PLAYER_HEALED, name, "", HEAL_AMOUNT));
        std::cout << name << " восстанавливает " << HEAL_AMOUNT << " HP. ";
        std::cout << "Здоровье: " << health << "/" << max_health << "\n";
    }
}

void Player::increase_score(int points){
    if (points > 0){
        score += points;
        std::cout << name << " получает " << points << " очков. ";
        std::cout << "Общий счет: " << score << "\n";
    }
}

void Player::level_up(){
    level++;
    max_health += PLAYER_HEALTH_PER_LEVEL;        
    health = max_health;
    melee_damage += PLAYER_MELEE_DAMAGE_PER_LEVEL;  
    ranged_damage += PLAYER_RANGED_DAMAGE_PER_LEVEL;
    std::cout << name << " достиг уровня " << level << "\n";
    std::cout << "Макс. здоровье: " << max_health << "\n";
    std::cout << "Ближний урон: " << melee_damage << "\n";
    std::cout << "Дальний урон: " << ranged_damage << "\n";
}

void Player::switch_combat_mode(){
    combat_mode = (combat_mode == Combat_mode::MELEE) ? Combat_mode::RANGED : Combat_mode::MELEE;
    std::string mode_name = (combat_mode == Combat_mode::MELEE) ? "БЛИЖНИЙ БОЙ" : "ДАЛЬНИЙ БОЙ";
    int current_damage = get_damage();
    std::cout << name << " переключается на " << mode_name << "\n";
    std::cout << "Текущий урон: " << current_damage << "\n";
}

void Player::display_status() const{
    std::cout << "\n Статус игрока \n";
    std::cout << "Имя: " << name << "\n";
    std::cout << "Уровень: " << level << "\n";
    std::cout << "Здоровье: " << health << "/" << max_health << "\n";
    std::cout << "Режим боя: " << ((combat_mode == Combat_mode::MELEE) ? "Ближний" : "Дальний") << "\n";
    std::cout << "Урон: " << get_damage() << " (ближ: " << melee_damage << ", даль: " << ranged_damage << ")\n";
    std::cout << "Очки: " << score << "\n";
    std::cout << "Мана: " << mana << "/" << max_mana << "\n";
    std::cout << "Состояние: " << (health > 0 ? "Жив" : "Мертв") << "\n";
    if (is_slowed) std::cout << " [ЗАМЕДЛЕН]";
}

void Player::perform_attack(){
    std::string mode_name = (combat_mode == Combat_mode::MELEE) ? "ближней" : "дальней";
    std::cout << name << " совершает " << mode_name << " атаку!\n";
}

// Методы для маны
int Player::get_mana() const{
    return mana;
}

int Player::get_max_mana() const{
    return max_mana;
}

void Player::set_mana(int new_mana){ 
    mana = (new_mana < 0) ? 0 : (new_mana > max_mana) ? max_mana : new_mana; 
}

void Player::restore_mana(int amount){ 
    set_mana(mana + amount); 
}

bool Player::spend_mana(int amount){
    if (mana >= amount) {
        mana -= amount;
        return true;
    }
    return false;
}

// Методы для заклинаний
Hand& Player::get_spell_hand(){
    return spell_hand;
}

const Hand& Player::get_spell_hand() const {
    return spell_hand;
}

bool Player::cast_spell(int spell_index, int target_x, int target_y){    
    if (!field) {
        std::cout << "ОШИБКА: field is nullptr!\n";
        return false;
    }    
    Spell_card* spell = spell_hand.get_spell(spell_index);
    if (!spell) {
        std::cout << "Заклинание не найдено!\n";
        return false;
    }
    if (Spell_buff::has_buff() && typeid(*spell) != typeid(Spell_buff)){ //бафф
        Spell_buff::apply_buff(spell);
    }
    if (!spend_mana(spell->get_mana_cost())) {
        std::cout << "Недостаточно маны! Нужно: " << spell->get_mana_cost() 
                  << ", есть: " << mana << "\n";
        return false;
    }
    spell->cast(*this, target_x, target_y, *field);
    std::cout << "Заклинание " << spell->get_name() << " успешно применено!\n";
    return true;
}
/*
bool Player::learn_spell(std::unique_ptr<Spell_card> spell){
    return spell_hand.add_spell(std::move(spell));
}
*/

bool Player::learn_spell(std::unique_ptr<Spell_card> spell) {
    if (spell_hand.add_spell(std::move(spell))) {
        auto spell_name = spell_hand.get_spell(spell_hand.get_spell_count() - 1)->get_name();
        
        Event_system::get_instance().notify(Game_event(Event_type::SPELL_LEARNED, name, spell_name));
        std::cout << "Заклинание " << spell_name << " добавлено в руку.\n";
        return true;
    }
    return false;
}

void Player::learn_random_spell(){
    std::unique_ptr<Spell_card> spell;
    int spell_type = rand() % 5;
    
    switch(spell_type) {
        case 0:
            spell = std::unique_ptr<Spell_card>(new Spell_direct_damage());
            break;
        case 1:
            spell = std::unique_ptr<Spell_card>(new Spell_area_damage());
            break;
        case 2:
            spell = std::unique_ptr<Spell_card>(new Spell_trap());
            break;
        case 3:
            spell = std::unique_ptr<Spell_card>(new Spell_summon());
            break;
        case 4:
            spell = std::unique_ptr<Spell_card>(new Spell_buff());
            break;
        default:
            spell = std::unique_ptr<Spell_card>(new Spell_direct_damage());
            break;
    }
    
    if (learn_spell(std::move(spell))) {
        std::cout << "Выучено новое заклинание: " << spell_hand.get_spell(spell_hand.get_spell_count() - 1)->get_name() << "\n";
    }
}

void Player::add_random_spell(){    
    if (spell_hand.get_spell_count() >= spell_hand.get_max_size()) {
        std::cout << "DEBUG: Hand полон, выходим" << std::endl;
        return;
    }
    learn_random_spell();
    std::cout << "DEBUG: === КОНЕЦ add_random_spell ===" << std::endl;
}

bool Player::buy_spell(int cost){
    if (score >= cost) {
        score -= cost;
        add_random_spell();
        return true;
    }
    return false;
}

//улучшения при поднятии уровня

void Player::upgrade_max_health(int amount){
    max_health += amount;
    health += amount;
    std::cout <<  "Максимальное здоровье увеличено до " << max_health << "\n";
}

void Player::upgrade_melee_damage(int amount){
    melee_damage += amount;
}

void Player::upgrade_ranged_damage(int amount){
    ranged_damage += amount;
}

void Player::upgrade_max_mana(int amount){
    max_mana += amount;
    mana += amount;
}

void Player::enhance_random_spell(){ //возможно надо доработать
    auto& hand = get_spell_hand();
    if (hand.get_spell_count() > 0){
        size_t random_index = rand() % hand.get_spell_count();
        hand.get_spell(random_index)->enhance();
    }
}