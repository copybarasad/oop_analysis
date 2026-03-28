#include <algorithm>
#include <cmath>
#include <iomanip>

#include <fstream>
#include <random>
#include <filesystem>

#include "Game.h"
#include "SaveLoadException.h"

Game::Game(int width, int height)
    : width_(width)
    , height_(height)
    , playerPos_(0,0)
    , gameOver_(false)
    , level_(1)
{
}

void Game::CleanupDeadAndRewardPlayer(Player& player)
{
    int coinsBefore = playerCoinsForKills_;
    player.AddCoins(FetchAndClearCoinPool());
    
    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(),
        [](std::unique_ptr<Enemy>& e)
        {
            return !e->IsAlive();
        }),
        enemies_.end());

    allies_.erase(std::remove_if(allies_.begin(), allies_.end(),
        [](std::unique_ptr<Ally>& a)
        {
            return !a->IsAlive();
        }),
        allies_.end());
    
    (void)coinsBefore;

    towers_.erase(
        std::remove_if(towers_.begin(), towers_.end(),
            [](std::unique_ptr<Tower>& t)
            {
                return !t->IsAlive();
            }),
        towers_.end());
}


void Game::StartNew(int level)
{
    // очистка старого состояния (не удаляем player_, только сущности)
    enemies_.clear();
    towers_.clear();
    allies_.clear();
    traps_.clear();
    playerCoinsForKills_ = 0;
    gameOver_ = false;

    level_ = level;

    // настройка размеров поля по уровню
    if (level_ == 1) {
        width_ = 10;
        height_ = 10;
    }
    else if (level_ == 2) {
        width_ = 12;
        height_ = 12;
    }
    else {
        width_ = 14;
        height_ = 14;
    }

    // если игрока ещё нет — создааём, иначе восстанавливаем позицию/здоровье
    if (!player_) {
         player_ = std::make_unique<Player>(Position{2,2}, 5, 100, 100);
    }
    else {
        player_->SetPosition({2,2});
        //  восстановление хп игрока - те лечим на 10*level
        player_->TakeDamage(-10 * level_);
    }

    SetPlayerPosition(player_->GetPosition());

    // спелы руки
    while (player_->GetHand().Size() < player_->GetHand().Capacity()) {
        player_->GetHand().AddSpell(Hand::CreateRandomSpell());
    }

    //  с усилением по уровню
    AddEnemy(std::make_unique<Enemy>("enemy1", Position{4,2}, 6 + level_*4, 2 + level_, 2 + level_));
    AddEnemy(std::make_unique<Enemy>("enemy2", Position{8,8},  5 + level_*3, 1 + level_, 2 + level_));
    AddTower(std::make_unique<Tower>("Tower", Position{5, 5}, 30 + level_*10, 3 + level_, 6 + level_, 2));

    // удаляем половину карт рандомно
    size_t cur = player_->GetHand().Size();
    size_t toRemove = cur / 2;

    if (toRemove > 0) {
        std::mt19937_64 rng(std::random_device{}());

        for (size_t i=0;i<toRemove;i++) {
            if (player_->GetHand().Size() == 0) 
            {
                break;
            }
            size_t idx = rng() % player_->GetHand().Size();
            player_->GetHand().RemoveAt(idx);
        }
    }

    std::cout << "Started level " << level_ << " (map " << width_ << "x" << height_ << ")\n";
}

void Game::ProcessTurn()
{
    ++turnCount_;

    if (player_) {
        player_->CollectCoinsFromGame(*this);
    }

    for (auto& t : towers_) 
    {
        if (t->IsAlive()) 
        {
            t->OnTurn(*this);
        }
    }
    MoveEnemies();
    CheckTraps();
    AlliesTurn();
    
    if (player_) {
        CleanupDeadAndRewardPlayer(*player_);
    }

    if (turnCount_ % 5 == 0)
    {
        if (enemies_.size() < 3)
        {
            static int eid = 2;
            ++eid;
            Position spawnP{width_ - 1, 0 + (eid % height_)};
            
            AddEnemy(std::make_unique<Enemy>("enemy" + std::to_string(eid), spawnP, 6 + (turnCount_/5), 2, 3)); 
            
            // cout << "Spawned enemy " << eid << " at " << spawnP << endl; To logging system later
        }
    }
}

void Game::ApplyUpgrade(int choice)
{
    if (!player_) return; 
    
    if (choice == 1)
    {
        player_->TakeDamage(-10); // heal
    }
    else if (choice == 2)
    {
        player_->AddCoins(10);
    }
    else if (choice == 3)
    {
        if (player_->GetHand().Size() < player_->GetHand().Capacity())
        {
            player_->GetHand().AddSpell(Hand::CreateRandomSpell());
        }
    }
}

void Game::Run()
{
    std::cerr << "Warning: Game::Run() is deprecated. Use GameController.\n";
}

void Game::SpawnAlly(Position pos)
{
    auto ally = std::make_unique<Ally>("Summoned Ally", pos, 10, 2);
    allies_.push_back(std::move(ally));
}

bool Game::PlaceTrap(Position pos, const std::string& name, int damage)
{
    if (!IsInBounds(pos)) {
        return false;
    }

    if (IsOccupied(pos)) {
        return false;
    }

    // ловушка создается активной
    auto trap = std::make_unique<Trap>(name, pos, damage, true);
    traps_.push_back(std::move(trap));
    return true;
}

bool Game::IsInBounds(Position p) const
{
    return p.x >= 0 && p.x < width_ && p.y >= 0 && p.y < height_;
}

bool Game::IsOccupied(Position p) const
{
    for (auto const& t : towers_)
    {
        if (t->IsAlive() && t->GetPosition() == p)
        {
            return true;
        }
    }
    for (auto const& e : enemies_)
    {
        if (e->IsAlive() && e->GetPosition() == p)
        {
            return true;
        }
    }
    for (auto const& a : allies_)
    {
        if (a->IsAlive() && a->GetPosition() == p)
        {
            return true;
        }
    }
    return false;
}

bool Game::IsInRange(Position from, Position to, int range) const
{
    int dx = std::abs(from.x - to.x);
    int dy = std::abs(from.y - to.y);
    return dx <= range && dy <= range;
}

bool Game::DamageEnemyOrTowerAt(Position target, int damage)
{
    bool hit = false;
    for (auto& e : enemies_)
    {
        if (e->IsAlive() && e->GetPosition() == target)
        {
            e->TakeDamage(damage);
            hit = true;
            if (!e->IsAlive())
            {
                playerCoinsForKills_ += e->GetReward();
            }
        }
    }
    for (auto& t : towers_)
    {
        if (t->IsAlive() && t->GetPosition() == target)
        {
            t->TakeDamage(damage);
            hit = true;
        }
    }
    return hit;
}

void Game::DamageArea(Position topLeft, int sizeX, int sizeY, int damage)
{
    for (int y = topLeft.y; y < topLeft.y + sizeY; ++y)
    {
        for (int x = topLeft.x; x < topLeft.x + sizeX; ++x)
        {
            Position p{x, y};
            if (IsInBounds(p))
            {
                DamageEnemyOrTowerAt(p, damage);
            }
        }
    }
}

int Game::FetchAndClearCoinPool()
{
    int coins = playerCoinsForKills_;
    playerCoinsForKills_ = 0;
    return coins;
}

void Game::AddEnemy(std::unique_ptr<Enemy> enemy)
{
    enemies_.push_back(std::move(enemy));
}

void Game::AddTower(std::unique_ptr<Tower> tower)
{
    towers_.push_back(std::move(tower));
}

void Game::MoveEnemies()
{
    for (auto& enemy : enemies_)
    {
        if (!enemy->IsAlive()) {
            continue;
        }

        Position enemyPos = enemy->GetPosition();
        Position playerPos = playerPos_;

        // атака врага
        int dx = std::abs(enemyPos.x - playerPos.x);
        int dy = std::abs(enemyPos.y - playerPos.y);

        if (dx <= 1 && dy <= 1)
        {
            // std::cout << enemy->GetId() << " attacks player for " << enemy->GetDamage() << " damage!\n";
            player_->TakeDamage(enemy->GetDamage());

            if (!player_->IsAlive())
            {
                // std::cout << "Player was killed by " << enemy->GetId() << "!\n";
                gameOver_ = true;
                return;
            }
            continue;
        }
        //движение без атаки
        Position delta{0, 0};

        if (playerPos.x > enemyPos.x) {
            delta.x = 1;
        }
        else if (playerPos.x < enemyPos.x) {
            delta.x = -1;
        }
        if (playerPos.y > enemyPos.y) {
            delta.y = 1;
        }
        else if (playerPos.y < enemyPos.y) {
            delta.y = -1;
        }

        Position newPos{enemyPos.x + delta.x, enemyPos.y + delta.y};

        if (IsInBounds(newPos) && !IsOccupied(newPos)) {
            enemy->SetPosition(newPos);
        }
    }
}

void Game::CheckTraps()
{
    for (auto& trap : traps_)
    {
        if (!trap->active)
        {
            continue;
        }
        for (auto& enemy : enemies_)
        {
            if (enemy->IsAlive() && enemy->GetPosition() == trap->pos)
            {
                enemy->TakeDamage(trap->damage);
                trap->active = false;
                break;
            }
        }
    }
}

void Game::AlliesTurn()
{
    for (auto& a : allies_)
    {
        if (a->IsAlive())
            a->OnTurn(*this);
    }
}

void Game::TowerAttackPlayer(Tower& tower)
{
    // std::cout << "Tower " << tower.GetId() << " attacks player for " << tower.GetDamage() << " damage!\n";

    player_->TakeDamage(tower.GetDamage());
    if (!player_->IsAlive())
    {
        // std::cout << "Player was destroyed by tower!\n";
        gameOver_ = true;
    }
}

Position Game::GetPlayerPosition() const {
    return playerPos_;
}

Player* Game::GetPlayer() {
    return player_.get();
}

const Player* Game::GetPlayer() const {
    return player_.get();
}

bool Game::DamagePlayerAt(Position pos, int dmg)
{
    if (!player_) return false;

    if (player_->GetPosition() == pos)
    {
        player_->TakeDamage(dmg);
        // std::cout << "Player takes " << dmg 
        //           << " damage! HP now: " << player_->GetHp() << "\n";
        return true;
    }

    return false;
}

//************************************ */
void Game::Save(const std::string& filename) const
{
    std::filesystem::create_directory("Saves");
    std::string path = "Saves/" + filename;
    std::ofstream out(path, std::ios::trunc);

    if (!out.is_open()) {
        throw SaveException("Cannot open file for saving: " + path);
    }

    // LEVEL
    out << "LEVEL " << level_ << "\n";

    // WIDTH HEIGHT
    out << "WIDTH " << width_ << " HEIGHT " << height_ << "\n";

    // PLAYER
    if (!player_) throw SaveException("No player to save.");
    out << "PLAYER " 
        << player_->GetPosition().x << " "
        << player_->GetPosition().y << " "
        << player_->GetHp() << " "
        << player_->GetCoins() << "\n";

    // SPELLS
    auto spellNames = player_->GetHand().GetSpellNames();
    out << "SPELLS " << spellNames.size();
    for (auto const& s : spellNames) {
        std::string saveName;
        if (s == "DamageSpell" || s == "Damage spell") saveName = "DamageSpell";
        else if (s == "AreaDamageSpell" || s == "Area damage spell") saveName = "AreaDamageSpell";
        else if (s == "SummonSpell" || s == "Ally") saveName = "SummonSpell";
        else if (s == "BuffSpell" || s == "Buff Spell" || s == "Buff") saveName = "BuffSpell";
        else if (s == "TrapSpell" || s == "Trap") saveName = "TrapSpell";
        else saveName = "Unknown"; // Fallback, though shouldn't happen with standard spells
        out << " " << saveName;
    }
    out << "\n";

    // ENEMIES
    out << "ENEMIES " << enemies_.size() << "\n";
    for (auto const& e : enemies_) {
        if (!e) continue;
        out << "ENEMY " 
            << e->GetId() << " "
            << e->GetPosition().x << " "
            << e->GetPosition().y << " "
            << e->GetHp() << " "
            << e->GetReward() << " "
            << e->GetDamage() << "\n";
    }

    // TOWERS
    out << "TOWERS " << towers_.size() << "\n";
    for (auto const& t : towers_) {
        if (!t) continue;
        out << "TOWER "
            << t->GetId() << " "
            << t->GetPosition().x << " "
            << t->GetPosition().y << " "
            << t->GetHp() << " "
            << t->GetRange() << " "
            << t->GetDamage() << " "
            << t->GetCooldown() << "\n";
    }

    // TRAPS
    out << "TRAPS " << traps_.size() << "\n";
    for (auto const& tr : traps_) {
        if (!tr) continue;
        out << "TRAP "
            << tr->name << " "
            << tr->pos.x << " "
            << tr->pos.y << " "
            << tr->damage << "\n";
    }

    out.flush();
    if (!out) throw SaveException("Failed during write: " + path);
}

void Game::Load(const std::string& filename)
{
    std::string path = "Saves/" + filename;
    std::ifstream in(path);
    if (!in)
        throw LoadException("Cannot open: " + path);

    gameOver_ = false; // Reset game over state on load

    std::string token;

    // LEVEL
    in >> token;
    if (token != "LEVEL")
        throw LoadException("LEVEL missing");
    in >> level_;

    // WIDTH HEIGHT
    in >> token;
    if (token != "WIDTH")
        throw LoadException("WIDTH missing");
    in >> width_;
    in >> token;
    if (token != "HEIGHT")
        throw LoadException("HEIGHT missing");
    in >> height_;

    // PLAYER
    in >> token;
    if (token != "PLAYER")
        throw LoadException("PLAYER missing");

    int px, py, php, pcoins;
    in >> px >> py >> php >> pcoins;

    player_ = std::make_unique<Player>(Position{px, py}, 5, php, pcoins);

    // SPELLS
    in >> token;
    if (token != "SPELLS")
        throw LoadException("SPELLS missing");

    size_t scount;
    in >> scount;

    player_->GetHand().Clear();
    for (size_t i = 0; i < scount; ++i) {
        std::string sname;
        in >> sname; // названия спеллов должны быть однословные
        player_->GetHand().AddSpell(Hand::CreateSpellByName(sname));
    }

    // ENEMIES
    in >> token;
    if (token != "ENEMIES")
        throw LoadException("ENEMIES missing");

    size_t ecount;
    in >> ecount;
    enemies_.clear();

    for (size_t i = 0; i < ecount; ++i) {
        std::string id;
        int ex, ey, ehp, rew, dmg;
        in >> token; // должно быть "ENEMY"
        if (token != "ENEMY")
            throw LoadException("ENEMY token expected");

        in >> id >> ex >> ey >> ehp >> rew >> dmg;
        enemies_.push_back(std::make_unique<Enemy>(id, Position{ex, ey}, ehp, rew, dmg));
    }

    // TOWERS
    in >> token;
    if (token != "TOWERS")
        throw LoadException("TOWERS missing");

    size_t tcount;
    in >> tcount;
    towers_.clear();

    for (size_t i = 0; i < tcount; ++i) {
        std::string id;
        int tx, ty, thp, range, dmg, cd;
        in >> token; // должно быть "TOWER"
        if (token != "TOWER")
            throw LoadException("TOWER token expected");

        in >> id >> tx >> ty >> thp >> range >> dmg >> cd;
        towers_.push_back(std::make_unique<Tower>(id, Position{tx, ty}, thp, range, dmg, cd));
    }

    // TRAPS
    in >> token;
    if (token != "TRAPS")
        throw LoadException("TRAPS missing");

    size_t trcount;
    in >> trcount;
    traps_.clear();

    for (size_t i = 0; i < trcount; ++i) {
        std::string name;
        int tx, ty, dmg, active;
        in >> token; // должно быть "TRAP"
        if (token != "TRAP")
            throw LoadException("TRAP token expected");

        in >> name >> tx >> ty >> dmg >> active;
       traps_.push_back(std::make_unique<Trap>(name, Position{tx, ty}, dmg, active == 1));
        traps_.back()->active = (active == 1);
    }

    // восстановим позицию игрока в игре
    SetPlayerPosition(player_->GetPosition());
}