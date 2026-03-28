#include <RainOfArrows.hpp> 
#include <Constants.hpp>
#include <Entity.hpp>
#include <Floor.hpp>
#include <Enemy.hpp>
#include <EnemyBase.hpp>
#include <ncurses.h>
#include <memory>
#include <Player.hpp>
#include <cmath> 

RainOfArrows::RainOfArrows() : name("Rain of arrows"), cost(RAIN_OF_ARROWS_COST), type(SpellType::RainOfArrows), damage(ARROW_DAMAGE) {}

std::string RainOfArrows::getName() const {
    return name;
}

int RainOfArrows::getCost() const {
    return cost;
}

SpellType RainOfArrows::getType() const {
    return type;
}

void RainOfArrows::showRadius(int centerY, int centerX, int radius, std::vector<std::vector<Ceil>>& ceils, bool switcher) {
    if (ceils.empty() || ceils[0].empty()) {
        return; 
    }
    int mapHeight = ceils.size();
    int mapWidth = ceils[0].size();

    if (switcher) {
        attron(A_REVERSE); 
    }

    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            int currentY = centerY + i;
            int currentX = centerX + j;

            if (currentY >= 0 && currentY < mapHeight &&
                currentX >= 0 && currentX < mapWidth) {

                ceils[currentY][currentX].getEntity()->draw();
            }
        }
    }

    if (switcher) {
        attroff(A_REVERSE); 
    }
    refresh();
}

bool RainOfArrows::apply(std::shared_ptr<Player> player, std::vector<std::vector<Ceil>>& ceils, std::vector<std::shared_ptr<LivingEntity>>& entities, const ScreenSize* screen) {
    int playerX = player->getX();
    int playerY = player->getY();
    int plMp = player->getMp();

    int charges = player->getEmpowermentCharges();

    int effectRadius = 1 + charges;

    int mapHeight = ceils.size();
    int mapWidth = ceils[0].size();
    int height = screen->yMax;
    int width = screen->xMax;

    int cursorScreenX = playerX;
    int cursorScreenY = playerY;

    int cursorWorldY = cursorScreenY - height / 2 + mapHeight / 2;
    int cursorWorldX = cursorScreenX - width / 2 + mapWidth / 2;

    showRadius(cursorWorldY, cursorWorldX, effectRadius, ceils, true);

    int ch = 0;
    while ((ch = getch()) != ENTER) {
        if (ch == ESC) {
            showRadius(cursorWorldY, cursorWorldX, effectRadius, ceils, false);
            return false; 
        }

        int prevWorldY = cursorWorldY;
        int prevWorldX = cursorWorldX;

        int nextScreenX = cursorScreenX; int nextScreenY = cursorScreenY;
        switch (ch) { case UP: nextScreenY--; break; case DOWN: nextScreenY++; break; case LEFT: nextScreenX--; break; case RIGHT: nextScreenX++; break; default: continue; }

        int nextWorldY = nextScreenY - height/2 + mapHeight/2; 
        int nextWorldX = nextScreenX - width/2 + mapWidth/2;
        int distY = std::abs(nextScreenY - playerY); 
        int distX = std::abs(nextScreenX - playerX);
        const int MAX_CAST_RADIUS = 3; 

        if (nextWorldY >= 1 && nextWorldY < mapHeight-1 && nextWorldX >= 1 && nextWorldX < mapWidth-1 && distY <= MAX_CAST_RADIUS && distX <= MAX_CAST_RADIUS*2) {
            cursorScreenX = nextScreenX; 
            cursorScreenY = nextScreenY; 
            cursorWorldY = nextWorldY; 
            cursorWorldX = nextWorldX;
        }

        if (prevWorldX != cursorWorldX || prevWorldY != cursorWorldY) {
            showRadius(prevWorldY, prevWorldX, effectRadius, ceils, false);
            showRadius(cursorWorldY, cursorWorldX, effectRadius, ceils, true);
        }
    }

    bool targetHit = false;

    for (int i = -effectRadius; i <= effectRadius; i++) {
        for (int j = -effectRadius; j <= effectRadius; j++) {
            int currentY = cursorWorldY + i;
            int currentX = cursorWorldX + j;

            if (currentY >= 0 && currentY < mapHeight && currentX >= 0 && currentX < mapWidth) {
                auto target = ceils[currentY][currentX].getEntity();
                if (target->getType() == Entity::Type::ENEMY) {
                    auto enemy = std::static_pointer_cast<Enemy>(target);

                    enemy->takeDamage(damage);
                    targetHit = true;

                    if (enemy->getHealth() <= 0) {
                        ceils[currentY][currentX].setEntity(std::make_shared<Floor>(target->getX(), target->getY()));
                    }
                }
            }
        }
    }

    showRadius(cursorWorldY, cursorWorldX, effectRadius, ceils, false);

    if (charges > 0) {
        player->clearEmpowermentCharges();
    }

    refresh();
    return true; 
}