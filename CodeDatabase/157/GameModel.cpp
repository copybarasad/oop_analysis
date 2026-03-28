#include "GameModel.h"
#include "SaveLoadException.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>

GameModel::GameModel(int w, int h, std::size_t hand_limit) {
    field_ = std::make_unique<GameField>(w, h);
    player_ = std::make_unique<Player>(1, 1, *field_, hand_limit);
    field_->setPlayer(player_.get());
    startLevel();
}

void GameModel::startLevel() {
    int w = baseWidth_ + (level_ - 1);
    int h = baseHeight_ + (level_ - 1);
    field_ = std::make_unique<GameField>(w, h);
    size_t cap = player_ ? player_->hand().capacity() : 10;

    int oldHp = player_ ? player_->hp() : 100;
    int oldScore = player_ ? player_->score() : 0;
    int oldKills = player_ ? player_->kills() : 0;

    player_ = std::make_unique<Player>(1, 1, *field_, cap);
    field_->setPlayer(player_.get());
    player_->setHp(oldHp);
    player_->setScore(oldScore);
    player_->setKills(oldKills);

    if (level_ == 1) {
        player_->hand().addRandomSpell();
        player_->hand().addRandomSpell();
        player_->hand().addRandomSpell();
        player_->hand().addRandomSpell();
    }

    int baseHp = 5 + level_ * 2;
    field_->addUnit(std::make_unique<Enemy>(w/2, h/2, baseHp, 2 + level_));
    field_->addUnit(std::make_unique<Enemy>(w/2+1, h/2, std::max(1, baseHp-2), 1 + level_));
    field_->addBuilding(std::make_unique<EnemyTower>(w-2, h-2, 20 + level_*5, 6 + level_, 3));
}

bool GameModel::movePlayerTo(int x, int y) {
    player_->moveTo(x, y);
    // Проверяем, сдвинулся ли игрок (или потратил ход на атаку)
    return player_->hasMoved();
}

bool GameModel::attackAt(int x, int y) {
    Unit* u = field_->unitAt(x, y);
    if (u && u->isEnemy()) {
        player_->attack(u);
        player_->spendTurn();
        return true;
    }
    return false;
}

bool GameModel::useAt(std::size_t idx, int x, int y) {
    bool ok = player_->hand().useSpell(idx, *player_, *field_, x, y);
    return ok;
}

void GameModel::stepTurn() {
    field_->tick();
    player_->nextTurn();
}

void GameModel::nextLevel() {
    std::vector<std::unique_ptr<ISpell>> oldCards = player_->hand().extractAllCards();

    if (!oldCards.empty()) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(oldCards.begin(), oldCards.end(), g);
        size_t keep = (oldCards.size() + 1) / 2;
        oldCards.resize(keep);
    }

    level_++;
    startLevel();

    for (auto& card : oldCards) {
        player_->hand().addSpell(std::move(card));
    }
}

void GameModel::saveToFile(const std::string &fn) const {
    std::ofstream ofs(fn);
    if (!ofs) throw SaveLoadException("Cannot open file for writing: " + fn);

    int w = baseWidth_ + (level_-1);
    int h = baseHeight_ + (level_-1);

    ofs << level_ << "\n";
    ofs << w << " " << h << "\n";
    ofs << player_->x() << " " << player_->y() << "\n";

    auto cardNames = player_->hand().getSpellNames();
    ofs << cardNames.size() << " " << player_->hand().capacity() << "\n";
    for (const auto& name : cardNames) {
        ofs << name << " ";
    }
    ofs << "\n";

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

void GameModel::loadFromFile(const std::string &fn) {
    std::ifstream ifs(fn);
    if (!ifs) throw SaveLoadException("Cannot open file: " + fn);

    int lvl; ifs>>lvl;
    int w,h; ifs>>w>>h;
    int px,py; ifs>>px>>py;
    size_t handSize, handCap; ifs>>handSize>>handCap;

    field_ = std::make_unique<GameField>(w,h);
    player_ = std::make_unique<Player>(px,py,*field_, handCap);
    field_->setPlayer(player_.get());

    // читаем имена и создаем нужные карты
    for (size_t i=0; i<handSize; ++i) {
        std::string sName;
        ifs >> sName;
        if (sName == "DirectDamage") {
            player_->hand().addSpell(std::make_unique<DirectDamageSpell>(5, 3));
        } else if (sName == "AreaDamage") {
            player_->hand().addSpell(std::make_unique<AreaDamageSpell>(3, 3));
        } else if (sName == "Trap") {
            player_->hand().addSpell(std::make_unique<TrapSpell>(4, 3));
        } else {
            // На случай старых сохранений или ошибок
            player_->hand().addRandomSpell();
        }
    }

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

    level_ = lvl;
    std::cout << "Loaded " << fn << "\n";
}

const std::vector<UnitState> GameModel::units() const {
    std::vector<UnitState> res;
    for (int x = 0; x < width(); ++x) {
        for (int y = 0; y < height(); ++y) {
            if (const Unit* u = field_->unitAt(x, y)) {
                UnitType type = dynamic_cast<const Enemy*>(u) ? UnitType::Enemy : UnitType::Trap;
                res.push_back(UnitState{u->x(), u->y(), u->hp(), type, 0});
            }
            if (const Building* b = field_->buildingAt(x, y)) {
                const EnemyTower* et = dynamic_cast<const EnemyTower*>(b);
                int cd = et ? et->cooldown() : 0;
                res.push_back(UnitState{b->x(), b->y(), b->hp(), UnitType::Tower, cd});
            }
        }
    }
    return res;
}

const std::vector<std::string> GameModel::hand() const {
    return player_->hand().getSpellNames();
}


bool GameModel::addCard(const std::string &name) {
    if (name == "direct") player_->hand().addSpell(std::make_unique<DirectDamageSpell>(5, 3));
    else if (name == "area") player_->hand().addSpell(std::make_unique<AreaDamageSpell>(3, 3));
    else if (name == "trap") player_->hand().addSpell(std::make_unique<TrapSpell>(4, 3));
    else return false;
    return true;
}
