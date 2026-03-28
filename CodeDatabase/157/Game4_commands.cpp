#include "Game4.h"
#include "SaveLoadException.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <random>
#include <algorithm>

void Game4::processCommand(const std::string &line) {
    std::istringstream ss(line);
    std::string cmd; ss >> cmd;
    if (cmd == "move") { int x,y; ss>>x>>y; cmdMove(x,y); }
    else if (cmd == "use") { size_t id; int tx,ty; ss>>id>>tx>>ty; cmdUse(id,tx,ty); }
    else if (cmd == "save") { std::string fn; ss>>fn; if (fn.empty()) throw SaveLoadException("Empty filename"); cmdSave(fn); }
    else if (cmd == "load") { std::string fn; ss>>fn; if (fn.empty()) throw SaveLoadException("Empty filename"); cmdLoad(fn); }
    else if (cmd == "status") cmdStatus();
    else if (cmd == "next") nextLevel();
    else if (cmd.empty()) {}
    else std::cout << "Unknown command\n";
}

void Game4::cmdMove(int x,int y) {
    player_->nextTurn();
    player_->moveTo(x,y);
}

void Game4::cmdUse(size_t idx,int tx,int ty) {
    player_->nextTurn();
    if (!player_->hand().useSpell(idx, *player_, *field_, tx, ty)) std::cout << "Spell failed\n";
}

void Game4::cmdSave(const std::string &fn) {
    std::ofstream ofs(fn);
    if (!ofs) throw SaveLoadException("Cannot open file for writing: " + fn);
    int w = baseWidth_ + (levelNumber_-1);
    int h = baseHeight_ + (levelNumber_-1);
    ofs << levelNumber_ << "\n";
    ofs << w << " " << h << "\n";
    ofs << player_->x() << " " << player_->y() << "\n";
    ofs << player_->hand().size() << " " << player_->hand().capacity() << "\n";
    ofs << player_->hp() << "\n";
    std::vector<Unit*> units;
    for (int x=0;x<w;++x) for (int y=0;y<h;++y) {
            Unit* u = field_->unitAt(x,y);
            if (u && !u->isDead()) units.push_back(u);
        }
    ofs << units.size() << "\n";
    for (auto u : units) {
        ofs << u->x() << " " << u->y() << " " << u->hp() << " ";
        if (u->isEnemy()) {
            Enemy* e = dynamic_cast<Enemy*>(u);
            int dmg = e ? e->damage() : 0;
            ofs << 1 << " " << dmg << "\n";
        } else {
            ofs << 0 << " " << 0 << "\n";
        }
    }
    std::vector<Building*> bs;
    for (int x=0;x<w;++x) for (int y=0;y<h;++y) {
            Building* b = field_->buildingAt(x,y);
            if (b && !b->isDestroyed()) bs.push_back(b);
        }
    ofs << bs.size() << "\n";
    for (auto b : bs) {
        EnemyTower* et = dynamic_cast<EnemyTower*>(b);
        int type = et ? 1 : 0;
        ofs << b->x() << " " << b->y() << " " << type << "\n";
    }
    ofs.close();
    if (!ofs) throw SaveLoadException("Write error");
    std::cout << "Saved to " << fn << "\n";
}

void Game4::cmdLoad(const std::string &fn) {
    std::ifstream ifs(fn);
    if (!ifs) throw SaveLoadException("Cannot open file: " + fn);
    int lvl; ifs>>lvl;
    int w,h; ifs>>w>>h;
    int px,py; ifs>>px>>py;
    size_t handSize, handCap; ifs>>handSize>>handCap;
    field_.reset(new GameField(w,h));
    player_.reset(new Player(px,py,*field_, handCap));
    field_->setPlayer(player_.get());
    for (size_t i=0;i<handSize;++i) player_->hand().addRandomSpell();
    int savedHp = 100;
    ifs >> savedHp;
    player_->setHp(savedHp);

    size_t uc; ifs>>uc;
    for (size_t i=0;i<uc;++i) {
        int ux,uy,uhp,enemy,dmg;
        ifs >> ux >> uy >> uhp >> enemy >> dmg;
        if (!ifs) throw SaveLoadException("Invalid unit entry in file");
        if (enemy) {
            field_->addUnit(std::make_unique<Enemy>(ux,uy,uhp, dmg));
        } else {
            field_->addUnit(std::make_unique<Unit>(ux,uy,uhp, false));
        }
    }
    size_t bc; ifs>>bc;
    for (size_t i=0;i<bc;++i) {
        int bx,by,type; ifs>>bx>>by>>type;
        if (type==1) field_->addBuilding(std::make_unique<EnemyTower>(bx,by,20 + lvl*5, 6 + lvl, 3));
        else field_->addBuilding(std::make_unique<Building>(bx,by,20,true));
    }
    levelNumber_ = lvl;
    std::cout << "Loaded " << fn << "\n";
}

void Game4::cmdStatus() {
    field_->debugPrint();
    player_->hand().list();
    std::cout << "Player at (" << player_->x() << "," << player_->y() << ") HP=" << player_->hp()
              << " dmg=" << player_->damage() << " score=" << player_->score() << "\n";
}

void Game4::nextLevel() {
    std::vector<std::unique_ptr<ISpell>> oldCards = player_->hand().extractAllCards();
    int oldScore = player_->score();
    int oldKills = player_->kills();
    size_t oldCap = player_->hand().capacity();

    if (!oldCards.empty()) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(oldCards.begin(), oldCards.end(), g);
        size_t keep = (oldCards.size() + 1) / 2;
        oldCards.resize(keep);
    }
    levelNumber_++;
    int newW = std::min(baseWidth_ + (levelNumber_ - 1), 25);
    int newH = std::min(baseHeight_ + (levelNumber_ - 1), 25);
    field_.reset(new GameField(newW, newH));
    player_.reset(new Player(1, 1, *field_, oldCap));
    field_->setPlayer(player_.get());
    player_->setHp(100);
    player_->setScore(oldScore);
    player_->setKills(oldKills);

    for (auto& card : oldCards) {
        player_->hand().addSpell(std::move(card));
    }
    int baseHp = 5 + levelNumber_ * 2;
    field_->addUnit(std::make_unique<Enemy>(newW / 2, newH / 2, baseHp, 2 + levelNumber_));
    field_->addUnit(std::make_unique<Enemy>(newW / 2 + 1, newH / 2, std::max(1, baseHp - 2), 1 + levelNumber_));
    field_->addBuilding(std::make_unique<EnemyTower>(newW - 2, newH - 2, 20 + levelNumber_ * 5, 6 + levelNumber_, 3));

    std::cout << "Advanced to level " << levelNumber_ << ". Field sized " << newW << "x" << newH
              << ". Player HP restored, hand reduced to " << player_->hand().size() << " card(s).\n";
}


int Game4::countEnemyUnits() const {
    int w = baseWidth_ + (levelNumber_-1);
    int h = baseHeight_ + (levelNumber_-1);
    int cnt=0;
    for (int x=0;x < w;++x) for (int y=0;y < h;++y) {
            Unit* u = field_->unitAt(x,y);
            if (u && !u->isDead() && u->isEnemy()) ++cnt;
        }
    return cnt;
}

bool Game4::anyEnemyLeft() const { return countEnemyUnits() > 0; }