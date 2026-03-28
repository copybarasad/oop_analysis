#include "Game.h"
#include <iostream>
#include <limits>
#include <chrono>
#include <sstream>
#include "Config.h"
#include "SaveException.h"
#include "LoadException.h"
#include "InvalidSaveDataException.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "SfmlInputReader.h"
#include "GameController.h"
#include "SfmlRenderer.h"
#include "GameVisualizer.h"
#include "Command.h"
#include "GameSerializer.h"
#include "TurnSystem.h"


Game::Game(int width, int height, int totalLevels)
    :board_(width, height), player_(PLAYER_HP, PLAYER_MELEE_DAMAGE, PLAYER_RANGED_DAMAGE, {0, 0}),
      ui_mode_(UiMode::MainMenu), settings_board_width_(width),
      settings_board_height_(height), difficulty_(Difficulty::Normal),
      main_menu_(), settings_menu_(settings_board_width_, settings_board_height_, difficulty_), level_manager_(totalLevels, width,  height, difficulty_),
      hp_bonus_(0), melee_bonus_(0), direct_spell_bonus_(0),needResizeWindow_(false)  {
    std::random_device rd;
    rng_ = std::mt19937(rd());
    base_ = nullptr;
    cellH_ = 64.f;
    cellW_ = 64.f;
    needEnemyStep_ = false;

    info_message_ = "";

    panelW_ = 220.f;

    generateTerrain(10, 10);
    

    board_.at({0, 0}).setPassable(true);
    board_.at({0, 0}).setSlow(false);
    board_.at({0, 0}).setSymbol('.');

    board_.at({board_.width() - 1, board_.height() - 1}).setPassable(true);
    board_.at({board_.width() - 1, board_.height() - 1}).setSlow(false);
    board_.at({board_.width() - 1, board_.height() - 1}).setSymbol('.');

    player_.setPosition({0, 0});

    base_ = new EnemyBase({board_.width() - 1, board_.height() - 1}, BASE_SPAWN_DELAY);

    placeTower();

    initStartingSpell();
}

void Game::run() {
    unsigned winW = static_cast<unsigned>(board_.width() * cellW_ + panelW_);
    unsigned winH = static_cast<unsigned>(board_.height() * cellH_);

    sf::RenderWindow window( sf::VideoMode(winW, winH), "To survive in Moscow", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    GameController<SfmlInputReader> controller(*this);
    GameVisualizer<SfmlRenderer>   visualizer(*this);

    while (window.isOpen()) {
        if (ui_mode_ == UiMode::Playing) {
            controller.ProcessInput(window);
        } else {
            processEvents(window);
        }

        window.clear();
        visualizer.RenderFrame(window);
        window.display();
    }
}


void Game::processEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            return;
        }
        if (ui_mode_ == UiMode::LevelUp) {
            LevelUpChoice choice = level_up_menu_.HandleEvent(event);
            if (choice != LevelUpChoice::None) {
                ApplyLevelUpChoice(choice);
                goToNextLevel();
                ApplyBoardSizeToWindow(window);
                ui_mode_ = UiMode::Playing;
            }
            continue;
        }

        if (ui_mode_ == UiMode::MainMenu) {
            MenuCommand cmd = main_menu_.HandleEvent(event);
            switch (cmd) {
            case MenuCommand::None:
                break;

            case MenuCommand::NewGame:
                startNewGame();
                ApplyBoardSizeToWindow(window);
                ui_mode_ = UiMode::Playing;
                break;

            case MenuCommand::LoadGame:
                try {
                    LoadGame("save.json");
                    ApplyBoardSizeToWindow(window);
                    ui_mode_ = UiMode::Playing;
                    info_message_ = "Game loaded";
                } catch (const SaveLoadException& e) {
                    std::cout << "Load error: " << e.what() << '\n';
                    info_message_ = e.what();
                }
                break;

            case MenuCommand::OpenSettings:
                ui_mode_ = UiMode::Settings;
                break;

            case MenuCommand::Exit:
                window.close();
                break;
            }
            continue;
        }
        if (ui_mode_ == UiMode::Settings) {
            bool back_to_menu = settings_menu_.HandleEvent(event);
            if (back_to_menu) {
                ui_mode_ = UiMode::MainMenu;
            }
            continue;
        }
    }

    if (needResizeWindow_) {
        ApplyBoardSizeToWindow(window);
        needResizeWindow_ = false;
    }
}


bool Game::castSelectedSpellAt(std::pair<int,int> cell){
    ISpell* s = player_.hand().GetSelected();
    if(s == 0){
        return false;
    }

    SpellContext ctx;
    ctx.board   = &board_;
    ctx.player  = &player_;
    ctx.enemies = &enemies_;
    ctx.base    = base_;
    ctx.towers  = &towers_;
    ctx.allies  = &allies_;
    ctx.targetCell = cell;
    ctx.hasTarget  = s->NeedsTarget();

    ISpell* exec = s;
    ISpell* temp = 0;

    if(s->ConsumesModifier() && player_.HasPendingModifier()){
        SpellModifier mod = player_.PeekPendingModifier();
        temp = s->CloneNew();
        temp->ApplyModifier(mod);
        exec = temp;
    }

    bool used = false;
    if(exec->NeedsTarget()){
        if(!isInside(cell)){
            if(temp) delete temp;
            return false;
        }
        used = exec->Cast(ctx);
    }else{
        ctx.hasTarget = false;
        used = exec->Cast(ctx);
    }

    if(used){
        if(s->ConsumesModifier() && player_.HasPendingModifier()){
            player_.TakePendingModifier();
        }
        if(!s->IsPersistent()){
            player_.hand().RemoveSelected();
        }
        cleanupEnemies();
        cleanupAllies();
    }

    if(temp) delete temp;
    return used;
}


void Game::update() {
    if (!needEnemyStep_) return;
    if (state_ != GameState::Running) return;

    turn_system_.ProcessTurn(*this);

    cleanupEnemies();
    cleanupAllies();
    chekEnd();

    needEnemyStep_ = false;
}

bool Game::isInside(std::pair<int, int> p){
    return p.first >= 0 && p.second >= 0 && p.first < board_.width() && p.second < board_.height();
}

bool Game::isEnemyAt(std::pair<int, int> p, int* IndexOut){
    for (int i = 0; i < enemies_.size(); i++){
        if (enemies_[i].isAlive() && enemies_[i].position() == p){
            if (IndexOut) *IndexOut = i;
            return true;
        }
    }
    return false;
}


void Game::placeEntities(int enemyCount){
    enemyCount = 1;
    std::uniform_int_distribution<int> wx(0, board_.width() - 1);
    std::uniform_int_distribution<int> wy(0, board_.height() - 1);
    enemies_.clear();
    for (int i = 0; i< enemyCount; i++){
        for (int t = 0; t < 1000; t++){
            int ex = wx(rng_);
            int ey = wy(rng_);
            if (!board_.at({ex, ey}).isPassable()) continue;
            if (ex == 0 && ey == 0) continue;
            if (ex == board_.width() - 1 && ey == board_.height() - 1) continue;

            enemies_.push_back(Enemy(ENEMY_HP, ENEMY_DAMAGE, {ex, ey}));
            break;
        }
    }
}

void Game::generateTerrain(int WallPercent, int slowPercent){
    std::uniform_int_distribution <int> rnd(1, 100);

    for (int y = 0; y < board_.height(); y++){
        for (int x = 0; x < board_.width(); x++){
            Cell& c = board_.at({x, y});
            int roll = rnd(rng_);
            if (roll <= WallPercent){
                c.setPassable(false);
                c.setSlow(false);
                c.setSymbol('#');
            }else if(roll <= WallPercent + slowPercent){
                c.setPassable(true);
                c.setSlow(true);
                c.setSymbol('~');
            }else{
                c.setPassable(true);
                c.setSlow(false);
                c.setSymbol('.');
            }
        }
    }
}

void Game::chekEnd(){
    if (player_.hp() <= 0 && state_ == GameState::Running) {
        state_ = GameState::Defeat;
        endText_.setString("GAME OVER");
        endText_.setFillColor(sf::Color::Red);
    } else if (state_ == GameState::Running && player_.score() >= level_manager_.RequiredScore()) {

        state_ = GameState::Victory;

        level_up_menu_.Start(level_manager_.CurrentLevel(), level_manager_.TotalLevels());
        ui_mode_ = UiMode::LevelUp;
    }
}


void Game::initStartingSpell(){
    std::uniform_int_distribution<int> d(0,3);
    int r = d(rng_);
    if(r == 0){
        player_.hand().AddCard(new DirectDamageSpell(DIRECT_RANGE, DIRECT_DAMAGE + direct_spell_bonus_));
    }else if(r == 1){
        player_.hand().AddCard(new AoESpell(AOE_RANGE, AOE_DAMAGE, AOE_SIZE));
    }else if (r == 3){
         player_.hand().AddCard(new TrapSpell(TRAP_RANGE, TRAP_DAMAGE));
    }else{
        player_.hand().AddCard(new SummonSpell(1));
    }
}

void Game::grantRandomSpell(){
    if(player_.hand().GetCount() >= player_.hand().GetCapacity()){
        return;
    }
    std::uniform_int_distribution<int> d(0,3);
    int r = d(rng_);
    if(r == 0){
        player_.hand().AddCard(new DirectDamageSpell(DIRECT_RANGE, DIRECT_DAMAGE + direct_spell_bonus_));
    }else if(r == 1){
        player_.hand().AddCard(new AoESpell(AOE_RANGE, AOE_DAMAGE, AOE_SIZE));
    }else if(r == 2){
        player_.hand().AddCard(new TrapSpell(TRAP_RANGE, TRAP_DAMAGE));
    }else{
        player_.hand().AddCard(new SummonSpell(1));
    }
}

bool Game::tryBuySpell(){
    if(player_.hand().GetCount() >= player_.hand().GetCapacity()){
        return false;
    }
    if(!player_.spendScore(SPELL_BUY_COST)){
        return false;
    }
    grantRandomSpell();
    return true;
}


void Game::placeTower(){
    std::uniform_int_distribution<int> wx(0, board_.width() - 1);
    std::uniform_int_distribution<int> wy(0, board_.height() - 1);

    for(int t = 0; t < 1000; t++){
        int x = wx(rng_);
        int y = wy(rng_);
        if(!board_.at({x,y}).isPassable()) continue;
        if(x == 0 && y == 0) continue;
        if(base_ && base_->position().first == x && base_->position().second == y) continue;
        bool busy = false;
        for(int i = 0; i < (int)enemies_.size(); ++i){
            if(enemies_[i].isAlive() && enemies_[i].position().first == x && enemies_[i].position().second == y){
                busy = true; break;
            }
        }
        if(busy) continue;

        towers_.push_back(EnemyTower({x,y}, TOWER_RANGE, TOWER_DAMAGE, TOWER_COOLDOWN, TOWER_HP));
        break;
    }
}


bool Game::isTowerAt(std::pair<int,int> p, int* indexOut){
    for(int i = 0; i < (int)towers_.size(); ++i){
        if(towers_[i].isAlive() && towers_[i].position() == p){
            if(indexOut) *indexOut = i;
            return true;
        }
    }
    return false;
}

bool Game::isAllyAt(std::pair<int,int> p, int* indexOut){
    for(int i = 0; i < (int)allies_.size(); ++i){
        if(allies_[i].isAlive() && allies_[i].position() == p){
            if(indexOut) *indexOut = i;
            return true;
        }
    }
    return false;
}


void Game::awardEmpowerCardOrBuff(){
    if(player_.hand().GetCount() < player_.hand().GetCapacity()){
        player_.hand().AddCard(
            new EmpowerSpell(EMPOWER_RADIUS_DELTA, EMPOWER_AOE_DELTA, EMPOWER_DAMAGE_DELTA, EMPOWER_SUMMON_DELTA)
        );
        return;
    }
    SpellModifier gain;
    gain.radiusDelta = EMPOWER_RADIUS_DELTA;
    gain.aoeSizeDelta = EMPOWER_AOE_DELTA;
    gain.damageDelta = EMPOWER_DAMAGE_DELTA;
    gain.summonCountDelta = EMPOWER_SUMMON_DELTA;
    player_.AddPendingModifier(gain);
}


void Game::cleanupEnemies(){
    for(int i = (int)enemies_.size() - 1; i >= 0; --i){
        if(!enemies_[i].isAlive()){
            enemies_.erase(enemies_.begin() + i);
        }
    }
}

void Game::cleanupAllies(){
    for(int i = (int)allies_.size() - 1; i >= 0; --i){
        if(!allies_[i].isAlive()){
            allies_.erase(allies_.begin() + i);
        }
    }
}

std::string Game::DifficultyToString() const {
    switch (difficulty_) {
    case Difficulty::Easy: return "Easy";
    case Difficulty::Normal: return "Normal";
    case Difficulty::Hard: return "Hard";
    }
    return "Normal";
}

Difficulty Game::DifficultyFromString(const std::string& value) const {
    if (value == "Easy") return Difficulty::Easy;
    if (value == "Normal") return Difficulty::Normal;
    if (value == "Hard") return Difficulty::Hard;
    throw InvalidSaveDataException("Unknown difficulty in save: " + value);
}


void Game::SaveGame(const std::string& filename) {
    GameSerializer::Save(
        board_,
        player_,
        enemies_,
        allies_,
        base_,
        towers_,
        level_manager_,
        difficulty_,
        hp_bonus_,
        melee_bonus_,
        direct_spell_bonus_,
        filename
    );
}

void Game::LoadGame(const std::string& filename) {
    GameSerializer::Load(
        board_,
        player_,
        enemies_,
        allies_,
        base_,
        towers_,
        level_manager_,
        difficulty_,
        hp_bonus_,
        melee_bonus_,
        direct_spell_bonus_,
        filename
    );
    settings_board_width_  = board_.width();
    settings_board_height_ = board_.height();

    state_        = GameState::Running;
    endText_.setString("");
    needEnemyStep_ = false;
    ui_mode_      = UiMode::Playing;
}



void Game::setupLevelFromManager() {
    int w = level_manager_.BoardWidth();
    int h = level_manager_.BoardHeight();
    enemies_.clear();
    allies_.clear();
    towers_.clear();
    if (base_ != nullptr) {
        delete base_;
        base_ = nullptr;
    }

    board_ = Board(w, h);
    generateTerrain(10, 10);

    std::pair<int,int> startPos = {0, 0};
    std::pair<int,int> basePos  = {board_.width() - 1, board_.height() - 1};

    board_.at(startPos).setPassable(true);
    board_.at(startPos).setSlow(false);
    board_.at(startPos).setSymbol('.');

    board_.at(basePos).setPassable(true);
    board_.at(basePos).setSlow(false);
    board_.at(basePos).setSymbol('.');

    
    player_.setPosition(startPos);


    base_ = new EnemyBase(basePos, BASE_SPAWN_DELAY);

    placeEntities(1);
    placeTower();

    state_ = GameState::Running;
    needEnemyStep_ = false;
}

void Game::startNewGame() {
    level_manager_.Reset();

    hp_bonus_ = 0;
    melee_bonus_ = 0;
    direct_spell_bonus_ = 0;

    std::pair<int,int> startPos = {0, 0};
    player_ = Player(PLAYER_HP, PLAYER_MELEE_DAMAGE, PLAYER_RANGED_DAMAGE, startPos);
    player_.setPosition(startPos);

    initStartingSpell();

    setupLevelFromManager();
}

void Game::goToNextLevel() {
    if (!level_manager_.GoToNextLevel()) {
        state_ = GameState::Victory;
        endText_.setString("VICTORY");
        endText_.setFillColor(sf::Color::Green);
        return;
    }

    player_.setHp(PLAYER_HP + hp_bonus_);
    player_.setSkipNextTurn(false);

    Hand& hand = player_.hand();
    int count = hand.GetCount();
    int toRemove = count / 2;

    for (int i = 0; i < toRemove && hand.GetCount() > 0; ++i) {
        std::uniform_int_distribution<int> dist(0, hand.GetCount() - 1);
        int idx = dist(rng_);
        hand.SelectByIndex(idx);
        hand.RemoveSelected();
    }

    if (hand.GetCount() == 0) {
        initStartingSpell();
    }

    setupLevelFromManager();

    needResizeWindow_ = true;
}


void Game::ApplyBoardSizeToWindow(sf::RenderWindow& window){
    unsigned winW = static_cast<unsigned>(board_.width() * cellW_ + panelW_);
    unsigned winH = static_cast<unsigned>(board_.height() * cellH_);

    window.setSize(sf::Vector2u(winW, winH));

    sf::View view(sf::FloatRect(0.f, 0.f, static_cast<float>(winW), static_cast<float>(winH)));
    window.setView(view);
}

void Game::ApplyLevelUpChoice(LevelUpChoice choice){
    int HP_BONUS = 10;
    int MELEE_BONUS = 2;
    int DIRECT_SPELL_BONUS = 2;

    switch (choice) {
    case LevelUpChoice::IncreaseHp:
        hp_bonus_ += HP_BONUS;
        player_.setHp(PLAYER_HP + hp_bonus_);
        break;

    case LevelUpChoice::IncreaseMeleeDamage:
        melee_bonus_ += MELEE_BONUS;
        break;

    case LevelUpChoice::IncreaseDirectSpellDamage: {
        direct_spell_bonus_ += DIRECT_SPELL_BONUS;

        Hand& hand = player_.hand();
        int oldSelected = hand.GetSelectedIndex();

        for (int i = 0; i < hand.GetCount(); ++i) {
            hand.SelectByIndex(i);
            ISpell* s = hand.GetSelected();
            if (!s) continue;

            auto* direct = dynamic_cast<DirectDamageSpell*>(s);
            if (direct) {
                direct->IncreaseDamage(DIRECT_SPELL_BONUS);
            }
        }
        if (oldSelected >= 0 && oldSelected < hand.GetCount()) {
            hand.SelectByIndex(oldSelected);
        }

        break;
    }

    case LevelUpChoice::None:
        break;
    }
}

int Game::CurrentPlayerDamage(){
    int dmg = player_.damage();
    if (!player_.isMode()) {
        dmg += melee_bonus_;
    }
    return dmg;
}


bool Game::ApplyCommand(const Command& command){
    if (ui_mode_ != UiMode::Playing) {
        return false;
    }
    if (state_ != GameState::Running) {
        return false;
    }
    if (player_.shouldSkipTurn()) {
        player_.setSkipNextTurn(false);
        needEnemyStep_ = true;
        chekEnd();
        return true;
    }

    bool acted = false;

    switch (command.type) {
    case CommandType::Move:
    {
        int dx = command.deltaX;
        int dy = command.deltaY;

        if (dx == 0 && dy == 0) break;

        if (player_.isMode()) {
            const int range = RANGED_RANGE;
            std::pair<int,int> dir(dx, dy);
            auto p = player_.position();

            for (int step = 1; step <= range; ++step) {
                std::pair<int,int> cell(
                    p.first  + dir.first  * step,
                    p.second + dir.second * step
                );

                if (!isInside(cell)) break;
                if (!board_.at(cell).isPassable()) break;

                int towerIndex = -1;
                int dmg = CurrentPlayerDamage();
                if (isTowerAt(cell, &towerIndex)) {
                    bool wasAlive = towers_[towerIndex].isAlive();
                    towers_[towerIndex].takeDamage(dmg);
                    if (wasAlive && !towers_[towerIndex].isAlive()) {
                        awardEmpowerCardOrBuff();
                    }
                    break;
                }

                int enemyIndex = 0;
                if (isEnemyAt(cell, &enemyIndex)) {
                    enemies_[enemyIndex].takeDamage(dmg);
                    if (!enemies_[enemyIndex].isAlive()) {
                        player_.addScore(10);
                    }
                    break;
                }
            }

            acted = true;
        }
        else {
            auto pos = player_.position();
            std::pair<int,int> next(pos.first + dx, pos.second + dy);

            if (!isInside(next)) break;

            int enemyIndex = 0;
            if (isEnemyAt(next, &enemyIndex)) {
                int dmg = CurrentPlayerDamage();
                enemies_[enemyIndex].takeDamage(dmg);
                if (!enemies_[enemyIndex].isAlive()) {
                    player_.addScore(10);
                }
                cleanupEnemies();
                acted = true;
                chekEnd();
            } else {
                int towerIndex = -1;
                if (isTowerAt(next, &towerIndex)) {
                    if (towerIndex >= 0) {
                        int dmg = CurrentPlayerDamage();
                        bool wasAlive = towers_[towerIndex].isAlive();
                        towers_[towerIndex].takeDamage(dmg);
                        if (wasAlive && !towers_[towerIndex].isAlive()) {
                            awardEmpowerCardOrBuff();
                        }
                    }
                    cleanupEnemies();
                    acted = true;
                    chekEnd();
                } else {
                    int allyIndex = -1;
                    if (isAllyAt(next, &allyIndex)) {
                        break;
                    }

                    if (board_.at(next).isPassable()
                        && !(base_ && base_->position() == next)) {
                        player_.setPosition(next);
                        if (board_.at(next).isSlow()) {
                            player_.setSkipNextTurn(true);
                        }
                        acted = true;
                    }
                }
            }
        }
        break;
    }
    case CommandType::ToggleAttackMode:
        player_.AttackMobile();
        acted = true;
        break;

    case CommandType::SelectCard:
        if (command.index >= 0) {
            player_.hand().SelectByIndex(command.index);
        }
        break;

    case CommandType::BuySpell:
        tryBuySpell();
        break;

    case CommandType::CastSpellAtCell:
        if (command.hasTarget) {
            if (castSelectedSpellAt(command.targetCell)) {
                acted = true;
            }
        }
        break;

    case CommandType::None:
    default:
        break;
    }

    if (acted) {
        needEnemyStep_ = true;
        chekEnd();
    }

    return acted;
}


void Game::SaveDefault() {
    try {
        SaveGame("save.json");
        info_message_ = "Game saved";
    } catch (const SaveLoadException& e) {
        std::cout << "Save error: " << e.what() << '\n';
        info_message_ = e.what();
    }
}

void Game::LoadDefault() {
    try {
        LoadGame("save.json");
        info_message_ = "Game loaded";
    } catch (const SaveLoadException& e) {
        std::cout << "Load error: " << e.what() << '\n';
        info_message_ = e.what();
    }
}