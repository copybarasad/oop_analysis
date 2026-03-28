#include "gamemodel.hpp"
#include "enemy_building.hpp"
#include "constants.hpp"
#include "event_dispatcher.hpp"
#include "events.hpp"
#include "cell.hpp"  
#include <cstdlib>
#include <ctime>

using namespace configs;

Game::Game(int width, int height): field_(width, height), player_(100, 10), move_(*this), initialWidth_(width), initialHeight_(height) {
    InitializeWorld();
}

std::pair<int, int> Game::GetRandomEmptyCell() {
    int x, y;
    do {
        x = rand() % field_.getWidth();
        y = rand() % field_.getHeight();
    } while (field_.At(x, y).getType() != CellType::kEmpty);
    return {x, y};
}

bool Game::InBounds(int x, int y) const {
    return x >= 0 && y >= 0 && x < field_.getWidth() && y < field_.getHeight();
}

void Game::RemoveDeadEnemies(std::vector<EnemyUnit>& enemies) {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
                       [](const EnemyUnit& e) { return !e.enemy.isAlive(); }),
        enemies.end());
}

void Game::RemoveDeadAllies(std::vector<std::unique_ptr<Ally>>& allies) {
    allies.erase(
        std::remove_if(allies.begin(), allies.end(),
            [](const std::unique_ptr<Ally>& ally) {
                return !ally->isAlive();
            }),
        allies.end());
}

void Game::RewardPlayer(RewardType type) {
    auto& player = getPlayer();

    switch (type) {
        case RewardType::KillEnemy:
            player.AddScore(kScore);
            player.AddKills();
            break;

        case RewardType::DestroyStructure:
            player.AddScore(kScore * 2);
            break;
    }
}

void Game::Cleanup() {
    RemoveDeadEnemies(enemies_);
    RemoveDeadAllies(allies_);
}

bool Game::AllEnemiesDefeated() const {
    for (const auto& e : enemies_) {
        if (e.enemy.isAlive())
            return false;
    }
    return true;
}

void Game::AddTrap(int x, int y, int damage) {
    traps_.push_back(std::make_unique<Trap>(x, y, damage));
    field_.At(x, y).set(CellType::kTrap);
}

void Game::AddAlly(int x, int y) {
    allies_.push_back(std::make_unique<Ally>(x, y, configs::kBaseAllyHealth, configs::kBaseAllyDamage));
    field_.At(x, y).set(CellType::kAlly);
}

void Game::CreateEmptyWorld(int width, int height) {
    enemies_.clear();
    buildings_.clear();
    towers_.clear();
    traps_.clear();
    allies_.clear();
    next_enemy_id_ = 1;

    field_ = Field(width, height);
}

void Game::PopulateLevelRandom(int level) {
    player_.setPosition(0, 0);
    field_.At(0, 0).set(CellType::kPlayer);
    int enemyCount = kEnemyCount + level;
    for (int i = 0; i < enemyCount; ++i) {
        auto cell = GetRandomEmptyCell();
        int ex = cell.first;
        int ey = cell.second;
        int health = (20 + rand() % 20) + level;  
        int damage = (4 + rand() % 5) + level;    
        enemies_.emplace_back(EnemyUnit{next_enemy_id_++, Enemy(health, damage), ex, ey});
        field_.At(ex, ey).set(CellType::kEnemy, enemies_.back().id);
    }
    auto cell1 = GetRandomEmptyCell();
    buildings_.emplace_back(cell1.first, cell1.second, kBuildingSpawnDelay - level);       
    field_.At(cell1.first, cell1.second).set(CellType::kBuilding);
    auto cell2 = GetRandomEmptyCell();
    int tx = cell2.first;
    int ty = cell2.second;
    int towerHealth = kBaseHealthTower + level * 20;
    int towerDamage = kBaseDamageTower + level * 3;
    int radius = kAttackRadiusTower + (level / 3);
    int delay = kTowerDelay - (level > 3 ? 1 : 0); 
    towers_.emplace_back(tx, ty, towerHealth, towerDamage, radius, delay);
    field_.At(tx, ty).set(CellType::kTower);
}

void Game::InitializeWorld() {
    srand(static_cast<unsigned>(time(nullptr)));
    player_.setMaxHealth(100);  
    player_.setDamage(10);     
    player_.setScore(0);
    player_.setKills(0);
    player_.setRanged(false);
    CreateEmptyWorld(initialWidth_, initialHeight_);
    player_.setHealth(player_.getMaxHealth());
    PopulateLevelRandom(0); 
}

void Game::GenerateNextLevel(int level) {
    int newWidth  = field_.getWidth()  + level;
    int newHeight = field_.getHeight() + level;
    CreateEmptyWorld(newWidth, newHeight);
    player_.Heal(player_.getMaxHealth()); 
    PopulateLevelRandom(level);
    turnNumber_ = 0;
    EventDispatcher::instance().notify(
        TurnCompletedEvent(turnNumber_, false)
    );
}

void Game::NextTurn()
{
    ++turnNumber_;
    bool combatHappened = false;
    for (auto& b : buildings_)
        b.Tick(*this);
    for (auto& t : towers_)
        t.Tick(*this);
    int prevHp = player_.getHealth();
    move_.MoveAllies();
    move_.MoveEnemies();
    Cleanup();
    if (player_.getHealth() < prevHp)
        combatHappened = true;
    if (!combatHappened)
    {
        ++no_combat_turns_;
        if (no_combat_turns_ >= configs::kAmountWithoutAttack)
        {
            player_.Heal(configs::kHealAmount);
            no_combat_turns_ = 0;
            EventDispatcher::instance().notify(
                UiMessageEvent(
                    "🌿 You had some rest and regained health! HP = " +
                    std::to_string(player_.getHealth())
                )
            );
        }
    }
    else {
        no_combat_turns_ = 0;
    }
    EventDispatcher::instance().notify(TurnCompletedEvent{turnNumber_, combatHappened});
}