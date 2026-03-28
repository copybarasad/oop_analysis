#include <SiegeArrow.hpp>
#include <Constants.hpp>
#include <Entity.hpp>
#include <ncurses.h>
#include <memory>
#include <Floor.hpp>
#include <EnemyBase.hpp>
#include <Player.hpp>

SiegeArrow::SiegeArrow() : name("Siege Arrow"), cost(SIEGE_ARROW_COST), type(SpellType::SiegeArrow) {}

SpellType SiegeArrow::getType() const {
    return type;
}

std::string SiegeArrow::getName() const {
    return name;
}

int SiegeArrow::getCost() const {
    return cost;
}

bool SiegeArrow::apply(std::shared_ptr<Player> player, std::vector<std::vector<Ceil>>& ceils, std::vector<std::shared_ptr<LivingEntity>>& entities, const ScreenSize* screen) {
    int playerX = player->getX();
    int playerY = player->getY();
    int plMp = player->getMp();

    int charges = player->getEmpowermentCharges();

    int castRadius = SIEGE_ARROW_RADIUS + charges;

    int mapHeight = ceils.size();
    int mapWidth = ceils[0].size();
    int height = screen->yMax;
    int width = screen->xMax;

    int cursorScreenX = playerX;
    int cursorScreenY = playerY;

    int startWorldY = cursorScreenY-height/2+mapHeight/2; int startWorldX = cursorScreenX-width/2+mapWidth/2;
    attron(A_REVERSE); ceils[startWorldY][startWorldX].getEntity()->draw(); attroff(A_REVERSE); refresh();

    int ch = 0;
    while ((ch = getch()) != ENTER) {
        if (ch == ESC) {
            int currentWorldY = cursorScreenY-height/2+mapHeight/2; int currentWorldX = cursorScreenX-width/2+mapWidth/2;
            ceils[currentWorldY][currentWorldX].getEntity()->draw(); refresh();
            return false;
        }

        int prevScreenX = cursorScreenX; int prevScreenY = cursorScreenY;
        int nextScreenX = cursorScreenX; int nextScreenY = cursorScreenY;
        switch (ch) { case UP: nextScreenY--; break; case DOWN: nextScreenY++; break; case LEFT: nextScreenX--; break; case RIGHT: nextScreenX++; break; default: continue; }

        int nextWorldY = nextScreenY - height/2 + mapHeight/2;
        int nextWorldX = nextScreenX - width/2 + mapWidth/2;
        int distY = std::abs(nextScreenY - playerY);
        int distX = std::abs(nextScreenX - playerX);

        if (nextWorldY >= 0 && nextWorldY < mapHeight &&
            nextWorldX >= 0 && nextWorldX < mapWidth &&
            distY <= castRadius && distX <= castRadius * 2 && 
            ceils[nextWorldY][nextWorldX].getEntity()->getType() != Entity::Type::WALL)
        {
            cursorScreenX = nextScreenX;
            cursorScreenY = nextScreenY;
        }

        if (prevScreenX != cursorScreenX || prevScreenY != cursorScreenY) {

            int prevWorldY = prevScreenY-height/2+mapHeight/2; int prevWorldX = prevScreenX-width/2+mapWidth/2;
            ceils[prevWorldY][prevWorldX].getEntity()->draw();
            int currentWorldY = cursorScreenY-height/2+mapHeight/2; int currentWorldX = cursorScreenX-width/2+mapWidth/2;
            attron(A_REVERSE); ceils[currentWorldY][currentWorldX].getEntity()->draw(); attroff(A_REVERSE); refresh();
        }
    }

    int finalWorldY = cursorScreenY - height/2 + mapHeight/2;
    int finalWorldX = cursorScreenX - width/2 + mapWidth/2;
    auto target = ceils[finalWorldY][finalWorldX].getEntity();

    ceils[finalWorldY][finalWorldX].getEntity()->draw(); 

    bool targetWasValid = false; 
    if(target->getType() == Entity::Type::ENEMY_BASE) {

        ceils[finalWorldY][finalWorldX].setEntity(std::make_shared<Floor>(cursorScreenX, cursorScreenY)); 
        std::static_pointer_cast<EnemyBase>(target)->destroyed();
        ceils[finalWorldY][finalWorldX].getEntity()->draw();
        targetWasValid = true;
    } else {
        mvprintw(height - 10, width / 2 - 22, "Incorrect target. Can only target enemy bases");
        refresh();
        napms(1000);
        flushinp();

        return false;
    }

    if (charges > 0 && targetWasValid) {
        player->clearEmpowermentCharges();
    }

    refresh();
    return true; 
}