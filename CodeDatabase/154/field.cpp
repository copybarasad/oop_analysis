#include "field.h"
#include "enemy.h"
#include "building.h"
#include "player.h"
#include "trap.h"
#include "combat_system.h"
#include "ally.h"
#include <iostream>
#include <random>
#include <algorithm>
#include "tower.h"
#include "event_system.h"
#include "direct_damage_spell.h"
#include "area_spell.h"
#include "trap_spell.h"
#include "summon_spell.h"
#include "buff_spell.h"

Field::Field(int rows, int cols)
    : grid_(rows, std::vector<Cell>(cols)), nextId_(1000), playerId_(-1), turnCounter_(0) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 99);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int centerR = rows / 2;
            int centerC = cols / 2;
            if (r == centerR && c == centerC) {
                grid_[r][c].setEmpty();
            } else {
                int chance = dist(gen);
                if (chance < 5) grid_[r][c].setOccupant(CellContent::Impassable, -1);
                else if (chance < 10) grid_[r][c].setOccupant(CellContent::Slow, -1);
                else grid_[r][c].setEmpty();
            }
        }
    }
}

Field::Field(const Field& other)
    : grid_(other.grid_), nextId_(other.nextId_), playerId_(other.playerId_), 
      turnCounter_(other.turnCounter_) {
    
    entities_ = other.entities_;
    positions_ = other.positions_;
}

int Field::rows() const { return static_cast<int>(grid_.size()); }
int Field::cols() const { return grid_.empty() ? 0 : static_cast<int>(grid_[0].size()); }

bool Field::inBounds(int r, int c) const {
    return r >= 0 && r < rows() && c >= 0 && c < cols();
}

int Field::generateEntityId() {
    return nextId_++;
}

int Field::findEmptyAdjacent(int r, int c) const {
    static const int dr[4] = {-1,1,0,0};
    static const int dc[4] = {0,0,-1,1};
    for (int k=0;k<4;k++) {
        int nr=r+dr[k], nc=c+dc[k];
        if (!inBounds(nr,nc)) continue;
        if (grid_[nr][nc].getContent() == CellContent::Empty) return nr*cols()+nc;
    }
    return -1;
}

bool Field::placePlayer(Player* player, int row, int col) {
    if (!inBounds(row, col)) return false;
    if (grid_[row][col].getContent() != CellContent::Empty) return false;
    playerId_ = player->getId();
    grid_[row][col].setOccupant(CellContent::Player, playerId_);
    positions_[playerId_] = {row, col};
    entities_[playerId_] = player;
    return true;
}

bool Field::placeEnemy(Entity* enemy, int row, int col) {
    if (!inBounds(row, col)) return false;
    if (grid_[row][col].getContent() != CellContent::Empty) return false;
    int id = enemy->getId();
    grid_[row][col].setOccupant(CellContent::Enemy, id);
    entities_[id] = enemy;
    positions_[id] = {row, col};
    // Inform event system about spawned enemy
    EventSystem::getInstance().logEnemySpawned(id, row, col);
    return true;
}

bool Field::placeBuilding(Entity* building, int row, int col) {
    if (!inBounds(row, col)) return false;
    if (grid_[row][col].getContent() != CellContent::Empty) return false;
    int id = building->getId();
    grid_[row][col].setOccupant(CellContent::Building, id);
    entities_[id] = building;
    positions_[id] = {row, col};
    // Inform event system about new building
    EventSystem::getInstance().logBuildingBuilt(id, row, col);
    return true;
}

bool Field::movePlayer(Direction dir) {
    if (playerId_ < 0) return false;
    auto player = entities_[playerId_]->asPlayer();
    if (!player) return false;

    if (!player->canMove()) {
        player->resetSlow();
        return false;
    }

    auto pos = positions_[playerId_];
    int newRow = pos.first;
    int newCol = pos.second;
    int stepR = 0;
    int stepC = 0;
    switch (dir) {
        case Direction::Up: newRow--; stepR = -1; break;
        case Direction::Down: newRow++; stepR = 1; break;
        case Direction::Left: newCol--; stepC = -1; break;
        case Direction::Right: newCol++; stepC = 1; break;
        default: return false;
    }

    if (!inBounds(newRow, newCol)) return false;

    auto& cell = grid_[newRow][newCol];
    CellContent prevContent = cell.getContent();
    if (prevContent == CellContent::Impassable) return false;

    if (prevContent == CellContent::Enemy) {
        int enemyId = cell.getOccupantId();
        auto jt = entities_.find(enemyId);
        if (jt != entities_.end()) {
            CombatSystem::dealDamage(*this, jt->first, player->getDamage(), player->getId());
            player->applySlow();
        }
        return true;
    }

    int attackRange = player->getAttackRange();
    if (attackRange > 1) {
        int pr = pos.first;
        int pc = pos.second;
        for (int s = 1; s <= attackRange; ++s) {
            int r = pr + s*stepR;
            int c = pc + s*stepC;
            if (!inBounds(r,c)) break;
            CellContent cc = grid_[r][c].getContent();
            if (cc == CellContent::Impassable || cc == CellContent::Building) break;
            if (cc == CellContent::Enemy) {
                int enemyId = grid_[r][c].getOccupantId();
                auto jt = entities_.find(enemyId);
                if (jt != entities_.end()) {
                    jt->second->setHealth(jt->second->getHealth() - player->getDamage());
                    if (!jt->second->isAlive()) {
                        CombatSystem::dealDamage(*this, jt->first, player->getDamage(), playerId_);
                    }
                    player->applySlow();
                    return true;
                }
            }
        }
    }

    grid_[pos.first][pos.second].setOccupant(CellContent::Empty, -1);
    grid_[newRow][newCol].setOccupant(CellContent::Player, playerId_);
    positions_[playerId_] = {newRow, newCol};

    if (prevContent == CellContent::Slow) {
        player->applySlow();
    }
    // Notify event system that player moved
    EventSystem::getInstance().logPlayerMoved(newRow, newCol);
    return true;
}

void Field::updateEntities() {
    std::vector<int> keys;
    for (const auto &p : entities_) keys.push_back(p.first);

    for (int id : keys) {
        auto it = entities_.find(id);
        if (it == entities_.end()) continue;
        Entity* e = it->second;
        if (!e->isAlive()) continue;

        if (e->getType() == EntityType::Ally) {
            Ally* ally = e->asAlly();
            if (ally) {
                int ar = positions_[id].first;
                int ac = positions_[id].second;
                
                auto step = ally->chooseStep(*this, ar, ac);
                int nr = ar + step.first;
                int nc = ac + step.second;

                if (inBounds(nr, nc)) {
                    CellContent cont = grid_[nr][nc].getContent();
                    if (cont == CellContent::Enemy) {
                        int enemyId = grid_[nr][nc].getOccupantId();
                        auto enemyIt = entities_.find(enemyId);
                        if (enemyIt != entities_.end()) {
                            enemyIt->second->setHealth(enemyIt->second->getHealth() - ally->getDamage());
                            if (!enemyIt->second->isAlive()) {
                                EventSystem::getInstance().logEnemyDefeated(enemyIt->first);
                                grid_[nr][nc].setOccupant(CellContent::Empty, -1);
                                entities_.erase(enemyIt);
                            }
                        }
                    }
                    else if (cont == CellContent::Empty) {
                        grid_[ar][ac].setOccupant(CellContent::Empty, -1);
                        grid_[nr][nc].setOccupant(CellContent::Ally, id);
                        positions_[id] = {nr, nc};
                    }
                }
                continue;
            }
        }

        if (e->getType() == EntityType::Building) {
            Building* b = e->asBuilding();
            if (b) {
                b->tick();
                b->tryAttackIfTower(*this);
                
                Tower* tower = e->asTower();
                if (!tower && b->getCounter() <= 0) {
                    int r = positions_[id].first;
                    int c = positions_[id].second;
                    int nid = generateEntityId();
                    static const int drs[4] = {-1,1,0,0};
                    static const int dcs[4] = {0,0,-1,1};
                    for (int k = 0; k < 4; ++k) {
                        int nr = r + drs[k];
                        int nc = c + dcs[k];
                        if (!inBounds(nr, nc)) continue;
                        if (grid_[nr][nc].getContent() == CellContent::Empty) {
                            Enemy* enemy = new Enemy(nid, 5, 1);
                            placeEnemy(enemy, nr, nc);
                            break;
                        }
                    }
                    b->resetCounter();
                }
            }
            continue;
        }

        if (e->getType() == EntityType::Enemy) {
            Enemy* enemy = e->asEnemy();
            if (!enemy) continue;
            if (playerId_ < 0) continue;
            auto pit = entities_.find(playerId_);
            if (pit == entities_.end()) continue;
            Player* player = pit->second->asPlayer();
            if (!player) continue;
            int pr = positions_[playerId_].first;
            int pc = positions_[playerId_].second;
            int er = positions_[id].first;
            int ec = positions_[id].second;

            auto step = enemy->chooseStep(pr, pc, er, ec);
            int nr = er + step.first;
            int nc = ec + step.second;

            if (!inBounds(nr, nc)) continue;
            CellContent cont = grid_[nr][nc].getContent();
            if (cont == CellContent::Impassable || cont == CellContent::Building || cont == CellContent::Enemy) continue;
            if (cont == CellContent::Player) {
                int dmg = enemy->getDamage();
                player->setHealth(player->getHealth() - dmg);
                EventSystem::getInstance().logPlayerTakeDamage(dmg, id);
                continue;
            }
            if (cont == CellContent::Trap) {
                int tid = grid_[nr][nc].getOccupantId();
                auto tIt = entities_.find(tid);
                if (tIt != entities_.end() && tIt->second->getType() == EntityType::Trap) {

                    Trap* trap = nullptr;

                    trap = tIt->second->asTrap();
                    if (trap) {
                        int dmg = trap->getDamage();
                        enemy->setHealth(enemy->getHealth() - dmg);

                        // log trap triggered
                        EventSystem::getInstance().logTrapTriggered(tid, id, dmg, nr, nc);

                        grid_[nr][nc].setOccupant(CellContent::Empty, -1);
                        entities_.erase(tIt);
                        positions_.erase(tid);
                        if (!enemy->isAlive()) {
                            EventSystem::getInstance().logEnemyDefeated(id);
                            grid_[er][ec].setOccupant(CellContent::Empty, -1);
                            positions_.erase(id);
                            entities_.erase(id);
                            continue;
                        }
                    }
                }
                continue;
            }

            grid_[er][ec].setOccupant(CellContent::Empty, -1);
            grid_[nr][nc].setOccupant(CellContent::Enemy, id);
            positions_[id] = {nr, nc};
        }
    }

    std::vector<int> toRemove;
    for (const auto &p : entities_) {
        if (!p.second->isAlive()) toRemove.push_back(p.first);
    }
    for (int id : toRemove) {
        auto posIt = positions_.find(id);
        if (posIt != positions_.end()) {
            int r = posIt->second.first;
            int c = posIt->second.second;
            if (inBounds(r,c) && grid_[r][c].getOccupantId() == id) grid_[r][c].setEmpty();
            positions_.erase(posIt);
        }
        if (id == playerId_) playerId_ = -1;
        entities_.erase(id);
    }

    ++turnCounter_;
}

bool Field::isGameOver() const {
    if (playerId_ < 0) return true;
    auto it = entities_.find(playerId_);
    if (it == entities_.end()) return true;
    return !it->second->isAlive();
}

void Field::print() const {
    for (int r = 0; r < rows(); ++r) {
        for (int c = 0; c < cols(); ++c) {
            char symbol = '.';
            auto content = grid_[r][c].getContent();
            if (content == CellContent::Player) symbol = 'P';
            else if (content == CellContent::Enemy) symbol = 'E';
            else if (content == CellContent::Building) {
                int oid = grid_[r][c].getOccupantId();
                auto it = entities_.find(oid);
                if (it != entities_.end()) {
                    Building* bptr = it->second->asBuilding();
                    if (bptr) symbol = bptr->getDisplaySymbol();
                    else symbol = 'B';
                } else symbol = 'B';
            }
            else if (content == CellContent::Trap) symbol = 'X';
            else if (content == CellContent::Impassable) symbol = '#';
            else if (content == CellContent::Ally) symbol = 'A';
            
            if (content == CellContent::Building) {
                int oid = grid_[r][c].getOccupantId();
                auto it = entities_.find(oid);
                if (it != entities_.end() && it->second) {
                    Tower* tower = it->second->asTower();
                    if (tower) symbol = 'T';
                }
            }
            else if (content == CellContent::Slow) symbol = '~';
            std::cout << symbol << ' ';
        }
        std::cout << "\n";
    }
    if (playerId_ >= 0) {
        auto player = entities_.at(playerId_)->asPlayer();
        std::cout << "Health: " << player->getHealth() << " Score: " << player->getScore()
                  << " Mode: " << (player->isRanged() ? "Ranged" : "Melee") << "\n";
        Hand &h = player->hand();
        std::cout << "Hand (" << h.size() << "/" << h.capacity() << "): ";
        for (size_t i = 0; i < h.size(); ++i) {
            std::cout << i << ":" << h.getSpellName(i) << " ";
        }
        std::cout << "\n";
    }
}

#include <cmath>

bool Field::getPosition(int id, int& r, int& c) const {
    auto it = positions_.find(id);
    if (it == positions_.end()) return false;
    r = it->second.first;
    c = it->second.second;
    return true;
}

int Field::manhattanDistance(int r1, int c1, int r2, int c2) const {
    return std::abs(r1 - r2) + std::abs(c1 - c2);
}

int Field::getOccupantIdAt(int r, int c) const {
    if (!inBounds(r,c)) return -1;
    return grid_[r][c].getOccupantId();
}

bool Field::isEnemy(int id) const {
    auto it = entities_.find(id);
    if (it == entities_.end()) return false;
    return it->second->getType() == EntityType::Enemy;
}

bool Field::isBuilding(int id) const {
    auto it = entities_.find(id);
    if (it == entities_.end()) return false;
    return it->second->getType() == EntityType::Building;
}

Entity* Field::getEntity(int id) {
    auto it = entities_.find(id);
    if (it != entities_.end()) {
        return it->second;
    }
    return nullptr;
}

const Entity* Field::getEntity(int id) const {
    auto it = entities_.find(id);
    if (it != entities_.end()) {
        return it->second;
    }
    return nullptr;
}

void Field::removeEntity(int id) {
    auto it = entities_.find(id);
    if (it == entities_.end()) return;

    int r, c;
    if (getPosition(id, r, c)) {
        grid_[r][c].setOccupant(CellContent::Empty, -1);
    }
    
    if (id == playerId_) {
        playerId_ = -1;
    }
    
    entities_.erase(it);
    positions_.erase(id);
}

void Field::placeEntity(Entity* e, int r, int c) {
    if (!inBounds(r,c) || !e) return;
    int id = e->getId();
    EntityType t = e->getType();
    entities_[id] = e;
    CellContent cc = CellContent::Enemy;
    if (t == EntityType::Player) cc = CellContent::Player;
    else if (t == EntityType::Building) cc = CellContent::Building;
    else if (t == EntityType::Trap) cc = CellContent::Trap;
    else if (t == EntityType::Ally) cc = CellContent::Ally;
    grid_[r][c].setOccupant(cc, id);
    positions_[id] = {r,c};
}

bool Field::placeAlly(Entity* ally, int row, int col) {
    if (!inBounds(row, col)) return false;
    if (grid_[row][col].getContent() != CellContent::Empty) return false;
    int id = ally->getId();
    grid_[row][col].setOccupant(CellContent::Ally, id);
    entities_[id] = ally;
    positions_[id] = {row, col};
    return true;
}

bool Field::cellIsEmpty(int r, int c) const {
    if (!inBounds(r,c)) return false;
    return grid_[r][c].getContent() == CellContent::Empty;
}

int Field::findPlayerInRadius(int r, int c, int range) const {
    if (playerId_ < 0) return -1;
    auto it = positions_.find(playerId_);
    if (it == positions_.end()) return -1;
    int pr = it->second.first;
    int pc = it->second.second;
    if (manhattanDistance(r,c,pr,pc) <= range) return playerId_;
    return -1;
}

Player* Field::getPlayerById(int id) const {
    auto it = entities_.find(id);
    if (it == entities_.end()) return nullptr;
    return it->second->asPlayer();
}

int Field::getPlayerId() const {
    return playerId_;
}

void Field::togglePlayerCombatMode() {
    if (playerId_ < 0) return;
    auto it = entities_.find(playerId_);
    if (it == entities_.end()) return;
    Player* player = it->second->asPlayer();
    if (player) {
        player->toggleCombatMode();
    }
}

bool Field::playerCastSpell(size_t index, int targetRow, int targetCol) {
    if (playerId_ < 0) return false;
    auto it = entities_.find(playerId_);
    if (it == entities_.end()) return false;
    Player* player = it->second->asPlayer();
    if (!player) return false;
    return player->hand().playSpell(index, playerId_, *this, targetRow, targetCol);
}

const std::unordered_map<int, Entity*>& Field::getAllEntities() const {
    return entities_;
}

const std::unordered_map<int, std::pair<int,int>>& Field::getAllPositions() const {
    return positions_;
}
