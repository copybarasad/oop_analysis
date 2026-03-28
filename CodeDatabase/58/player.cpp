#include "player.hpp"

Player::Player(int hp, int damage, TypeAttack type_attack, int size_hand)
    : Entity(hp, damage, Fraction::BLUE), type_attack(type_attack), hand(size_hand) {};

int Player::validate_points(int p){
    if (p < MIN_POINTS || p > MAX_POINTS){
        p = BASE_POINTS;
    }
    return p;
}

int Player::get_points() {return points;};
void Player::add_points(int p) {points += validate_points(p);};
void Player::set_points(int p) {points = validate_points(p);};

void Player::switch_slowed(){
    is_slowed = !is_slowed;
}
bool Player::has_slowed(){return is_slowed == true;};

void Player::switch_attack(){
    if(get_type_attack() == TypeAttack::MELEE){
        set_type_attack(TypeAttack::RANGED);
    } else{
        set_type_attack(TypeAttack::MELEE);
    }
}
TypeAttack Player::get_type_attack() {return type_attack;};
void Player::set_type_attack(TypeAttack type_attack) {this->type_attack = type_attack;};


void Player::print_avaible_cards() {
    std::vector<ISpellCard*> cards = hand.get_cards();

    if (cards.empty()){
        std::cout << "your hand is empty" << std::endl;
    }

    int i = 0;
    for (auto card : cards){
        std::cout << "# " << i++ << " card: " << card->get_name() << std::endl;
    }
}

PlayerHand& Player::get_hand() {return hand;};

int Player::get_damage(){
    int base_dmg = get_base_damage();
    if (type_attack == TypeAttack::RANGED){
        base_dmg /= 2;
    }

    return base_dmg;
}
char Player::get_symbol() {return 'P';};

void Player::update_player(){
    std::string improve;

    std::cout << "what will we improve? (hp, damage): ";
    std::cin >> improve;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::transform(improve.begin(), improve.end(), improve.begin(), ::toupper);

    while(true){
        if(improve == "HP"){
            int new_hp = get_hp() * 2;
            if (new_hp >= 999) new_hp = 999;
            set_hp(new_hp);
            set_max_hp(new_hp);
            break;
        } else if(improve == "DAMAGE"){
            int new_dmg = get_base_damage() * 2;
            if (new_dmg >= 999) new_dmg = 999;
            set_base_damage(new_dmg);
            set_start_base_demage(new_dmg);
            break;
        } else{
            std::cout << "error: write hp/damage: ";
            std::cin >> improve;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::transform(improve.begin(), improve.end(), improve.begin(), ::toupper);
            continue;
        }
    }
}
