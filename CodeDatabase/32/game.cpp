#include "game.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "direct_damage_spell.hpp"
#include "area_damage_spell.hpp"
#include "trap.hpp"
#include "enemy_tower.hpp"
#include "summon_spell.hpp"
#include "enhancementspell.hpp"
#include "spell_factory.hpp"
#include "file_raii.hpp"
#include "game_io.hpp"
#include "constants.hpp"

Game::Game()
    : player_("Артур", PLAYER_HEALTH, PLAYER_DAMAGE),
      field_(FIELD_WIDTH, FIELD_HEIGHT), level_(1) {
    player_.SetPosition(0, 0);
    InitEnemies();
    InitPotions();
    InitBuildings();

    SpellFactory factory;
    player_.GetHand().AddSpell(factory.GenerateRandomSpell());
}

Player& Game::GetPlayer() { return player_; }
Field& Game::GetField() { return field_; }
std::vector<Enemy>& Game::GetEnemies() { return enemies_; }
std::vector<EnemyBuilding>& Game::GetBuildings() { return buildings_; }
std::vector<EnemyTower>& Game::GetTowers() { return towers_; }
std::vector<Trap>& Game::GetTraps() { return traps_; }
std::vector<Ally>& Game::GetAllies() { return allies_; }

int Game::GetLevel() const {
    return level_;
}

void Game::NextLevel() {
    level_++;

    int newSize = FIELD_WIDTH + (level_ - 1) * 2;

    field_ = Field(newSize, newSize);

    int newMaxHp = player_.GetMaxHealth() + 20;
    player_.SetMaxHealth(newMaxHp);
    player_.SetHealth(newMaxHp);
    int newDamage = player_.GetDamage() + 5;
    player_.SetDamage(newDamage);

    player_.GetHand().RemoveRandomHalf();

    GenerateLevel(level_);
}

bool Game::IsLevelCompleted() const {
    for (const auto& e : enemies_) {
        if (e.IsAlive()) return false;
    }
    return true;
}

void Game::GenerateLevel(int level) {
    field_ = Field(field_.width(), field_.height());

    enemies_.clear();
    buildings_.clear();
    traps_.clear();
    towers_.clear();
    allies_.clear();

    player_.SetPosition(0, 0);
    field_.At(0, 0).SetType(CellType::kPlayer);

    int enemy_count = 3 + level; 
    for (int i = 0; i < enemy_count; ++i) {
        int hp   = ENEMY_GOBLIN_HP  + (level - 1) * 10;
        int dmg  = ENEMY_GOBLIN_DMG + (level - 1) * 3; 
        Enemy e("Гоблин", hp, dmg);

        auto [x, y] = FindFreeCell();
        e.SetPosition(x, y);
        enemies_.push_back(e);
    }

    int numPotions = POTION_MIN_COUNT + std::rand() % POTION_COUNT_VARIATION;
    for (int i = 0; i < numPotions; ++i) {
        auto [x, y] = FindFreeCell();
        int type = std::rand() % POTION_TYPE_COUNT;
        field_.At(x, y).SetType(
            type == 0 ? CellType::kPotionSmall : CellType::kPotionLarge
        );
    }

    {
        auto [bx, by] = FindFreeCell();
        buildings_.emplace_back(bx, by, BUILDING_SPAWN_DELAY);
        field_.At(bx, by).SetType(CellType::kEnemyBuilding);
    }

    if (level >= 2) {
        auto [tx, ty] = FindFreeCell();
        towers_.emplace_back(tx, ty, this);
        field_.At(tx, ty).SetType(CellType::kEnemyTower);
}
}

std::pair<int, int> Game::FindFreeCell() {
    int x, y;
    bool free_found = false;

    while (!free_found) {
        x = std::rand() % field_.width();
        y = std::rand() % field_.height();
        free_found = true;

        if (x == player_.GetX() && y == player_.GetY())
            free_found = false;

        for (const auto& e : enemies_)
            if (e.GetX() == x && e.GetY() == y)
                free_found = false;

        for (const auto& b : buildings_)
            if (b.GetX() == x && b.GetY() == y)
                free_found = false;

        for (const auto& t : towers_)
            if (t.GetX() == x && t.GetY() == y)
                free_found = false;

        if (field_.At(x, y).type() != CellType::kEmpty)
            free_found = false;
    }

    return {x, y};
}

void Game::InitEnemies() {
    enemies_.emplace_back("Гоблин", ENEMY_GOBLIN_HP, ENEMY_GOBLIN_DMG);
    enemies_.emplace_back("Гоблин", ENEMY_GOBLIN_HP, ENEMY_GOBLIN_DMG);
    enemies_.emplace_back("Гоблин", ENEMY_GOBLIN_HP, ENEMY_GOBLIN_DMG);

    for (auto& e : enemies_) {
        auto [x, y] = FindFreeCell();
        e.SetPosition(x, y);
    }
}

void Game::InitPotions() {
    int numPotions = POTION_MIN_COUNT + std::rand() % POTION_COUNT_VARIATION;

    for (int i = 0; i < numPotions; ++i) {
        auto [x, y] = FindFreeCell();

        int typeRand = std::rand() % POTION_TYPE_COUNT;
        field_.At(x, y).SetType(
            typeRand == 0 ? CellType::kPotionSmall : CellType::kPotionLarge);
    }
}

void Game::InitBuildings() {
    auto [bx, by] = FindFreeCell();
    buildings_.emplace_back(bx, by, BUILDING_SPAWN_DELAY);
    field_.At(bx, by).SetType(CellType::kEnemyBuilding);
}

void Game::InitTowers() {
    auto [tx, ty] = FindFreeCell();
    towers_.emplace_back(tx, ty, this);
    field_.At(tx, ty).SetType(CellType::kEnemyTower);
}

void Game::EnemyTryMeleeAttack(Enemy& e, int playerX, int playerY) {
    const int dist = std::abs(e.GetX() - playerX) + std::abs(e.GetY() - playerY);

    if (dist == ATTACK_RANGE_MELEE && !e.HasAttacked()) {
        std::cout << "⚔️ " << e.GetName()
                  << " атакует игрока вблизи и наносит "
                  << e.GetDamage() << " урона!\n";
        player_.TakeDamage(e.GetDamage());
        e.SetAttacked(true);
        std::cout << "❤️ HP игрока после атаки: "
                  << player_.GetHealth() << "\n\n";
    } else if (dist > ATTACK_RANGE_MELEE && e.HasAttacked()) {
        e.SetAttacked(false);
    }
}

void Game::MoveEnemies() {
    const int playerX = player_.GetX();
    const int playerY = player_.GetY();

    for (auto& e : enemies_) {
        if (e.GetHealth() <= 0)
            continue;

        int dx = 0, dy = 0;
        if (playerX != e.GetX())
            dx = (playerX > e.GetX()) ? 1 : -1;
        else if (playerY != e.GetY())
            dy = (playerY > e.GetY()) ? 1 : -1;

        const int newX = e.GetX() + dx;
        const int newY = e.GetY() + dy;

        if (newX < 0 || newY < 0 ||
            newX >= field_.width() || newY >= field_.height())
            continue;

        auto targetType = field_.At(newX, newY).type();
        if (targetType != CellType::kEmpty && targetType != CellType::kTrap)
            continue;

        if (newX == playerX && newY == playerY) {
            EnemyTryMeleeAttack(e, playerX, playerY);
            continue;
        }

        bool occupied = false;
        for (const auto& other : enemies_) {
            if (&other == &e || other.GetHealth() <= 0)
                continue;
            if (other.GetX() == newX && other.GetY() == newY) {
                occupied = true;
                break;
            }
        }
        if (occupied)
            continue;

        bool cell_has_ally = false;
        for (const auto& a : allies_) {
            if (a.IsAlive() && a.GetX() == newX && a.GetY() == newY) {
                cell_has_ally = true;
                break;
            }
        }
        if (cell_has_ally)
            continue;

        e.SetPosition(newX, newY);
        EnemyTryMeleeAttack(e, playerX, playerY);
    }

    CheckTraps();

    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(),
                       [](const Enemy& e) { return !e.IsAlive(); }),
        enemies_.end()
    );

    for (auto& t : towers_) {
        t.Tick();
    }

    buildings_.erase(
        std::remove_if(buildings_.begin(), buildings_.end(),
                       [](const EnemyBuilding& b) { return b.IsDestroyed(); }),
        buildings_.end()
    );
}

void Game::PrintState() {
    Field temp(field_.width(), field_.height());

    for (int y = 0; y < field_.height(); ++y)
        for (int x = 0; x < field_.width(); ++x)
            temp.At(x, y).SetType(CellType::kEmpty);

    for (int y = 0; y < field_.height(); ++y)
        for (int x = 0; x < field_.width(); ++x) {
            CellType t = field_.At(x, y).type();
            if (t != CellType::kPlayer &&
                t != CellType::kEnemy &&
                t != CellType::kAlly)
            {
                temp.At(x, y).SetType(t);
            }
        }

    for (auto& e : enemies_)
        if (e.IsAlive())
            temp.At(e.GetX(), e.GetY()).SetType(CellType::kEnemy);

    for (auto& a : allies_)
        if (a.IsAlive())
            temp.At(a.GetX(), a.GetY()).SetType(CellType::kAlly);

    for (auto& tr : traps_)
        temp.At(tr.GetX(), tr.GetY()).SetType(CellType::kTrap);

    for (auto& b : buildings_)
        if (!b.IsDestroyed())
            temp.At(b.GetX(), b.GetY()).SetType(CellType::kEnemyBuilding);

    for (auto& t : towers_)
        temp.At(t.GetX(), t.GetY()).SetType(CellType::kEnemyTower);

    temp.At(player_.GetX(), player_.GetY()).SetType(CellType::kPlayer);
    temp.Print();
    std::cout << "HP игрока: " << player_.GetHealth() << "\n";
}

void Game::AddTrap(int x, int y, int damage) {
    traps_.emplace_back(x, y, damage);
}

void Game::CheckTraps() {
    auto& field = GetField();

    for (auto& enemy : enemies_) {
        if (!enemy.IsAlive()) continue;

        for (auto it = traps_.begin(); it != traps_.end();) {
            if (enemy.GetX() == it->GetX() && enemy.GetY() == it->GetY()) {
                std::cout << "☠️ Враг " << enemy.GetName()
                          << " наступает на ловушку и получает "
                          << it->GetDamage() << " урона!\n";
                enemy.TakeDamage(it->GetDamage());
                field.At(it->GetX(), it->GetY()).SetType(CellType::kEmpty);
                it = traps_.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void Game::AddAlly(const Ally& ally) {
    allies_.push_back(ally);
}

void Game::AllyTryAttackEnemy(Ally& a) {
    if (!a.IsAlive()) return;

    for (auto& e : enemies_) {
        if (!e.IsAlive()) continue;

        int dist = std::abs(a.GetX() - e.GetX()) + std::abs(a.GetY() - e.GetY());
        if (dist == ATTACK_RANGE_MELEE) {
            std::cout << "🗡️ Союзник " << a.GetName()
                      << " атакует врага " << e.GetName()
                      << " и наносит " << a.GetDamage() << " урона!\n";

            e.TakeDamage(a.GetDamage());
            a.IncreaseAttackCount();

            if (!e.IsAlive()) {
                std::cout << "💀 Враг " << e.GetName() << " повержен союзником!\n";
            }

            if (a.IsExpired()) {
                std::cout << "💨 Союзник " << a.GetName()
                          << " испарился после трёх атак!\n";
                field_.At(a.GetX(), a.GetY()).SetType(CellType::kEmpty);
                a.TakeDamage(a.GetHealth());
            }

            return;
        }
    }
}

void Game::MoveAllies() {
    for (auto& ally : allies_) {
        if (!ally.IsAlive()) continue;
        ally.Move(enemies_, field_, player_);
        AllyTryAttackEnemy(ally);
    }

    allies_.erase(
        std::remove_if(allies_.begin(), allies_.end(),
                       [](const Ally& a) { return !a.IsAlive(); }),
        allies_.end());
}

void Game::ForceResizeField(int w, int h) {
    field_ = Field(w, h);
}

void Game::SetLevel(int lvl) {
    level_ = lvl;
}
