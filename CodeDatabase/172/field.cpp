#include "field.h"
#include "map_generator.h"
#include "spell_context.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <stdexcept>

namespace game {
    // ========= Constructors =========

    // --------------- Default constructor ---------------
    Field::Field(int w, int h, const GameConfig& cfg, std::mt19937& rng)
        : cfg_(cfg), rng_(rng), turnCounter_(1), player_(cfg, Pos{0, 0}, rng_)
    {
        if (w < cfg.minSize || w > cfg.maxSize ||
            h < cfg.minSize || h > cfg.maxSize)
        {
            throw std::invalid_argument("Invalid field size!");
        }

        MapGenerator mgen;
        MapData md = mgen.generateMap(w, h, cfg, rng);

        width_ = md.width;
        height_ = md.height;
        cells_ = std::move(md.cells);
        player_ = Player(cfg_, md.playerStart, rng_);
        
        ebuildings_.emplace_back(md.enemyBuilding,
                                 cfg_.defaultSpawnInterval,
                                 cfg_.enemiesPerSpawn);
        
        etower_ = ETower(md.enemyTower, cfg_.eTowerDamage, cfg_.eTowerRadius, cfg_.eTowerCooldown);

        const Pos playerPos = md.playerStart;
        cells_[playerPos.y][playerPos.x].placePlayer();

        spawnEnemyNearBuilding(ebuildings_.front());
        for (Enemy& e: enemies_) {
            const Pos enemyPos = e.getPosition();
            cells_[enemyPos.y][enemyPos.x].placeEnemy();
        }
    }

    #ifdef TESTING
    Field::Field(const GameConfig& cfg, MapData& md, std::mt19937& rng)
        : width_(md.width), height_(md.height), cfg_(cfg),
        rng_(rng), turnCounter_(1),
        cells_(md.cells), player_(cfg_, md.playerStart),
        enemies_(), ebuildings_()
    {   
        

        ebuildings_.emplace_back(md.enemyBuilding,
                                 cfg_.defaultSpawnInterval,
                                 cfg_.enemiesPerSpawn);

        const Pos playerPos = md.playerStart;
        cells_[playerPos.y][playerPos.x].placePlayer();

        spawnEnemyNearBuilding(ebuildings_.front());
        for (Enemy& e: enemies_) {
            const Pos enemyPos = e.getPosition();
            cells_[enemyPos.y][enemyPos.x].placeEnemy();
        }
    }
    #endif
    // ----------------- Copy constructor ----------------
    Field::Field(const Field& other)
        : width_(other.width_), height_(other.height_), cfg_(other.cfg_),
          rng_(std::random_device{}()), turnCounter_(other.turnCounter_),
          cells_(other.cells_), player_(other.player_),
          enemies_(other.enemies_), ebuildings_(other.ebuildings_)
    {}

    // ----------------- Move constructor ----------------
    Field::Field(Field&& other)
        : width_(other.width_), height_(other.height_), cfg_(other.cfg_),
          rng_(std::random_device{}()), turnCounter_(other.turnCounter_),
          cells_(std::move(other.cells_)), player_(std::move(other.player_)),
          enemies_(std::move(other.enemies_)), ebuildings_(std::move(other.ebuildings_))
    {
        other.width_ = other.height_ = other.turnCounter_ = 0;
    }


    // ========== Operators ===========

    // ---------------- Copy assignment op ----------------
    Field& Field::operator=(const Field& other)
    {
        if (this == &other) return *this;

        width_ = other.width_;
        height_ = other.height_;
        cfg_ = other.cfg_;
        rng_ = std::mt19937(std::random_device{}());
        turnCounter_ = other.turnCounter_;
        cells_ = other.cells_;
        player_ = other.player_;
        enemies_ = other.enemies_;
        ebuildings_ = other.ebuildings_;

        return *this;
    }

    // ---------------- Move assignment op ----------------
    Field& Field::operator=(Field&& other)
    {
        if (this == &other) return *this;

        width_ = other.width_;
        height_ = other.height_;
        cfg_ = std::move(other.cfg_);
        rng_ = std::move(other.rng_);
        turnCounter_ = other.turnCounter_;
        cells_ = std::move(other.cells_);
        player_ = std::move(other.player_);
        enemies_ = std::move(other.enemies_);
        ebuildings_ = std::move(other.ebuildings_);

        other.width_ = other.height_ = other.turnCounter_ = 0;

        return *this;
    }


    // =========== Helpers ============

    // -------------- Get enemy index at pos --------------
    int Field::enemyIndexAt(Pos p) const 
    {
        for (size_t i = 0; i < enemies_.size(); ++i) {
            if (enemies_[i].isAlive() && enemies_[i].getPosition() == p) return (int)i;
        }
        return -1;
    }
    
    // ---------------- Check pos for enemy ---------------
    bool Field::hasEnemyAt(Pos p) const  { return enemyIndexAt(p) >= 0; }

    bool Field::damageEnemyAt(Pos p, int dmg)
    {
        if (!hasEnemyAt(p)) {
            return false;
        }

        int index = enemyIndexAt(p);
        Enemy& e = enemies_[index];

        if (e.isAlive()) e.takeDamage(dmg);
        if (e.isCorpse()) createCorpse(e);
        
        return true;
    }

    bool Field::tryCastByIndex(size_t index, std::optional<Pos> target)
    {
        if (index >= player_.getHand().size()) return false;

        SpellContext ctx {*this, player_, cfg_, rng_, target, target};

        if (auto* spell = player_.getHand().atMutable(index)) {
            if (spell->canCast(ctx) && spell->cast(ctx)) {
                player_.getHand().removeAt(index);
                nextTurn();
                return true;
            }
        }

        return false;
    }

    std::optional<Pos> Field::findNearestEnemy(Pos from, int maxRange) const
    {
        int bestDist = cfg_.maxSize;
        std::optional<Pos> best = std::nullopt;

        for (const Enemy& e: enemies_) {
            const Pos p = e.getPosition();
            const int dist = std::abs(p.x - from.x) + std::abs(p.y - from.y);

            if (maxRange >= 0 && dist > maxRange) continue;
            if (dist < bestDist) {
                bestDist = dist;
                best = p;
            }
        }

        return best;
    }

    bool Field::applyAreaDamage(Pos center, int radius, int dmg)
    {
        if (radius < 0) return false;

        for (int y = center.y - radius; y <= center.y + radius; ++y) {
            for (int x = center.x - radius; x <= center.x + radius; ++x) {
                if (!inBounds(Pos(x, y), 0)) continue;

                damageEnemyAt(Pos(x, y), dmg);
            }
        }

        return true; // always cast
    }
    // в соответствии с требованиями:
    bool Field::applyAreaDamage2x2(Pos topLeft, int dmg)
    {
        for (int dy = 0; dy < 2; ++dy) {
            for (int dx = 0; dx < 2; ++dx) {
                const Pos p {topLeft.x + dx, topLeft.y + dy};

                if (!inBounds(p, 0)) continue;
                damageEnemyAt(p, dmg);
            }
        }

        return true; // always cast
    }

    bool Field::placeTrap(Pos p, int dmg) {
        if (!inBounds(p, 0)) return false;
        return cells_[p.y][p.x].placeTrap(dmg);
    }

    bool Field::triggerTraps() {
        for (Enemy& e: enemies_) {
            Pos enemyPos = e.getPosition();
            Cell& cell = cells_[enemyPos.y][enemyPos.x];

            if (e.isAlive() && cell.hasTrap()) {
                damageEnemyAt(enemyPos, cell.getTrapDamage());
                cell.removeTrap();
            }
        }
        return true;
    }

    // ---------------- Check pos for valid ---------------
    bool Field::inBounds(Pos p, int shift) const 
    {
        return (p.x >= shift && p.x < width_ - shift &&
                p.y >= shift && p.y < height_ - shift);
    }


    // =========== Getters ============

    // --------------- Get cell icon at pos ---------------
    char Field::iconAt(Pos p) const 
    {   
        if (inBounds(p, 0)) {
            return cells_[p.y][p.x].getIcon();
        }

        return 'N';
    }

    int Field::getNextSpawnTurn() const 
    {
        int nextSpawnTurn = cfg_.defaultSpawnInterval;
        for (const EnemyBuilding& eb: ebuildings_) {
            nextSpawnTurn = std::min(eb.turnsRemainSpawn(), nextSpawnTurn);
        }

        return nextSpawnTurn;
    }

    // -------------- Get count alive enemies -------------
    int Field::enemiesAlive() const 
    {
        int count = 0;
        for (const Enemy& e: enemies_) if (e.isAlive()) count++;
                
        return count;
    }
    

    // =========== Gameplay ===========

    // -------------- Create corpse of enemy -------------
    void Field::createCorpse(Character& enemy)
    {
        Pos corpsePos = enemy.getPosition();

        cells_[corpsePos.y][corpsePos.x].setWall();
        cells_[corpsePos.y][corpsePos.x].placeCorpse();
    }
    
    // --------------- Move enemies on field --------------
    void Field::moveEnemies()
    {
        std::uniform_int_distribution<int> chance(0, 99);

        std::vector<Pos> occupied;
        for (const Enemy& e: enemies_)
            if (e.isAlive()) occupied.push_back(e.getPosition());


        for (Enemy& e: enemies_) {
            if (!e.isAlive()) continue;

            Pos oldPos = e.getPosition();
            Pos newPos;

            if (chance(rng_) > cfg_.enemyRandMoveChance) {
                newPos = e.moveToPlayer(width_, height_, player_, cells_, occupied);
            } else {
                newPos = e.moveRandom(rng_, width_, height_, player_, cells_, occupied);
            }
            
            cells_[oldPos.y][oldPos.x].resetIcon();
            cells_[newPos.y][newPos.x].placeEnemy();

            // Update moved position
            for (Pos& pos: occupied)
                if (pos == oldPos) { pos = newPos; break; }
        }

        triggerTraps();
    }

    // --------------- Process player moving --------------
    bool Field::movePlayer(Direction dir)
    {
        Pos delta = Pos::toPos(dir);
        Pos oldPos = player_.getPosition();

        Pos newPos = Pos(oldPos.x + delta.x,
                         oldPos.y + delta.y);
        
        if (!inBounds(newPos, 0)) return false;
        if (!cells_[newPos.y][newPos.x].isWalkable()) return false;

        for (EnemyBuilding& eb: ebuildings_) {
            Pos ebuildingPos = eb.getPosition();

            if (newPos == ebuildingPos) {
                return false;
            }
        }

        for (Enemy& e: enemies_) {
            Pos enemyPos = e.getPosition();

            if (newPos == enemyPos &&
                e.isAlive())
            {   
                
                player_.attack(e, cfg_, rng_);
                
                if (e.isCorpse()) {
                    createCorpse(e);
                }

                return false;
            }
        }
        
        player_.move(dir, width_, height_);

        if (newPos != oldPos) {
            
            cells_[oldPos.y][oldPos.x].resetIcon();
            cells_[newPos.y][newPos.x].placePlayer();
            
            // Slime effect
            if (cells_[newPos.y][newPos.x].isSlime()) {
                nextTurn();
            }
        }

        return true;
    }

    // --------------- Process next turn ------------------
    void Field::nextTurn()
    {   
        for (Enemy& e: enemies_) {
            if (e.isCorpse()) e.incrementDeadCounter();
        }

        for (size_t i = 0; i < enemies_.size(); /*nothing*/ ) {
            const Enemy& e = enemies_[i];

            // after enemy death: spawn slime at this cell
            if (e.isCorpse() && e.getDeadTurnCounter() >= cfg_.enemyDespawnTurn) {
                const Pos deadEnemyPos = e.getPosition();
                cells_[deadEnemyPos.y][deadEnemyPos.x].setSlime();

                enemies_.erase(enemies_.begin() + i);
            } else {
                ++i;
            }
        }

        moveEnemies();

        for (EnemyBuilding& eb: ebuildings_) {
            eb.incrementTimer();

            if (eb.shouldSpawn()) {
                eb.resetTimer();

                int cnt = std::min(eb.getCountEnemies(), cfg_.maxEnemies - enemiesAlive());
                if (cnt < 0) cnt = 0;
                for (int i = 0; i < cnt; ++i) {
                    if (enemiesAlive() < cfg_.maxEnemies) {
                        spawnEnemyNearBuilding(eb);
                        player_.getHand().addRandomSpell(cfg_, rng_);
                    }
                }
            }

        }

        etower_.tick(*this);

        turnCounter_++;
    }

    // ----------------- Player attack --------------------
    bool Field::playerAttack()
    {
        if (!player_.isAlive()) return false;

        Pos playerPos = player_.getPosition();

        // Melee fight
        if (player_.getFightMode() == FightMode::Melee) {
            for (Enemy& e: enemies_) {
                if (!e.isAlive()) continue;
                Pos enemyPos = e.getPosition();

                int dx = abs(playerPos.x - enemyPos.x);
                int dy = abs(playerPos.y - enemyPos.y);

                if (dx <= 1 && dy <= 1) {
                    player_.attack(e, cfg_, rng_);

                    if (e.isCorpse()) {
                        createCorpse(e);
                    }
                    
                    nextTurn();
                    return true;  // Attack first near enemy (del for SPLASH effect)
                }
            }
        }
        else // Ranged fight (attack first near enemy)
        {
            Enemy* nearEnemy = nullptr;
            int nearDistanceSq = width_ * width_ + height_ * height_;
            for (Enemy& e: enemies_) {
                if (!e.isAlive()) continue;
                Pos enemyPos = e.getPosition();

                int dx = abs(playerPos.x - enemyPos.x);
                int dy = abs(playerPos.y - enemyPos.y);

                int distanceSq = dx * dx + dy * dy;
                if (distanceSq < nearDistanceSq) {
                    nearDistanceSq = distanceSq;
                    nearEnemy = &e;
                }
            }

            if (nearEnemy) {
                player_.attack(*nearEnemy, cfg_, rng_);

                if ((*nearEnemy).isCorpse()) {
                    createCorpse(*nearEnemy);
                }

                nextTurn();
                return true;
            }
        }

        nextTurn();
        return false;
    }

    // ---------------- Spawn new enemies -----------------
    bool Field::spawnEnemyNearBuilding(EnemyBuilding& ebuilding)
    {
        Pos ebuildingPos = ebuilding.getPosition();

        std::vector<Direction> candidates = {
            Direction::Up, Direction::Left,
            Direction::Down, Direction::Right
        };

        std::uniform_int_distribution<int> dirDist(0, (int)size(candidates) - 1);

        for (int i = 0; i < (int)candidates.size(); i++) {
            int randomDirIndex = dirDist(rng_);

            Pos candidatePos = ebuildingPos + Pos::toPos(candidates[randomDirIndex]);

            if (inBounds(candidatePos, 0) &&
                cells_[candidatePos.y][candidatePos.x].isWalkable() &&
                !hasEnemyAt(candidatePos) &&
                candidatePos != player_.getPosition())
            {
                enemies_.emplace_back(cfg_, candidatePos);
                cells_[candidatePos.y][candidatePos.x].placeEnemy();

                return true;
            }
        }

        return false;
    }

    // ------------- Switch player fight mode -------------
    void Field::switchPlayerFightMode() 
    { 
        player_.switchFightMode();
        nextTurn();
    }

    // -------------------- Save state --------------------
    void Field::save(std::ostream& out) const
    {
        // Size
        out << getW() << ' ' << getH() << '\n';

        // Turn
        out << getTurn() << '\n';

        // Player
        const Player& p = getPlayer();
        Pos playerPos = p.getPosition();

        out << playerPos.x << ' ' << playerPos.y   << '\n';
        out << p.getHP()   << ' ' << p.getDamage() << '\n';
        out << static_cast<int>(p.getFightMode())  << '\n';

        // PlayerHand
        p.getHand().save(out);

        // Enemies
        const auto& enemies = getEnemies();
        out << enemies.size() << '\n';
        for (const Enemy& e: enemies) {
            Pos enemyPos = e.getPosition();
            out << enemyPos.x   << ' ' << enemyPos.y    << ' '
                << e.getHP()    << ' ' << e.getDamage() << ' '
                << e.getDeadTurnCounter()
                << '\n';
        }

        // EBuildings
        const auto& ebuildings = getEBuildings();
        out << ebuildings.size() << '\n';
        for (const EnemyBuilding& eb: ebuildings) {
            Pos ebPos = eb.getPosition();
            out << ebPos.x       << ' ' << ebPos.y << ' '
                << eb.getTimer() << ' ' << eb.getCountEnemies()
                << '\n';
        }

        // ETower
        const ETower& t = getETower();
        Pos tPos        = t.getPosition();
        out << tPos.x << ' ' << tPos.y << ' '
            << t.getCooldownLeft()
            << '\n';

        // Cells
        for (int y = 0; y < getH(); ++y) {
            for (int x = 0; x < getW(); ++x) {
                const Cell& cell = getCell(x, y);
                out << static_cast<int>(cell.getType()) << ' '
                    << (cell.hasTrap() ? 1 : 0)         << ' '
                    << (cell.hasTrap() ? cell.getTrapDamage() : 0)
                    << '\n';
            }
        }

    }

    // -------------------- Load state --------------------
    void Field::load(std::istream& in)
    {
        int turn = 0;
        if (!(in >> turn)) {
            throw LoadError("Failed to read turn from save file");
        }
        turnCounter_ = turn;

        // Player
        int playerX = 0, playerY = 0;
        if (!(in >> playerX >> playerY)) {
            throw LoadError("Failed to read player position from save file");
        }

        int playerHP = 0, playerDamage = 0;
        if (!(in >> playerHP >> playerDamage)) {
            throw LoadError("Failed to read player stats from save file");
        }

        int fightMode = 0;
        if (!(in >> fightMode)) {
            throw LoadError("Failed to read player fight mode from save file");
        }

        Player& p = player_;
        p.setPosition(Pos(playerX, playerY));
        p.setHP(playerHP);
        p.setDamage(playerDamage);
        p.setFightModeById(fightMode);

        p.getHand().load(in, cfg_);


        // Enemies
        size_t enemyCount = 0;
        if (!(in >> enemyCount)) {
            throw LoadError("Failed to read enemy count from save file");
        }

        enemies_.clear();
        enemies_.reserve(enemyCount);
        for (size_t i = 0; i < enemyCount; ++i) {
            int enemyX, enemyY, enemyHP, enemyDamage, deadTurns;
            if (!(in >> enemyX >> enemyY >> enemyHP >> enemyDamage >> deadTurns)) {
                throw LoadError("Failed to read enemy stats from save file");
            }
            
            Enemy enemy(cfg_, Pos(enemyX, enemyY));
            enemy.setHP(enemyHP);
            enemy.setDamage(enemyDamage);
            enemy.setDeadTurnCounter(deadTurns);

            enemies_.push_back(std::move(enemy));
        }
        

        // EBuildings
        size_t ebCount = 0;
        if (!(in >> ebCount)) {
            throw LoadError("Failed to read enemy building count from save file");
        }

        ebuildings_.clear();
        ebuildings_.reserve(ebCount);
        for (size_t i = 0; i < ebCount; ++i) {
            int ebX, ebY, ebTimer, ebCountEnemies;
            if (!(in >> ebX >> ebY >> ebTimer >> ebCountEnemies)) {
                throw LoadError("Failed to read enemy building stats from save file");
            }

            EnemyBuilding eb(Pos(ebX, ebY), cfg_.defaultSpawnInterval, ebCountEnemies);
            eb.setTimer(ebTimer);

            ebuildings_.push_back(std::move(eb));
        }


        // ETower
        int tX = 0, tY = 0, tCD = 0;
        if (!(in >> tX >> tY >> tCD)) {
            throw LoadError("Failed to read tower stats from save file");
        }

        etower_.setPosition(Pos(tX, tY));
        etower_.setCooldownLeft(tCD);


        // Cells
        for (int y = 0; y < getH(); ++y) {
            for (int x = 0; x < getW(); ++x) {
                int type, hasTrap, trapDmg;
                if (!(in >> type >> hasTrap >> trapDmg)) {
                    throw LoadError("Failed to read cell stats from save file");
                }

                Cell& cell = cells_[y][x];
                cell.setTypeById(type);
                if (hasTrap) {
                    cell.placeTrap(trapDmg);
                } else {
                    cell.removeTrap();
                }

                // Place entities
                if (playerX == x && playerY == y) cell.placePlayer();
                for (const Enemy& e: enemies_) {
                    if (e.getPosition() == Pos(x, y)) {
                        if (e.isAlive()) {
                            cell.placeEnemy();
                        } else {
                            cell.placeCorpse();
                        }
                    }
                }
                for (const EnemyBuilding& eb: ebuildings_) {
                    if (eb.getPosition() == Pos(x, y)) cell.placeEBuilding();
                }
                if (tX == x && tY == y) cell.placeETower();

            }
        }
    }

    // === Testing ===
    #ifdef TESTING
    void Field::spawnEnemy(int x, int y)
    {
        enemies_.emplace_back(cfg_, Pos(x, y));
        cells_[y][x].placeEnemy();
    }

    void Field::placeWall(int x, int y)
    {
        cells_[y][x].setWall();
    }
    #endif
}