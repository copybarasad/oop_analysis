#include "earthquake.hpp"
#include "player.hpp"

EarthquakeSpell::EarthquakeSpell() 
    : Spell("Earthquake", "Deals 8 damage in 2x2 area around player"), 
      damage(8) {}

void EarthquakeSpell::cast_spell(Player& player, int dx, int dy, 
                                std::vector<Enemy>& enemies, const GameField& field) {
    int playerX = player.get_x();
    int playerY = player.get_y();
    int enemiesHit = 0;
    for (int x = playerX - 1; x <= playerX + 1; x++) {
        for (int y = playerY - 1; y <= playerY + 1; y++) {
            if (x < 0 || x >= field.getWidth() || y < 0 || y >= field.getHeight()) {
                continue;
            }
            for (auto& enemy : enemies) {
                if (enemy.is_alive() && enemy.get_x() == x && enemy.get_y() == y) {
                    enemy.take_damage(damage);
                    enemiesHit++;
                    if (!enemy.is_alive()) {
                        player.add_score(10);
                    }
                }
            }
        }
    }
    if (enemiesHit > 0) {
        std::cout << "Earthquake hit " << enemiesHit << " enemies!" << std::endl;
    } else {
        std::cout << "No enemies in earthquake area" << std::endl;
    }
}

std::string EarthquakeSpell::getType() const {
    return "AREA_DAMAGE";
}