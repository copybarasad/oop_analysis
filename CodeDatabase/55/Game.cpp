#include "Game.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;


Game::Game(GameConfig &cfg): config(cfg), sea_field(cfg), pl(cfg), defeated_enemies(0), isGameOn(true) {
    pl.setPosition(cfg.pl_start_pos);
    sea_field.SetOnSeaZone(cfg.pl_start_pos, pl);
    spawn();
}



void Game::spawn(){
    for (int i = 0; i < config.en_count; ++i) {
        Enemy en(config);
        Position enemyPos = getFreePositions();
        en.setPosition(enemyPos);
        sea_field.SetOnSeaZone(enemyPos, en);
        enemies.push_back(en);
    }

    for (int i = 0; i < config.vortex_count; ++i) {
        Vortex vortex(config);
        Position vortexPos = getFreePositions();
        vortex.setPosition(vortexPos);
        sea_field.SetOnSeaZone(vortexPos, vortex);
        vortexes.push_back(vortex);
    }

    for (int i = 0; i < config.oct_count; ++i) {
        Octopus oct(config);
        Position octPos = getFreePositions();
        oct.setPosition(octPos);
        sea_field.SetOnSeaZone(octPos, oct);
        octopuses.push_back(oct);
    }
}

Sea &Game::getSea(){
    return sea_field;
}

Position Game::getFreePositions() {
    auto emptyPositions = sea_field.emptyZones();
    if(emptyPositions.empty()) {
        return Position(0, 0);
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, emptyPositions.size() - 1);
    
    return emptyPositions[dis(gen)];
}

Enemy* Game::findEnemyAtPosition(Position &pos) {
    for(auto& enemy : enemies) {
        if(enemy.getPosition() == pos && enemy.isAlive()) {
            return &enemy;
        }
    }
    return nullptr;
}

Octopus* Game::findOctAtPosition(Position &pos) {
    for(auto& oct : octopuses) {
        if(oct.getPosition() == pos && oct.isAlive()) {
            return &oct;
        }
    }
    return nullptr;
}

void Game::removeDeads(){
    vector<Enemy> al_ens;
    vector<Octopus> al_octs;

    for (auto& enemy : enemies) {
        if(enemy.isAlive()){
            al_ens.push_back(enemy);
        }else{
            sea_field.removeEntity(enemy.getPosition());
        }
    }
    for (auto& oct : octopuses) {
        if(oct.isAlive()){
            al_octs.push_back(oct);
        }else{
            sea_field.removeEntity(oct.getPosition());
        }
    }
    enemies = al_ens;
    octopuses = al_octs;
    defeated_enemies = config.en_count + config.oct_count - enemies.size() - octopuses.size();
}

vector<Enemy>& Game::getEnemies(){
    return enemies;
}

Player &Game::getPlayer(){
    return pl;
}

template <typename DiverType>
bool Game::moveEntity(Position& new_pos, Position& curr_pos, DiverType &dv){
    if(sea_field.isEmptyPosition(new_pos) && dv.MoveDiver(new_pos)){
        sea_field.removeEntity(curr_pos);
        return sea_field.SetOnSeaZone(new_pos, dv);
    }
    return false;
}

GameConfig &Game::getConfig(){
    return config;
}

bool Game::getIsGameOn() {
    return isGameOn && pl.isAlive() && (!enemies.empty() || !octopuses.empty());
}

bool Game::getIsWin() {
    return pl.isAlive();
}

int Game::getDefEns(){
    return defeated_enemies;
}

void Game::plMovement(int dx, int dy) {
    if (!pl.isAlive() || !isGameOn || !pl.canAttackThisTurn()) {
        return;
    }

    Position curr_pos = pl.getPosition();
    Position new_pos(curr_pos.x + dx, curr_pos.y + dy);

    if (!sea_field.isValidPosition(new_pos)) {
        return;
    }

    Sea_sign nextZone = sea_field.getSeaZone(new_pos);

    if (nextZone.isEmpty()) {
        if (moveEntity(new_pos, curr_pos, pl)) {
            pl.performAttack();
        }
    }
    else if (nextZone.getFlag() == enemy_flag) {
        Enemy* enemy = findEnemyAtPosition(new_pos);
        if(enemy && pl.getAttackType() == common_attack) {
            enemy->takeDamage(pl.getDamage());
            pl.addScore();
            pl.performAttack();
            
            if (!enemy->isAlive()) {
                pl.addScore();
                pl.getBag().gift();
                removeDeads();
                sea_field.removeEntity(new_pos);
                moveEntity(new_pos, curr_pos, pl);
            }
        }
    }
    else if (nextZone.getFlag() == vortex_flag) {
        return;
    }
}

void Game::plRangedAttack(int target_x, int target_y) {
    if (!pl.isAlive() || !isGameOn || !pl.canAttackThisTurn()) {
        return;
    }

    if (pl.getAttackType() != AttackType::ranged_attack) {
        cout << "\033[41mОшибка: Для дальней атаки. Нужно переключиться на тип 'Дальний бой' (команда T)\033[0m" << endl;
        return;
    }

    Position playerPos = pl.getPosition();
    Position targetPos(target_x, target_y);
    if (!sea_field.isValidPosition(targetPos)) {
        return;
    }
    Sea_sign target = sea_field.getSeaZone(targetPos);
    
    int dx = abs(targetPos.x - playerPos.x);
    int dy = abs(targetPos.y - playerPos.y);
    
    if (target.getFlag() == enemy_flag && dx <= pl.getRangedRange() && dy <= pl.getRangedRange() && (dx + dy) > 0){
        Enemy* enemy = findEnemyAtPosition(targetPos);
        if (enemy) {
            enemy->takeDamage(pl.getDamage());
            pl.addScore();
            pl.performAttack();
            
            if (!enemy->isAlive()) {
                pl.addScore();
                pl.getBag().gift();
                removeDeads();
                sea_field.removeEntity(targetPos);
            }
        }else{
            cout << "\033[41mОшибка: В указанной позиции нет врага\033[0m" << endl;
        }
    }else if(target.getFlag() == octopus_flag && dx <= pl.getRangedRange() && dy <= pl.getRangedRange() && (dx + dy) > 0){
        Octopus* oct = findOctAtPosition(targetPos);
        if (oct) {
            oct->takeDamage(pl.getDamage());
            pl.addScore();
            pl.performAttack();
            
            if (!oct->isAlive()) {
                pl.addScore();
                pl.getBag().gift();
                removeDeads();
                sea_field.removeEntity(targetPos);
            }
        }else{
            cout << "\033[41mОшибка: В указанной позиции нет врага\033[0m" << endl;
        }
    }else{
        cout << "\033[41mОшибка: Цель вне зоны досягаемости дальней атаки\033[0m" << endl;
    }
}

Divers_bag& Game::getPlayerBag(){
    return pl.getBag();
}

void Game::choiseSpell(){
    pl.displayBag();
}

void Game::useSpell(int spellIndex, int x, int y) {
    if (!pl.isAlive() || !isGameOn || !pl.canAttackThisTurn()) {
        return;
    }

    Divers_bag &bag = pl.getBag();
    if (spellIndex < 0 || spellIndex >= bag.getSize()) {
        cout << "\033[41mНеверный номер заклинания!\033[0m" << endl;
        return;
    }

    Position pos(x, y);
    if (!sea_field.isValidPosition(pos)){
        cout << "\033[41mПозиция для использования заклинания вне диапазона!\033[0m" << endl;
        return;
    }

    if(!bag.whatGet(spellIndex)){
        bag.getTrap(spellIndex)->setTrap(pl, pos, sea_field);
    }else{
        Spell* spell = bag.getSpell(spellIndex);
        std::vector<Enemy>& ens = enemies;
        std::vector<Octopus>& oct = octopuses;
        if(spell->attack(pl, sea_field, ens, oct, pos)){
            pl.performAttack();
            bag.useSpell(spellIndex);
            removeDeads();
        }else{
            std::cout << "\033[41mНевозможно применить заклинание к указанной цели!\033[0m";
        }
    }
}

void Game::plSwitchAttackType() {
    if (!pl.isAlive() || !isGameOn || !pl.canAttackThisTurn()) {
        return;
    }
    pl.switchAttackType();
}

void Game::enMovement() {
    if(!isGameOn) {
        return;
    }

    Position player_pos = pl.getPosition();
    for(auto& enemy : enemies) {
        if(!enemy.isAlive()) continue;
        Position curr_pos = enemy.getPosition();
        int dx = 0, dy = 0;

        if(player_pos.x > curr_pos.x) dx = 1;
        else if(player_pos.x < curr_pos.x) dx = -1;
        
        if(player_pos.y > curr_pos.y) dy = 1;
        else if(player_pos.y < curr_pos.y) dy = -1;
        
        Position new_pos(curr_pos.x + dx, curr_pos.y + dy);
        
        if(sea_field.isValidPosition(new_pos)) {
            Sea_sign target = sea_field.getSeaZone(new_pos);
            
            if(target.isEmpty()) {
                moveEntity(new_pos, curr_pos, enemy);
            }
            else if(target.getFlag() == player_flag) {
                pl.takeDamage(enemy.getDamage());
                if(!pl.isAlive()) {
                    isGameOn = false;
                }
            }else if(target.getFlag() == fish_net_flag){
                Divers_bag &bag = pl.getBag();
                int damage = bag.useTrap(new_pos);
                enemy.takeDamage(damage);
                sea_field.removeEntity(new_pos);
                moveEntity(new_pos, curr_pos, enemy);
                bag.gift();
                removeDeads();
            }
        }
    }
}

void Game::octopus_attack(){
    if(!isGameOn) {
        return;
    }

    Position plPos = pl.getPosition();
    for(auto& oct : octopuses) {
        if(!oct.isAlive()) continue;
        Position currPos = oct.getPosition();
               
        if(abs(plPos.x - currPos.x) <= oct.getRange() && abs(plPos.y - currPos.y) <= oct.getRange()){
            pl.takeDamage(oct.getDamage());
            if(!pl.isAlive()){
                isGameOn = false;
            }
        }
    }
}

void Game::resetPlayerTurn() {
    pl.resetAttack();
}

void Game::printStatistics() {
    cout << "------------ Игровая статистика ------------" << endl;
    pl.getInfo();
    cout << setw(38) << left << "Врагов убито:" << setw(30) << left << defeated_enemies << endl;
    cout << setw(41) << left << "Можно атаковать:" << setw(30) << left << (pl.canAttackThisTurn() ? "Да" : "Нет") << endl;
    cout << "---------------------------------------------" << endl;
}

void Game::printControls() {
    cout << "Управление:" << endl;
    cout << "WASD - движение" << endl;
    cout << "T - переключить тип атаки" << endl;
    cout << "R - дальняя атака (укажите координаты)" << endl;
    cout << "E - открыть сумку с заклинаниями" << endl;
    cout << "Q - выход" << endl;
    cout << "---------------------------------------------" << endl;
}

void Game::render() {
    sea_field.display();
}

void Game::reset(GameConfig &new_cfg) {
    isGameOn = true;
    defeated_enemies = 0;

    config = new_cfg;
    sea_field.deallocateField();
    Sea new_sea(config);
    sea_field = new_sea;

    pl.reset();
    sea_field.SetOnSeaZone(config.pl_start_pos, pl);

    enemies.clear();
    octopuses.clear();
    vortexes.clear();

    spawn();
}

void Game::save(const std::string& path) const {
    try {
        SaveFileRAII file(path);

        config.saveTo(file);
        pl.saveTo(file);
        sea_field.saveTo(file);

        int j = (int)enemies.size();
        file.write(j);
        for(const auto& en : enemies){
            en.saveTo(file);
        }

        int k = (int)octopuses.size();
        file.write(k);
        for(const auto& oc : octopuses){
            oc.saveTo(file);
        }

        int z = (int)vortexes.size();
        file.write(z);
        for(const auto& vo : vortexes){
            vo.saveTo(file);
        }

        file.write(isGameOn).write(defeated_enemies);

    } catch (const std::exception& e) {
        std::cerr << "\033[41m[SAVE ERROR]\033[0m" << e.what() << std::endl;
        throw;
    }
}

void Game::load(const std::string& path){
    SaveFileRAII in = SaveFileRAII::load(path);

    config.loadFrom(in);
    pl.loadFrom(in);
    sea_field.loadFrom(in);

    enemies.clear();
    int n;
    in.read(n);
    for (int i = 0; i < n; ++i) {
        Enemy e(config);
        e.loadFrom(in);
        enemies.push_back(std::move(e));
    }

    octopuses.clear();
    int k;
    in.read(k);
    for (int i = 0; i < k; ++i) {
        Octopus o(config);
        o.loadFrom(in);
        octopuses.push_back(std::move(o));
    }

    vortexes.clear();
    int z;
    in.read(z);
    for (int i = 0; i < z; ++i) {
        Vortex v(config);
        v.loadFrom(in);
        vortexes.push_back(std::move(v));
    }

    in.read(isGameOn).read(defeated_enemies);
}