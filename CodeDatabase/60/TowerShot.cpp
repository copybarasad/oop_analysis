#include <TowerShot.hpp>
#include <Constants.hpp>
#include <ncurses.h>
#include <Player.hpp>

TowerShot::TowerShot() 
    : name("Tower Arrow Shot"), cost(0), type(SpellType::TowerShot), damage(TOWER_DAMAGE) {}

std::string TowerShot::getName() const { return name; }
int TowerShot::getCost() const { return cost; }
SpellType TowerShot::getType() const { return type; }

bool TowerShot::apply(std::shared_ptr<Player>, std::vector<std::vector<Ceil>>&, std::vector<std::shared_ptr<LivingEntity>>& entities, const ScreenSize*) {
    return false;
}

bool TowerShot::apply(std::shared_ptr<Player> player) {
    if (player) {
        player->takeDamage(damage);
        return true;
    }
    return false;
}