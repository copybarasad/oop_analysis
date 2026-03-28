#include "player.h"
#include "enemy.h"
#include "tower.h"

Player::Player(std::string name_, int maxHealth_, int health_, int points_, int damage_, int x_, int y_)
: BasicInfo(maxHealth_, health_, damage_, x_, y_, points_), name(name_), spellHand()
{
    if (points < 0) points = 0;
    sword_damage = damage;
    bow_damage = damage / 2;
    weapon = 1;
}

void Player::increase_points(int value){
    points += value;
}

int Player::change_weapon(){
    if(weapon == 1){
        damage = bow_damage;
        weapon = 2;
    } else {
        damage = sword_damage;
        weapon = 1;
    }
    return 1;
}

int Player::attack(Enemy& enemy){
    if (weapon == 1){
        int target_x = enemy.get_x();
        int target_y = enemy.get_y();
        int res_x = x;
        int res_y = y;

        if (x < target_x) res_x++;
        else if (x > target_x) res_x--;

        if (y < target_y) res_y++;
        else if (y > target_y) res_y--;

        if (res_x == target_x && res_y == target_y){
            enemy.damage_recieved(get_damage());
            if (enemy.check_health()){
                increase_points(1);
                return 5;
            }
            return 1;
        }
        return 0;
    } else {
        enemy.damage_recieved(get_damage());
        if (enemy.check_health()){
            increase_points(1);
            return 5;
        }
        return 1;
    }
}

int Player::attack(Tower& tower){
    if (weapon == 1){
        int target_x = tower.get_x();
        int target_y = tower.get_y();
        int res_x = x;
        int res_y = y;

        if (x < target_x) res_x++;
        else if (x > target_x) res_x--;

        if (y < target_y) res_y++;
        else if (y > target_y) res_y--;

        if (res_x == target_x && res_y == target_y){
            tower.damage_recieved(get_damage());
            if (tower.check_health()){
                increase_points(3);
                return 5;
            }
            return 1;
        }
        return 0;
    } else {
        tower.damage_recieved(get_damage());
        if (tower.check_health()){
            increase_points(3);
            return 5;
        }
        return 1;
    }
}

void Player::print_current_stats(){
    std::cout << "Тип: Игрок" << "    Очки: " << points << '\n';
    BasicInfo::print_current_stats();
}

void Player::move(int x_, int y_){
    this->x = x_;
    this->y = y_;
}

int Player::use_spell(int num, int target_x, int target_y, Place& place, std::vector<Enemy>& enemies, Tower& tower,  MagicTower& magic_tower){
    return spellHand.use_spell(num, *this, target_x, target_y, place, enemies, tower, magic_tower);
}
int Player::buy_spell(){
    return spellHand.buy_spell(*this);
}

void Player::getInfo(){
    spellHand.getInfo();
}

int Player::remove_random_spell(){
    int n = spellHand.remove_random_spell();
    return n;
}
void Player::heal(){
    health = maxHealth;
}
void Player::increase_max_health(int n){
    maxHealth += n;
}
void Player::increase_damage(int n){
    damage += n;
}
std::vector<int> Player::get_spell_types() const {
    return spellHand.get_spell_types();
}

void Player::load_spells(const std::vector<int>& spell_types) {
    spellHand.load_spells(spell_types);
}

void Player::get_spell_info(){
    spellHand.getInfo();
}
int Player::upgrade_spell(int value){
    std::cout << "\n--- УЛУЧШЕНИЕ ЗАКЛИНАНИЙ ---\n";
    spellHand.getInfo();
    
    std::cout << "Выберите номер заклинания для улучшения (1-" << spellHand.getSize() + 1<< "): ";
    int spell_choice;
    std::cin >> spell_choice;
    spell_choice--;
    
    if (spell_choice < 0 || spell_choice >= spellHand.getSize() + 1) {
        std::cout << "Неверный номер заклинания\n";
        return 0;
    }
    spellHand.upgrade(spell_choice, value);
    std::cout << "Заклинание улучшено! Дальность i атаки увеличена.\n";
    return 1;
    
}
void Player::to_start(){
    x = 2;
    y = 2;
}
Player::~Player(){}