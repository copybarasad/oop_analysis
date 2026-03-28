#include <SummonDecoySpell.hpp>
#include <Constants.hpp>
#include <DecoyAlly.hpp>
#include <Player.hpp>
#include <ncurses.h>
#include <cmath>
#include <random>
#include <algorithm>

SummonDecoySpell::SummonDecoySpell() : name(SUMMON_DECOY_NAME), cost(SUMMON_DECOY_COST), type(SpellType::SummonDecoy) {}

std::string SummonDecoySpell::getName() const { return name; }
int SummonDecoySpell::getCost() const { return cost; }
SpellType SummonDecoySpell::getType() const { return type; }

bool SummonDecoySpell::apply(std::shared_ptr<Player> player,
                           std::vector<std::vector<Ceil>>& ceils, 
                           std::vector<std::shared_ptr<LivingEntity>>& entities, 
                           const ScreenSize* screen) {
    int playerX = player->getX();
    int playerY = player->getY();
    int plMp = player->getMp();

    int mapHeight = ceils.size();
    int mapWidth = ceils[0].size();
    int height = screen->yMax;
    int width = screen->xMax;

    int cursorScreenX = playerX;
    int cursorScreenY = playerY;

    mvprintw(height - 1, 20, "Select a destination for the shadow(s) and press ENTER.");
    refresh();

    int startWorldY = cursorScreenY - height/2 + mapHeight/2;
    int startWorldX = cursorScreenX - width/2 + mapWidth/2;

    attron(A_REVERSE); ceils[startWorldY][startWorldX].getEntity()->draw(); attroff(A_REVERSE); refresh();

    int ch = 0;
    while ((ch = getch()) != ENTER) {
        if (ch == ESC) {
            ceils[startWorldY][startWorldX].getEntity()->draw();
            refresh();
            return false;
        }

        int prevScreenX = cursorScreenX; int prevScreenY = cursorScreenY;
        switch (ch) { case UP: cursorScreenY--; break; case DOWN: cursorScreenY++; break; case LEFT: cursorScreenX--; break; case RIGHT: cursorScreenX++; break; default: continue; }

        int nextWorldY = cursorScreenY - height/2 + mapHeight/2;
        int nextWorldX = cursorScreenX - width/2 + mapWidth/2;

        if (!(nextWorldY >= 1 && nextWorldY < mapHeight-1 && nextWorldX >= 1 && nextWorldX < mapWidth-1 && ceils[nextWorldY][nextWorldX].getEntity()->getType() == Entity::Type::FLOOR)) {
            cursorScreenX = prevScreenX;
            cursorScreenY = prevScreenY;
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
    ceils[finalWorldY][finalWorldX].getEntity()->draw();
    refresh();

    int charges = player->getEmpowermentCharges();
    int alliesToSummon = 1 + charges;
    int alliesSummoned = 0;

    int offsets[][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(std::begin(offsets), std::end(offsets), g);

    for (auto& offset : offsets) {
        if (alliesSummoned >= alliesToSummon) {
            break;
        }

        int checkX = playerX + offset[1];
        int checkY = playerY + offset[0];
        int worldY = checkY - height/2 + mapHeight/2;
        int worldX = checkX - width/2 + mapWidth/2;

        if (worldY >= 0 && worldY < mapHeight && worldX >= 0 && worldX < mapWidth &&
            ceils[worldY][worldX].getEntity()->getType() == Entity::Type::FLOOR) {

            auto newAlly = std::make_shared<DecoyAlly>(checkX, checkY, cursorScreenX, cursorScreenY);
            ceils[worldY][worldX].setEntity(newAlly);
            entities.push_back(newAlly);

            alliesSummoned++;
        }
    }

    if (alliesSummoned == 0) {
        mvprintw(height-1, 0, "No space to summon shadow!");
        refresh(); napms(1000);
        return false;
    }

    if (charges > 0) {
        player->clearEmpowermentCharges();
    }

    return true;
}