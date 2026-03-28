#include <iostream>  
#include <sstream>   
#include <string>    
#include <vector>    
#include <memory>    
#include <algorithm> 
#include <random>    
#include <stdexcept> 
#include <fstream>   
#include <ctime>     


#include "GameHelper.h"
#include "Game.h"  
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BuffSpell.h"

// Функция для поиска индекса врага по координатам
int Game::findEnemyIndexAt(int x, int y) const {
    for (int i = 0; i < enemies_.size(); ++i) {
        if (enemies_[i].getX() == x && enemies_[i].getY() == y && enemies_[i].isAlive()) {
            return i;
        }
    }
    return -1;  
}

// Функция для поиска индекса башни по координатам
int Game::findTowerIndexAt(int x, int y) const {
    for (int i = 0; i < towers_.size(); ++i) {
        if (towers_[i].getX() == x && towers_[i].getY() == y) {
            return i;
        }
    }
    return -1;  
}

// Функция для нанесения урона врагу
void Game::damageEnemy(int index, int dmg) {
    if (index >= 0 && index < enemies_.size()) {
        bool wasAlive = enemies_[index].isAlive();
        enemies_[index].takeDamage(dmg);  // Уменьшаем здоровье врага
        
        // Логирование урона врагу
        if (eventObserver_ != nullptr) {
            eventObserver_->onEnemyDamaged(index, dmg);
        }
        
        // Если враг был жив и теперь мёртв - начисляем очки
        if (wasAlive && !enemies_[index].isAlive()) {
            player_.addScore(10);  // Начисляем 10 очков за убийство врага
            
            // Логирование убийства врага
            if (eventObserver_ != nullptr) {
                eventObserver_->onEnemyKilled(index);
            }
            // Сообщение о событии должно выводиться через рендерер, а не здесь
        }
    }
}

// Функция для нанесения урона башне
void Game::damageTower(int index, int dmg) {
    if (index >= 0 && index < towers_.size()) {
        towers_[index].takeDamage(dmg);  // Уменьшаем здоровье башни
        
        // Логирование урона башне
        if (eventObserver_ != nullptr) {
            eventObserver_->onTowerDamaged(index, dmg);
        }
    }
}

// Функция для добавления союзника
bool Game::addAlly(int x, int y) {
    if (isCellEmptyForMove(x, y)) {
        allies_.emplace_back(x, y, 100, 10);  // Добавляем нового союзника с начальным здоровьем 100 и урон 10
        return true;
    }
    return false;  
}

// Функция для установки ловушки
bool Game::placeTrap(int x, int y, int damage) {
    if (isCellEmptyForMove(x, y)) {
        traps_.emplace_back(x, y, damage);  // Добавляем ловушку на клетку с указанным уроном
        return true;
    }
    return false;  
}

// Функция для проверки, все ли враги уничтожены
bool Game::allEnemiesDefeated() const {
    for (const auto& enemy : enemies_) {
        if (enemy.isAlive()) return false;  // Если хотя бы один враг жив, возвращаем false
    }
    return true;  
}

// Функция для сохранения состояния игры в поток
void Game::saveToStream(std::ostream& os) const {
    // Сохраняем основные параметры игры
    os << "VERSION 1\n";
    os << "TURN " << turn_ << "\n";
    os << "LEVEL " << level_ << "\n";
    os << "MAXLEVELS " << maxLevels_ << "\n";
    os << "FIELD " << field_.getWidth() << ' ' << field_.getHeight() << "\n";

    // Сохраняем сетку поля
    os << "FIELDGRID\n";
    for (int y = 0; y < field_.getHeight(); ++y) {
        for (int x = 0; x < field_.getWidth(); ++x) {
            int t = static_cast<int>(field_.getCellType(x, y));
            os << t;
            if (x + 1 < field_.getWidth()) os << ' ';
        }
        os << '\n';
    }
    os << "ENDGRID\n";

    // Сохраняем данные игрока
    SpellEnhancement pen = player_.getPendingEnhancement();
    os << "PLAYER " << player_.getX() << ' ' << player_.getY() << ' '
        << player_.getHP() << ' ' << player_.getMeleeDamage() << ' '
        << player_.getRangedDamage() << ' ' << player_.getRangedRange() << ' '
        << player_.getScore() << ' ' << static_cast<int>(player_.getAttackMode()) << ' '
        << pen.extraDamage << ' ' << pen.extraRange << ' ' << pen.extraArea << ' ' << pen.extraSummons << "\n";

    // Сохраняем данные врагов
    os << "ENEMIES " << enemies_.size() << "\n";
    for (const auto& e : enemies_) {
        os << e.getX() << ' ' << e.getY() << ' ' << e.getHP() << ' ' << e.getDamage() << ' ' << (e.isAlive() ? 1 : 0) << "\n";
    }

    // Сохраняем данные башен
    os << "TOWERS " << towers_.size() << "\n";
    for (const auto& t : towers_) {
        os << t.getX() << ' ' << t.getY() << ' ' << t.getHP() << ' ' << t.getDamage() << ' ' << t.getRange() << ' ' << t.getLastAttackTurn() << ' ' << t.getAttackCooldown() << "\n";
    }

    // Сохраняем данные союзников
    os << "ALLIES " << allies_.size() << "\n";
    for (const auto& a : allies_) {
        os << a.getX() << ' ' << a.getY() << ' ' << a.getHP() << ' ' << a.getDamage() << "\n";
    }

    // Сохраняем данные ловушек
    os << "TRAPS " << traps_.size() << "\n";
    for (const auto& tr : traps_) {
        os << tr.getX() << ' ' << tr.getY() << ' ' << tr.getDamage() << "\n";
    }

    // Сохраняем карты из руки игрока
    os << "HAND " << hand_.getSpells().size() << "\n";
    for (const auto& spPtr : hand_.getSpells()) {
        if (!spPtr) {
            os << "EMPTY\n";
            continue;
        }
        os << spPtr->name() << "\n";
    }

    os << "END\n";
}

// Функция для загрузки состояния игры из потока
void Game::loadFromStream(std::istream& is) {
    std::string token;
    if (!(is >> token)) throw LoadException("Невозможно прочитать заголовок файла");
    if (token != "VERSION") throw LoadException("Ожидался маркер VERSION");
    int ver = 0;
    is >> ver;
    if (ver != 1) throw LoadException("Неподдерживаемая версия сохранения");

    enemies_.clear();
    towers_.clear();
    allies_.clear();
    traps_.clear();
    hand_ = Hand(static_cast<int>(hand_.getSpells().size()));

    // Загрузка данных из файла
    while (is >> token) {
        if (token == "TURN") {
            is >> turn_;
        }
        else if (token == "LEVEL") {
            is >> level_;
        }
        else if (token == "MAXLEVELS") {
            is >> maxLevels_;
        }
        else if (token == "FIELD") {
            int w = 0, h = 0;
            is >> w >> h;
            if (w < 10 || w > 25 || h < 10 || h > 25) {
                throw LoadException("Размеры поля вне допустимого диапазона");
            }
            field_ = Field(w, h);
        }
        else if (token == "FIELDGRID") {
            std::string line;
            std::getline(is, line);
            for (int y = 0; y < field_.getHeight(); ++y) {
                std::string row;
                std::getline(is, row);
                if (row.empty()) { --y; continue; }
                std::istringstream rs(row);
                for (int x = 0; x < field_.getWidth(); ++x) {
                    int t;
                    if (!(rs >> t)) {
                        throw LoadException("Некорректные данные FIELDGRID");
                    }
                    field_.setCellType(x, y, static_cast<Cell::Type>(t));
                }
            }
            std::string maybeEnd;
            std::getline(is, maybeEnd);
        }
        else if (token == "PLAYER") {
            int x, y, hp, melee, ranged, range, score, mode;
            int pen_dmg, pen_rng, pen_area, pen_summons;
            is >> x >> y >> hp >> melee >> ranged >> range >> score >> mode >> pen_dmg >> pen_rng >> pen_area >> pen_summons;

            Player newP;
            newP.setPosition(x, y);

            int curHP = newP.getHP();
            if (hp < curHP) newP.takeDamage(curHP - hp);
            else if (hp > curHP) {
                // при необходимости реализовать addHP в Player
            }

            if (score > 0) newP.addScore(score);
            newP.setAttackMode(static_cast<Player::AttackMode>(mode));

            SpellEnhancement pen;
            pen.extraDamage = pen_dmg; pen.extraRange = pen_rng; pen.extraArea = pen_area; pen.extraSummons = pen_summons;
            newP.addPendingEnhancement(pen);

            player_ = std::move(newP);
        }
        else if (token == "ENEMIES") {
            int n = 0; is >> n;
            for (int i = 0; i < n; ++i) {
                int x, y, hp, dmg, alive;
                is >> x >> y >> hp >> dmg >> alive;
                Enemy e(x, y, hp, dmg);
                if (!alive) {
                    e.takeDamage(hp);
                }
                enemies_.push_back(std::move(e));
            }
        }
        else if (token == "TOWERS") {
            int n = 0; is >> n;
            for (int i = 0; i < n; ++i) {
                int x, y, hp, dmg, range, last;
                int cooldown = 1;
                is >> x >> y >> hp >> dmg >> range >> last;
                if (is.peek() == ' ') {
                    std::streampos pos = is.tellg();
                    int maybe;
                    if ((is >> maybe)) {
                        cooldown = maybe;
                    }
                    else {
                        is.clear();
                        is.seekg(pos);
                    }
                }
                Tower t(x, y, dmg, range, hp, cooldown);
                t.setLastAttackTurn(last);
                towers_.push_back(std::move(t));
            }
        }
        else if (token == "ALLIES") {
            int n = 0; is >> n;
            for (int i = 0; i < n; ++i) {
                int x, y, hp, dmg;
                is >> x >> y >> hp >> dmg;
                Ally a(x, y, hp, dmg);
                allies_.push_back(std::move(a));
            }
        }
        else if (token == "TRAPS") {
            int n = 0; is >> n;
            for (int i = 0; i < n; ++i) {
                int x, y, dmg;
                is >> x >> y >> dmg;
                Trap tr(x, y, dmg);
                traps_.push_back(std::move(tr));
            }
        }
        else if (token == "HAND") {
            int n = 0; is >> n;
            std::string line;
            std::getline(is, line);
            for (int i = 0; i < n; ++i) {
                std::string sline;
                std::getline(is, sline);
                if (sline.empty()) { --i; continue; }
                if (sline == "EMPTY") continue;
                std::istringstream ss(sline);
                std::string type; ss >> type;
                std::unique_ptr<Spell> sp;
                if (type == "DirectDamage") sp = std::make_unique<DirectDamageSpell>();
                else if (type == "AreaDamage") sp = std::make_unique<AreaDamageSpell>();
                else if (type == "Trap") sp = std::make_unique<TrapSpell>();
                else if (type == "Summon") sp = std::make_unique<SummonSpell>();
                else if (type == "Buff") {
                    SpellEnhancement enh; enh.extraDamage = 10;
                    sp = std::make_unique<BuffSpell>(enh);
                }
                else {
                    continue;
                }
                hand_.addSpell(std::move(sp));
            }
        }
        else if (token == "END") {
            break;
        }
        else {
            throw LoadException(std::string("Неизвестный маркер в файле: ") + token);
        }
    }

    auto checkInside = [this](int x, int y) {
        return this->field_.isInside(x, y);
        };
    if (!checkInside(player_.getX(), player_.getY())) throw LoadException("Координаты игрока вне поля");
    for (const auto& e : enemies_) {
        if (!checkInside(e.getX(), e.getY())) throw LoadException("Координаты врага вне поля");
    }
    for (const auto& t : towers_) {
        if (!checkInside(t.getX(), t.getY())) throw LoadException("Координаты башни вне поля");
    }
    for (const auto& a : allies_) {
        if (!checkInside(a.getX(), a.getY())) throw LoadException("Координаты союзника вне поля");
    }
    for (const auto& tr : traps_) {
        if (!checkInside(tr.getX(), tr.getY())) throw LoadException("Координаты ловушки вне поля");
    }
}
