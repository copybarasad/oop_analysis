#include "Game.h"
#include <iostream>
#include "spawn_system.h"
#include "movement_system.h"
#include "AIEnemy.h"
#include "spell_cast.h"
#include "random.h"
#include "directed_spell.h"
#include "area_spell.h" 
#include "trap_spell.h"
#include "player.h"
#include "hand_spells.h"
#include <fstream> 
#include <ctime>

Game::Game(int width, int height) 
    : field(new Field(width, height)), current_turn(0), game_over(false), level(1) {
    field->initilization_field(1);
}
int Game::getPlayerHP() const {
    return field->getPlayer() ? field->getPlayer()->getHP() : 0;
}

int Game::getPlayerMaxHP() const {
    return field->getPlayer() ? field->getPlayer()->getMaxHP() : 0;
}

int Game::getPlayerMana() const {
    return field->getPlayer() ? field->getPlayer()->getMana() : 0;
}

int Game::getPlayerPoints() const {
    return field->getPlayer() ? field->getPlayer()->getPoints() : 0;
}
int Game::getCntEnemyWithGame(){
    return field->getCntEnemy();
}
int Game::getCntBuildingWithGame(){
    return field->getCntBuilding();
}

void Game::incrementTurn() { current_turn++; } 
bool Game::isGameOver()  { return game_over; }

int Game::getLevel() { return level; }

std::vector<std::string> Game::getPlayerSpells() const {
    std::vector<std::string> spells;
    if (Player* player = field->getPlayer()) {
        Hand& hand = player->getHand();
        for (size_t i = 0; i < hand.getSpellCount(); ++i) {
            spells.push_back(hand.getSpell(i).getName());
        }
    }
    return spells;
}

nlohmann::json Game::toJson() const {
    return {
        {"game_state", {
            {"current_turn", current_turn},
            {"game_over", game_over}
        }},
        {"field", field->toJson()} 
    };
}

void Game::fromJson(const nlohmann::json& j) {
    auto& gameState = j["game_state"];
    current_turn = gameState["current_turn"];
    game_over = gameState["game_over"];
    
    field->fromJson(j["field"]);
}

void Game::enemy_move() {
    if (game_over) return;
    const auto& enemies = field->getEnemies();
    int i = 0;
    Player* gamer = field->getPlayer();
    while (i < field->getCntEnemy()) {
        bool flag = AIEnemy::choice_attack_go(*enemies[i], *gamer);
        if (gamer->getHP() <= 0){
            game_over = true;
            gamer->setHP(0);
            return;
        }
        if (!flag) {
            std::pair<int, int> pairs = AIEnemy::alg_finding_way(*enemies[i], *gamer);
            MovementSystem::moves_game_object(*enemies[i], pairs.first, pairs.second, *field);
        }
        const auto& enemies = field->getEnemies();
        if (i < field->getCntEnemy() && enemies[i]->getHP() > 0) {
            i++;
        }
    }
}

void Game::building_move() {
    if (game_over) return;
    int cnt_building = field->getCntBuilding();
    for (int i = 0; i < cnt_building; i++) {
        Building* build = field->getBuild(i);
        bool flag = SpawnSystem::spawn_enemy(*build, *field);

    }
}

void Game::spell_tower_move() {
    if (game_over) return;
    int cnt_spell_tower = field->getCntSpellTower();
    for (int i = 0; i < cnt_spell_tower; i++) {
        SpellTower* tower = field->getSpellTower(i);
        
        if (tower->getTimerCast() > 0) {
            tower->TimerReduce();
            continue; 
        }
        
        Player* gamer = field->getPlayer();
        
        int distance = std::max(
            std::abs(tower->getX() - gamer->getX()),
            std::abs(tower->getY() - gamer->getY())
        );
        
        if (distance <= tower->getSpell().getRange()) {
            bool cast_success = SpellCast::castFromTower(
                tower->getSpell(), 
                *field, 
                gamer->getX(), 
                gamer->getY()
            );
            
            if (cast_success) {
                tower->setTimer(tower->getRechargeCast());
                std::cout << "Башня заклинаний атаковала игрока! Урон: " 
                         << tower->getSpell().getDamage() << "\n";
            }
        }
    }
}


Field* Game::getField() { return field.get(); }

int Game::getCurrentTurn() { return current_turn; }

int Game::getLevel() const { return level; }

Player* Game::getPlayer() { return field->getPlayer(); }

int Game::getEnemyCount() const { return field->getCntEnemy(); }

int Game::getBuildingCount() const { return field->getCntBuilding(); }

bool Game::movePlayer(int dx, int dy){
    Player* player = field->getPlayer();
    if (!player || player->getHP() <= 0) return false;
    return MovementSystem::moves_game_object(*player, 
        player->getX() + dx, 
        player->getY() + dy, 
        *field);
}

bool Game::attack(int dx, int dy){
    Player* player = field->getPlayer();
    if (!player || player->getHP() <= 0) return false;
    return field->attack_player(dx, dy);
}
bool Game::searchSpellPlayer(std::string spell_name){
    return field->getPlayer()->getHand().SearchSpellTrue(spell_name);
}

std::unique_ptr<Game> Game::loadGame(std::string& filename) {
    try {
        std::ifstream file(filename + ".json");
        if (!file.is_open()) {
            throw std::runtime_error("Файл не найден: " + filename);
        }
        
        nlohmann::json saveData;
        file >> saveData;
        file.close();
        
        const auto& metadata = saveData["metadata"];
        if (metadata["type"] != "game_save") {
            throw std::runtime_error("Неверный формат файла сохранения");
        }
        
        auto game = std::make_unique<Game>(15, 15);
        game->fromJson(saveData["game"]);
        
        std::cout << "Игра загружена из файла: " << filename + ".json" << std::endl;
        return game;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка загрузки игры: " << e.what() << std::endl;
        return nullptr;
    }
}
bool Game::saveGame(std::string& filename) {
    try {
        nlohmann::json saveData = {
            {"metadata", {
                {"type", "game_save"},
                {"version", "1.0"},
                {"timestamp", std::time(nullptr)}
            }},
            {"game", this->toJson()}
        };
        
        std::ofstream file(filename + ".json");
        file << saveData.dump(1);
        file.close();
        
        std::cout << "Игра сохранена в файл: " << filename + ".json" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка сохранения игры: " << e.what() << std::endl;
        return false;
    }
}



int Game::getHpPlayer(){
    return field->getPlayer()->getHP();
}

void Game::restorePlayerHP(){
    field->getPlayer()->setHP(field->getPlayer()->getMaxHP());
}


bool Game::useSpell(std::string spell_name, int dx, int dy){
    Player* gamer = field->getPlayer();
    Spell* spell_ptr = gamer->getHand().SearchSpell(spell_name);
    if (!SpellCast::cast(*spell_ptr, *gamer, *field, dx + gamer->getX(), dy + gamer->getY())) {
        return false;
    }
    return true;
}
void Game::DelSpell(std::string spell_name){
    field->getPlayer()->getHand().removeSpell(spell_name);
}

void Game::switchToMelee(){
    field->switch_melee();
}
void Game::switchToRanged(){
    field->switch_ranged();    
}

void Game::spellUp(int spell_numer){
    field->getPlayer()->getHand().UpSpell(spell_numer);
}

void Game::levelUpPlayer(){
    field->getPlayer()->upPlayer();
}


void Game::newField(bool flag){

    int width = field->getWidth();
    int height = field->getHeight(); 
    
    std::unique_ptr<Player> new_player;
    Player* old_player = field->getPlayer();
    
    if (flag){
        new_player = std::make_unique<Player>(old_player->getX(), old_player->getY(),
        old_player->getHP(), old_player->getDamage(),  old_player->getPoints(),
        old_player->getMana(), old_player->getMaxHP());
        new_player->getHand().clearHand();
        Hand& old_hand = old_player->getHand();
        int len_hand_player = old_hand.getSpellCount();
        Hand& new_hand = new_player->getHand();
        int m = 0;
        int k = (len_hand_player+1)/2;
        while (m != k){
            int rnd_numb = random_numb(0, len_hand_player-1);
            std::string namespell = old_hand.getSpell(rnd_numb).getName();
            if (namespell == "Explosion"){
                new_hand.addSpell(std::make_unique<DirectedSpell>(1, "Explosion", 5, 30, 3));
            } else if (namespell == "Fireball"){
                new_hand.addSpell(std::make_unique<AreaSpell>(2, "Fireball", 12, 2, 2, 15, 6));
            } else if (namespell == "Mine"){
                new_hand.addSpell(std::make_unique<TrapSpell>(3, "Mine", 8, 30, 2));
            }
            old_hand.removeSpell(namespell);
            m++;
            len_hand_player--;
        }
    }
    if (flag){
        field = std::make_unique<Field>(width+5, height+5);
    } else{
        field = std::make_unique<Field>(width, height);

    }
    field->initilization_field(level);
    field->setPlayer(std::move(new_player));

    current_turn = 0;
}