#include "ise_ball.hpp"
#include "player.hpp"
IseBall::IseBall() : Spell("IseBall", "Deals 15 damage to a target within 4 cells."), damage(15), range(4){}


void IseBall::cast_spell(Player& player, int dx, int dy, 
    std::vector<Enemy>& enemies, const GameField& field) {
    int attack_x = player.get_x();
    int attack_y = player.get_y();
    for (int i = 1; i <= range; i++) {
        attack_x += dx;
        attack_y += dy;
        if (attack_x < 0 || attack_x >= field.getWidth() || 
            attack_y < 0 || attack_y >= field.getHeight()) {
            break;
        }
        if (!field.getField()[attack_x][attack_y].get_isPass()) {
            break;
        }
        for (auto& enemy : enemies) {
            if (enemy.is_alive() && enemy.get_x() == attack_x && enemy.get_y() == attack_y) {
                enemy.take_damage(damage);
                std::cout << "Ice Ball hit enemy for " << damage << " damage!" << std::endl;
                
                if (!enemy.is_alive()) {
                    player.add_score(10);
                    std::cout << "Enemy defeated! +10 score" << std::endl;
                }
                return;
            }
        }
    }
    
    std::cout << "No enemies hit - spell wasted" << std::endl;
}

std::string IseBall::getType() const {
    return "DIRECT_DAMAGE";
}