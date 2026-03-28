#include <PlaceTrapSpell.hpp>
#include <Constants.hpp>
#include <PlayerTrap.hpp>
#include <ncurses.h>
#include <cmath>
#include <Player.hpp>

PlaceTrapSpell::PlaceTrapSpell() : name("Place Trap"), cost(PLACE_TRAP_COST), type(SpellType::PlaceTrap) {}

std::string PlaceTrapSpell::getName() const {
    return name;
}

int PlaceTrapSpell::getCost() const {
    return cost;
}

SpellType PlaceTrapSpell::getType() const {
    return type;
}

bool PlaceTrapSpell::apply(std::shared_ptr<Player> player, std::vector<std::vector<Ceil>>& ceils, std::vector<std::shared_ptr<LivingEntity>>& entities, const ScreenSize* screen) {
    int playerX = player->getX();
    int playerY = player->getY();
    int plMp = player->getMp();

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
        if (ch == ESC) { ceils[startWorldY][startWorldX].getEntity()->draw(); refresh(); return false; }
        int prevScreenX = cursorScreenX; int prevScreenY = cursorScreenY;
        switch (ch) { case UP: cursorScreenY--; break; case DOWN: cursorScreenY++; break; case LEFT: cursorScreenX--; break; case RIGHT: cursorScreenX++; break; default: continue; }
        int nextWorldY = cursorScreenY-height/2+mapHeight/2; int nextWorldX = cursorScreenX-width/2+mapWidth/2;
        int distY = std::abs(cursorScreenY - playerY); int distX = std::abs(cursorScreenX - playerX);
        if (!(nextWorldY>=1 && nextWorldY<mapHeight-1 && nextWorldX>=1 && nextWorldX<mapWidth-1 && distY<=PLACE_TRAP_RADIUS && distX<=PLACE_TRAP_RADIUS*2 && ceils[nextWorldY][nextWorldX].getEntity()->getType() == Entity::Type::FLOOR)) {
            cursorScreenX = prevScreenX; cursorScreenY = prevScreenY;
        }
        if (prevScreenX != cursorScreenX || prevScreenY != cursorScreenY) {
            int prevWorldY = prevScreenY-height/2+mapHeight/2; int prevWorldX = prevScreenX-width/2+mapWidth/2;
            ceils[prevWorldY][prevWorldX].getEntity()->draw();
            int currentWorldY=cursorScreenY-height/2+mapHeight/2; int currentWorldX = cursorScreenX-width/2+mapWidth/2;
            attron(A_REVERSE); ceils[currentWorldY][currentWorldX].getEntity()->draw(); attroff(A_REVERSE); refresh();
        }
    }
    int finalWorldY = cursorScreenY - height/2 + mapHeight/2; int finalWorldX = cursorScreenX - width/2 + mapWidth/2;

    int charges = player->getEmpowermentCharges();

    auto newTrap = std::make_shared<PlayerTrap>(cursorScreenX, cursorScreenY);
    if (charges > 0) {
        newTrap->empower(charges); 
    }

    ceils[finalWorldY][finalWorldX].setEntity(newTrap);
    ceils[finalWorldY][finalWorldX].getEntity()->draw();

    if (charges > 0) {
        player->clearEmpowermentCharges();
    }

    refresh();
    return true;
}