#include "Player.h"
#include "Spell.h"
#include "Hand.h"


Player::Player(int maxHealth, int damage):
    health(maxHealth),
    maxHealth(maxHealth),
    attackDamage(damage),
    posX(0),
    posY(0),
    hand(5)
{    
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int choice = std::rand() % 2;
    if (choice == 0) {
        hand.AddSpell(new DirectDamageSpell(5, 3)); 
    } else {
        hand.AddSpell(new AreaDamageSpell(3, 2));
    }

}

void Player::Attack(GameField& field) {
    int px = GetX();
    int py = GetY();

    for (Enemy* e : field.GetEnemies()) {
        int ex = e->GetX();
        int ey = e->GetY();

        if (abs(ex - px) <= 1 && abs(ey - py) <= 1) {
            e->TakeDamage(attackDamage);
            std::cout << "Attacked enemy at (" << ex << ", " << ey << ") for " << attackDamage << " damage!\n";

            if (!e->IsAlive()) {
                std::cout << "Enemy at (" << ex << ", " << ey << ") is defeated!\n";
                field.RemoveEnemy(e); 
            }
        }
    }
}



bool Player::IsAlive() const {
    return health > 0;
}

void Player::Move(int dx, int dy) {
    posX += dx;
    posY += dy;
}

void Player::TakeDamage(int damage_amount) {
    health -= damage_amount;
    if (health < 0){
        health = 0;
    }
}

int Player::GetX() const {
    return posX;
}

int Player::GetY() const {
    return posY;
}

std::pair<int, int> Player::GetPos() const {
    return {posX, posY};
}

int Player::GetHealth() const {
    return health;
}

int Player::GetDamage() const {
    return attackDamage;
}

void Player::UseSpell(int index, GameField* field, int targetX, int targetY) {
    Spell* spell = hand.GetSpell(index);
    if (spell) {
        spell->Use(this, field, targetX, targetY);
    }
}