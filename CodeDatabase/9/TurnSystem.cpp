#include "TurnSystem.h"
#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBase.h"
#include "EnemyTower.h"
#include "Ally.h"
#include "Cell.h"

#include <utility>
#include <vector>

void TurnSystem::ProcessTurn(Game& game) {
    Board& board  = game.board();
    Player& player = game.player();
    std::vector<Enemy>& enemies = game.enemies();
    std::vector<Ally>&  allies = game.allies();
    EnemyBase* base = game.base();
    std::vector<EnemyTower>& towers = game.towers();

    auto isInside = [&board](std::pair<int,int> p) {
        return p.first >= 0 && p.second >= 0 &&
               p.first < board.width() && p.second < board.height();
    };

    auto findEnemyAt = [&enemies](std::pair<int,int> p) -> int {
        for (int i = 0; i < (int)enemies.size(); ++i) {
            if (enemies[i].isAlive() && enemies[i].position() == p) {
                return i;
            }
        }
        return -1;
    };

    auto findAllyAt = [&allies](std::pair<int,int> p) -> int {
        for (int i = 0; i < (int)allies.size(); ++i) {
            if (allies[i].isAlive() && allies[i].position() == p) {
                return i;
            }
        }
        return -1;
    };

    auto findTowerAt = [&towers](std::pair<int,int> p) -> int {
        for (int i = 0; i < (int)towers.size(); ++i) {
            if (towers[i].isAlive() && towers[i].position() == p) {
                return i;
            }
        }
        return -1;
    };

    for (int i = 0; i < (int)allies.size(); ++i) {
        Ally& a = allies[i];
        if (!a.isAlive()) continue;
        if (a.shouldSkipTurn()) {
            a.setSkipNextTurn(false);
            continue;
        }

        int bestIdx  = -1;
        int bestDist = 1000000;
        std::pair<int,int> ap = a.position();

        for (int j = 0; j < (int)enemies.size(); ++j) {
            if (!enemies[j].isAlive()) continue;
            std::pair<int,int> ep = enemies[j].position();
            int dx = ap.first  - ep.first;  if (dx < 0) dx = -dx;
            int dy = ap.second - ep.second; if (dy < 0) dy = -dy;
            int dist = dx > dy ? dx : dy;
            if (dist < bestDist) {
                bestDist = dist;
                bestIdx  = j;
            }
        }

        if (bestIdx == -1) {
            continue;
        }

        if (bestDist <= 1) {
            enemies[bestIdx].takeDamage(a.damage());
            if (!enemies[bestIdx].isAlive()) {
                player.addScore(10);
            }
            continue;
        }

        std::pair<int,int> ep   = enemies[bestIdx].position();
        std::pair<int,int> dest = ap;
        if (ap.first  < ep.first)  dest.first++;
        else if (ap.first > ep.first) dest.first--;
        if (ap.second < ep.second) dest.second++;
        else if (ap.second > ep.second) dest.second--;

        if (!isInside(dest)) continue;
        if (!board.at(dest).isPassable()) continue;

        int otherEnemy = findEnemyAt(dest);
        if (otherEnemy >= 0) continue;

        int towerIdx = findTowerAt(dest);
        if (towerIdx >= 0) continue;

        if (base && base->position() == dest) continue;
        if (player.position() == dest)       continue;

        bool allyBusy = false;
        for (int k = 0; k < (int)allies.size(); ++k) {
            if (k != i && allies[k].isAlive() && allies[k].position() == dest) {
                allyBusy = true;
                break;
            }
        }
        if (allyBusy) continue;

        a.setPosition(dest);
        if (board.at(dest).isSlow()) {
            a.setSkipNextTurn(true);
        }
    }

    for (int i = 0; i < (int)enemies.size(); ++i) {
        Enemy& e = enemies[i];
        if (!e.isAlive()) continue;
        if (e.shouldSkipTurn()) {
            e.setSkipNextTurn(false);
            continue;
        }
        auto pe = e.position();
        auto pp = player.position();
        std::pair<int,int> tp = pp;

        int tdx = pe.first  - pp.first;  if (tdx < 0) tdx = -tdx;
        int tdy = pe.second - pp.second; if (tdy < 0) tdy = -tdy;
        int bestDist = (tdx > tdy) ? tdx : tdy;
        bool targetIsAlly = false;

        for (int j = 0; j < (int)allies.size(); ++j) {
            if (!allies[j].isAlive()) continue;
            std::pair<int,int> ap = allies[j].position();
            int adx = pe.first  - ap.first;  if (adx < 0) adx = -adx;
            int ady = pe.second - ap.second; if (ady < 0) ady = -ady;
            int ad  = (adx > ady) ? adx : ady;
            if (ad < bestDist || (ad == bestDist && !targetIsAlly)) {
                bestDist    = ad;
                tp          = ap;
                targetIsAlly = true;
            }
        }

        std::pair<int,int> dest = pe;
        if (pe.first  < tp.first)  dest.first++;
        else if (pe.first > tp.first) dest.first--;
        if (pe.second < tp.second) dest.second++;
        else if (pe.second > tp.second) dest.second--;

        if (dest == pp) {
            player.takeDamage(e.damage());
            continue;
        }
        int allyIdxHit = findAllyAt(dest);
        if (allyIdxHit >= 0) {
            allies[allyIdxHit].takeDamage(e.damage());
            continue;
        }

        if (!isInside(dest)) continue;

        int other = findEnemyAt(dest);
        if (other >= 0) continue;

        int allyIdxMove = findAllyAt(dest);
        if (allyIdxMove >= 0) continue;

        int towerIdx = findTowerAt(dest);
        if (towerIdx >= 0) continue;

        if (!board.at(dest).isPassable()) continue;

        e.setPosition(dest);
        Cell& cell = board.at(dest);
        if (cell.hasTrap()) {
            e.takeDamage(cell.trapDamage());
            cell.clearTrap();
            if (!e.isAlive()) {
                player.addScore(10);
            }
        }

        if (board.at(dest).isSlow()) {
            e.setSkipNextTurn(true);
        }
    }

    if (base != nullptr) {
        base->onTurn(enemies, board, player);
    }

    for (int i = 0; i < (int)towers.size(); ++i) {
        towers[i].onTurn(player);
    }
}