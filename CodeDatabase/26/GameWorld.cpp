#include "GameWorld.hpp"
#include "Field.hpp"
#include "Player.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iterator>
#include <limits>
#include <memory>
#include <numeric>
#include <queue>
#include <random>
#include <stdexcept>
#include <vector>
#include <sstream>

#include "SpellHand.hpp"
#include "SpellBase.hpp"
#include "EmpowerSpell.hpp"
#include "DirectDamageSpell.hpp"
#include "SpellLibrary.hpp"
#include "EventBus.hpp"
#include "GameExceptions.hpp"

namespace {
constexpr int kTowerDefaultRadius   = 4;
constexpr int kTowerDefaultDamage   = 3;
constexpr int kTowerDefaultCooldown = 2;
constexpr int kTowerDefaultHealth   = 12;

int spellAwardIntervalForLevel(int levelNumber) {
    if (levelNumber <= 1) {
        return 3;
    }
    if (levelNumber == 2) {
        return 5;
    }
    return 7;
}

std::string formatPosition(const Position& pos) {
    std::ostringstream oss;
    oss << "(" << pos.x << "," << pos.y << ")";
    return oss.str();
}
}

GameWorld::GameWorld(Field& field,
                     Player& player,
                     std::size_t handCapacity,
                     unsigned int randomSeed,
                     int initialTowerCount,
                     int levelNumber,
                     int totalLevels,
                     std::vector<LevelPlanEntry> levelPlan,
                     EventBus& events,
                     const ControlScheme* controls)
    : field_(field),
      player_(player),
      hand_(handCapacity),
      powerState_(),
      rng_(randomSeed ? randomSeed : static_cast<unsigned int>(std::random_device{}())),
      traps_(),
      allies_(),
      towers_(),
      levelPlan_(std::move(levelPlan)),
      levelNumber_(levelNumber),
      totalLevels_(totalLevels),
      spellAwardInterval_(spellAwardIntervalForLevel(levelNumber)),
      killsBaseline_(player.kills()),
      events_(events),
      controls_(controls)
{
    nextSpellAwardAtKills_ = killsBaseline_ + spellAwardInterval_;
    const int towerCount = std::max(0, initialTowerCount);
    if (field_.width() > 0 && field_.height() > 0) {
        std::uniform_int_distribution<int> xDist(0, field_.width() - 1);
        std::uniform_int_distribution<int> yDist(0, field_.height() - 1);
        int attempts = 0;
        while (static_cast<int>(towers_.size()) < towerCount && attempts < 100) {
            ++attempts;
            Position candidate{xDist(rng_), yDist(rng_)};
            if (!field_.isCellFreeForStructure(candidate)) {
                continue;
            }
            auto playerPos = field_.playerPosition();
            if (playerPos.has_value()) {
                const int dist = std::abs(playerPos->x - candidate.x) +
                                 std::abs(playerPos->y - candidate.y);
                if (dist < 4) {
                    continue;
                }
            }
            if (addTower(candidate,
                          kTowerDefaultRadius,
                          kTowerDefaultDamage,
                          kTowerDefaultCooldown,
                          kTowerDefaultHealth)) {
            }
        }
    }
    refreshAllyOverlay();
}

GameWorld::~GameWorld() {}

SpellHand& GameWorld::hand() noexcept {
    return hand_;
}

const SpellHand& GameWorld::hand() const noexcept {
    return hand_;
}

SpellPowerState& GameWorld::powerState() noexcept { return powerState_; }
const SpellPowerState& GameWorld::powerState() const noexcept { return powerState_; }

std::mt19937& GameWorld::rng() noexcept { return rng_; }

Player& GameWorld::playerEntity() noexcept { return player_; }
const Player& GameWorld::playerEntity() const noexcept { return player_; }

std::optional<Position> GameWorld::playerPosition() const noexcept {
    return field_.playerPosition();
}

bool GameWorld::dealDirectDamageAt(const Position& cell,
                                   int damage,
                                   int rangeLimit)
{
    if (damage <= 0 || rangeLimit < 0) {
        return false;
    }
    auto playerPosOpt = field_.playerPosition();
    if (!playerPosOpt.has_value()) {
        return false;
    }
    return dealDirectDamageFromOrigin(*playerPosOpt, cell, damage, rangeLimit);
}

bool GameWorld::dealDirectDamageFromOrigin(const Position& origin,
                                           const Position& cell,
                                           int damage,
                                           int rangeLimit)
{
    if (damage <= 0 || rangeLimit < 0) {
        return false;
    }
    if (!field_.contains(cell)) {
        return false;
    }
    const int dist = std::abs(origin.x - cell.x) +
                     std::abs(origin.y - cell.y);
    if (dist > rangeLimit) {
        return false;
    }
    bool hit = false;
    auto playerPosOpt = field_.playerPosition();
    if (playerPosOpt.has_value() && *playerPosOpt == cell) {
        if (dealDirectDamageToPlayer(damage)) {
            hit = true;
        }
    }
    if (field_.dealDamageToEnemyAt(cell, damage)) {
        hit = true;
        publishEvent("Direct damage " + std::to_string(damage) + " to enemy at " + formatPosition(cell),
                     GameEvent::Type::DamageApplied);
    }
    if (field_.dealDamageToNestAt(cell, damage)) {
        hit = true;
        publishEvent("Direct damage " + std::to_string(damage) + " to nest at " + formatPosition(cell),
                     GameEvent::Type::DamageApplied);
    }
    if (damageTowerAt(cell, damage)) {
        hit = true;
        publishEvent("Direct damage " + std::to_string(damage) + " to tower at " + formatPosition(cell),
                     GameEvent::Type::DamageApplied);
    }
    if (hit) {
        cleanupDestroyedTowers();
    }
    return hit;
}

bool GameWorld::dealAreaDamageAt(const Position& targetStart,
                                 int areaSize,
                                 int damage,
                                 int castRange)
{
    if (areaSize <= 0 || damage <= 0) {
        return false;
    }
    const int halfSpan = areaSize / 2;
    Position center{targetStart.x + halfSpan, targetStart.y + halfSpan};
    if (!isCellInRangeFromPlayer(center, castRange)) {
        return false;
    }

    bool anyEffect = false;
    for (int dy = 0; dy < areaSize; ++dy) {
        for (int dx = 0; dx < areaSize; ++dx) {
            Position cell{targetStart.x + dx, targetStart.y + dy};
            if (!field_.contains(cell)) {
                continue;
            }
            if (field_.dealDamageToEnemyAt(cell, damage)) {
                anyEffect = true;
                publishEvent("Area damage " + std::to_string(damage) + " to enemy at " + formatPosition(cell),
                             GameEvent::Type::DamageApplied);
            }
            if (field_.dealDamageToNestAt(cell, damage)) {
                anyEffect = true;
                publishEvent("Area damage " + std::to_string(damage) + " to nest at " + formatPosition(cell),
                             GameEvent::Type::DamageApplied);
            }
            if (damageTowerAt(cell, damage)) {
                anyEffect = true;
                publishEvent("Area damage " + std::to_string(damage) + " to tower at " + formatPosition(cell),
                             GameEvent::Type::DamageApplied);
            }
        }
    }
    if (anyEffect) {
        cleanupDestroyedTowers();
    }
    return true;
}

bool GameWorld::placeTrapAt(const Position& cell,
                            int damage,
                            int placementRange,
                            int trapLifetime)
{
    if (damage <= 0 || trapLifetime <= 0) {
        return false;
    }
    if (!isCellInRangeFromPlayer(cell, placementRange)) {
        return false;
    }
    if (!isCellFreeForTrap(cell)) {
        return false;
    }

    TrapInfo trap;
    trap.pos = cell;
    trap.damage = damage;
    trap.remainingLifetime = trapLifetime;
    trap.active = true;
    traps_.push_back(trap);
    refreshTrapOverlay();
    return true;
}

bool GameWorld::summonAlliesNearPlayer(int allyCount,
                                       int allyHealth,
                                       int allyDamage)
{
    if (allyCount <= 0 || allyHealth <= 0 || allyDamage < 0) {
        return false;
    }

    auto playerPosOpt = field_.playerPosition();
    if (!playerPosOpt.has_value()) {
        return false;
    }

    static const Position deltas[] = {
        { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1},
        { 1,  1}, { 1, -1}, {-1,  1}, {-1, -1}
    };

    int spawned = 0;
    for (const auto& delta : deltas) {
        if (spawned >= allyCount) {
            break;
        }
        Position target{playerPosOpt->x + delta.x, playerPosOpt->y + delta.y};
        if (!isCellOccupiableByAlly(target)) {
            continue;
        }
        Ally ally(target, allyHealth, allyDamage);
        if (field_.isCellSlow(target)) {
            ally.applySlowForNextTurn();
        }
        allies_.push_back(ally);
        publishEvent("Ally summoned at " + formatPosition(target) +
                         " hp " + std::to_string(allyHealth) +
                         " dmg " + std::to_string(allyDamage),
                     GameEvent::Type::AllySummoned);
        ++spawned;
    }

    refreshAllyOverlay();
    return spawned > 0;
}


bool GameWorld::isPlayerInRange(const Position& towerPos,
                                int radius) const noexcept
{
    if (radius < 0) {
        return false;
    }
    auto playerPosOpt = field_.playerPosition();
    if (!playerPosOpt.has_value()) {
        return false;
    }
    const Position& playerPos = *playerPosOpt;
    const int dist = std::abs(playerPos.x - towerPos.x) +
                     std::abs(playerPos.y - towerPos.y);
    return dist <= radius;
}

bool GameWorld::dealDirectDamageToPlayer(int damage) {
    if (damage < 0) {
        return false;
    }
    if (field_.applyDamageToPlayer(damage)) {
        publishEvent("Player received " + std::to_string(damage) + " damage",
                     GameEvent::Type::DamageApplied);
        return true;
    }
    return false;
}

void GameWorld::advanceEnemyTowers() {
    cleanupDestroyedTowers();
    for (auto& tower : towers_) {
        tower.tickAndAttack(*this);
    }
    cleanupDestroyedTowers();
}

void GameWorld::advanceAllies() {
    static const Position directions[] = {
        { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1}
    };

    auto distanceMap = buildDistanceMapToEnemies();
    const bool canNavigate = !distanceMap.empty();

    for (auto& ally : allies_) {
        if (!ally.isAlive()) {
            continue;
        }

        const Position current = ally.position();
        bool attacked = false;
        for (const auto& dir : directions) {
            Position target{current.x + dir.x, current.y + dir.y};
            if (!field_.contains(target)) {
                continue;
            }
            if (field_.dealDamageToEnemyAt(target, ally.damage())) {
                attacked = true;
                publishEvent("Ally dealt " + std::to_string(ally.damage()) + " to enemy at " + formatPosition(target),
                             GameEvent::Type::DamageApplied);
                break;
            }
        }
        if (attacked) {
            continue;
        }

        if (!ally.canMoveThisTurn()) {
            continue;
        }
        if (!canNavigate) {
            continue;
        }

        tryMoveAllyTowards(ally, distanceMap);
    }

    allies_.erase(
        std::remove_if(allies_.begin(), allies_.end(), [](const Ally& ally) {
            return !ally.isAlive();
        }),
        allies_.end()
    );
    refreshAllyOverlay();
}

void GameWorld::advanceWorldTurn() {
    auto allyRequests = field_.drainAllyDamageRequests();
    if (!allyRequests.empty()) {
        for (const auto& req : allyRequests) {
            const Position pos = req.first;
            const int dmg = req.second;
            for (auto& ally : allies_) {
                if (ally.isAlive() && ally.position() == pos) {
                    ally.takeDamage(dmg);
                    break;
                }
            }
        }
        allies_.erase(
            std::remove_if(allies_.begin(), allies_.end(), [](const Ally& ally) {
                return !ally.isAlive();
            }),
            allies_.end()
        );
        refreshAllyOverlay();
    }
    resolveTraps();
    ageTraps();
    advanceEnemyTowers();
    advanceAllies();
    cleanupDestroyedTowers();
}

bool GameWorld::isCellInRangeFromPlayer(const Position& cell,
                                        int rangeLimit) const noexcept
{
    if (rangeLimit < 0) {
        return false;
    }
    if (!field_.contains(cell)) {
        return false;
    }
    auto playerPosOpt = field_.playerPosition();
    if (!playerPosOpt.has_value()) {
        return false;
    }
    const Position& playerPos = *playerPosOpt;
    const int dist = std::abs(playerPos.x - cell.x) +
                     std::abs(playerPos.y - cell.y);
    return dist <= rangeLimit;
}

bool GameWorld::isCellFreeForTrap(const Position& cell) const noexcept
{
    if (!field_.contains(cell)) {
        return false;
    }
    if (!field_.isCellPassable(cell)) {
        return false;
    }
    if (field_.hasEnemyAt(cell) || field_.hasNestAt(cell) || field_.hasTowerAt(cell)) {
        return false;
    }
    auto playerPosOpt = field_.playerPosition();
    if (playerPosOpt.has_value() && *playerPosOpt == cell) {
        return false;
    }
    for (const auto& trap : traps_) {
        if (trap.active && trap.pos == cell) {
            return false;
        }
    }
    return true;
}

void GameWorld::giveRandomStartingSpell() {
    auto firstSpell = generateRandomSpell();
    if (tryAddSpellToHand(std::move(firstSpell))) {
        return;
    }
    if (!hand_.hasSpellHotkey(5)) {
        tryAddSpellToHand(std::unique_ptr<SpellBase>(new EmpowerSpell(1)));
    }
}

void GameWorld::tryAwardSpell(Player& player) {
    if (player.kills() < nextSpellAwardAtKills_) {
        return;
    }
    bool granted = false;
    for (int attempt = 0; attempt < 5; ++attempt) {
        auto reward = generateRandomSpell();
        if (!reward) {
            break;
        }
        if (tryAddSpellToHand(std::move(reward))) {
            granted = true;
            break;
        }
    }
    nextSpellAwardAtKills_ += spellAwardInterval_;
    if (!granted) {
        publishEvent("Spell reward skipped (hand full)", GameEvent::Type::SpellGranted);
        return;
    }
}

std::unique_ptr<SpellBase> GameWorld::generateRandomSpell() {
    if (hand_.size() >= hand_.capacity()) {
        return nullptr;
    }

    std::vector<int> hotkeys = SpellLibrary::availableHotkeys();
    if (hotkeys.empty()) {
        return nullptr;
    }
    std::uniform_int_distribution<std::size_t> pick(0, hotkeys.size() - 1);
    int hotkey = hotkeys[pick(rng_)];
    return SpellLibrary::createSpellByHotkey(hotkey);
}

int GameWorld::levelNumber() const noexcept {
    return levelNumber_;
}

int GameWorld::totalLevels() const noexcept {
    return totalLevels_;
}

const std::vector<GameWorld::LevelPlanEntry>& GameWorld::levelPlan() const noexcept {
    return levelPlan_;
}

const ControlScheme* GameWorld::controlScheme() const noexcept {
    return controls_;
}
bool GameWorld::addTower(const Position& position,
                         int attackRadius,
                         int damage,
                         int cooldownPeriod,
                         int health)
{
    if (!field_.registerTowerAt(position)) {
        return false;
    }
    try {
        towers_.emplace_back(position, attackRadius, damage, cooldownPeriod, health);
        publishEvent("Tower spawned at " + formatPosition(position),
                     GameEvent::Type::SystemMessage);
        return true;
    } catch (...) {
        field_.removeTowerAt(position);
        throw;
    }
}

bool GameWorld::damageTowerAt(const Position& cell, int damage) {
    if (damage <= 0) {
        return false;
    }
    bool hit = false;
    for (auto& tower : towers_) {
        if (tower.position() == cell && tower.isAlive()) {
            tower.takeDamage(damage);
            hit = true;
            if (!tower.isAlive()) {
                publishEvent("Tower destroyed at " + formatPosition(cell),
                             GameEvent::Type::StructureDestroyed);
            } else {
                publishEvent("Tower at " + formatPosition(cell) + " took " + std::to_string(damage) + " damage",
                             GameEvent::Type::DamageApplied);
            }
        }
    }
    return hit;
}

void GameWorld::cleanupDestroyedTowers() {
    auto it = std::remove_if(towers_.begin(), towers_.end(), [&](const EnemyTower& tower) {
        if (!tower.isAlive()) {
            field_.removeTowerAt(tower.position());
            return true;
        }
        return false;
    });
    towers_.erase(it, towers_.end());
}

void GameWorld::refreshAllyOverlay() {
    std::vector<Position> alive;
    alive.reserve(allies_.size());
    for (const auto& ally : allies_) {
        if (ally.isAlive()) {
            alive.push_back(ally.position());
        }
    }
    field_.setAllyOverlay(alive);
}

void GameWorld::refreshTrapOverlay() {
    std::vector<Position> t;
    t.reserve(traps_.size());
    for (const auto& trap : traps_) {
        if (trap.active) t.push_back(trap.pos);
    }
    field_.setTrapOverlay(t);
}

void GameWorld::resolveTraps() {
    for (auto& trap : traps_) {
        if (!trap.active) {
            continue;
        }
        if (field_.hasEnemyAt(trap.pos)) {
            field_.dealDamageToEnemyAt(trap.pos, trap.damage);
            publishEvent("Trap dealt " + std::to_string(trap.damage) + " to enemy at " + formatPosition(trap.pos),
                         GameEvent::Type::DamageApplied);
            trap.active = false;
            refreshTrapOverlay();
        }
    }
}

void GameWorld::ageTraps() {
    for (auto& trap : traps_) {
        if (!trap.active) {
            continue;
        }
        if (trap.remainingLifetime > 0) {
            trap.remainingLifetime -= 1;
            if (trap.remainingLifetime <= 0) {
                trap.active = false;
            }
        }
    }
    traps_.erase(
        std::remove_if(traps_.begin(), traps_.end(), [](const TrapInfo& info) {
            return !info.active;
        }),
        traps_.end()
    );
    refreshTrapOverlay();
}

bool GameWorld::isAllyAt(const Position& cell) const noexcept {
    for (const auto& ally : allies_) {
        if (ally.isAlive() && ally.position() == cell) {
            return true;
        }
    }
    return false;
}

void GameWorld::save(std::ostream& out) const {
    out << "GAMEWORLD_V1\n";
    out << nextSpellAwardAtKills_ << '\n';
    out << powerState_.pendingStacks() << '\n';
    out << spellAwardInterval_ << '\n';
    out << hand_.capacity() << ' ' << hand_.size() << '\n';
    out << levelNumber_ << ' ' << totalLevels_ << '\n';
    for (const auto& card : hand_.cards()) {
        if (card) {
            out << card->hotkey() << ' ';
        }
    }
    out << '\n';
    std::ostringstream rngState;
    rngState << rng_;
    out << rngState.str() << '\n';

    out << traps_.size() << '\n';
    for (const auto& trap : traps_) {
        out << trap.pos.x << ' ' << trap.pos.y << ' '
            << trap.damage << ' ' << trap.remainingLifetime << ' '
            << static_cast<int>(trap.active) << '\n';
    }

    out << allies_.size() << '\n';
    for (const auto& ally : allies_) {
        out << ally.position().x << ' ' << ally.position().y << ' '
            << ally.health() << ' ' << ally.damage() << ' '
            << static_cast<int>(ally.skipMovePending()) << '\n';
    }

    out << towers_.size() << '\n';
    for (const auto& tower : towers_) {
        out << tower.position().x << ' ' << tower.position().y << ' '
            << tower.attackRadius() << ' ' << tower.damage() << ' '
            << tower.cooldownPeriod() << ' ' << tower.health() << ' '
            << tower.cooldownRemaining() << '\n';
    }
}

void GameWorld::load(std::istream& in) {
    const char* header = "GAMEWORLD_V1";
    std::string token;
    if (!(in >> token) || token != header) {
        throw InvalidDataException("GameWorld header mismatch");
    }
    if (!(in >> nextSpellAwardAtKills_)) {
        throw InvalidDataException("Failed to read nextSpellAwardAtKills");
    }
    int stacks = 0;
    if (!(in >> stacks)) {
        throw InvalidDataException("Failed to read spell stacks");
    }
    powerState_.setStacks(stacks);
    if (!(in >> spellAwardInterval_)) {
        throw InvalidDataException("Failed to read spell award interval");
    }
    if (spellAwardInterval_ <= 0) {
        spellAwardInterval_ = spellAwardIntervalForLevel(levelNumber_);
    }

    std::size_t capacityStored = 0;
    std::size_t handSize = 0;
    if (!(in >> capacityStored >> handSize)) {
        throw InvalidDataException("Failed to read hand metadata");
    }
    if (!(in >> levelNumber_ >> totalLevels_)) {
        throw InvalidDataException("Failed to read level metadata");
    }
    hand_ = SpellHand(capacityStored);
    hand_.clear();
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (handSize > 0) {
        std::string line;
        if (!std::getline(in, line)) {
            throw InvalidDataException("Failed to read hand entries");
        }
        std::istringstream hotkeys(line);
        for (std::size_t i = 0; i < handSize; ++i) {
            int hotkey = 0;
            if (!(hotkeys >> hotkey)) {
                throw InvalidDataException("Corrupted hand data");
            }
            auto spell = SpellLibrary::createSpellByHotkey(hotkey);
            if (!spell) {
                throw InvalidDataException("Unknown spell hotkey in save");
            }
            if (!hand_.addSpell(std::move(spell))) {
                throw InvalidDataException("Failed to restore spell hand");
            }
        }
    }
    else {
        std::string dummy;
        std::getline(in, dummy);
    }

    std::string rngLine;
    if (!std::getline(in, rngLine)) {
        throw InvalidDataException("Failed to read RNG state");
    }
    if (!rngLine.empty()) {
        std::istringstream rngState(rngLine);
        rngState >> rng_;
    }

    std::size_t trapCount = 0;
    if (!(in >> trapCount)) {
        throw InvalidDataException("Failed to read trap count");
    }
    traps_.clear();
    for (std::size_t i = 0; i < trapCount; ++i) {
        TrapInfo info;
        int active = 0;
        if (!(in >> info.pos.x >> info.pos.y >> info.damage
              >> info.remainingLifetime >> active)) {
            throw InvalidDataException("Failed to read trap entry");
        }
        info.active = (active != 0);
        traps_.push_back(info);
    }

    std::size_t allyCount = 0;
    if (!(in >> allyCount)) {
        throw InvalidDataException("Failed to read ally count");
    }
    allies_.clear();
    for (std::size_t i = 0; i < allyCount; ++i) {
        Position pos{};
        int hp = 0;
        int dmg = 0;
        int skip = 0;
        if (!(in >> pos.x >> pos.y >> hp >> dmg >> skip)) {
            throw InvalidDataException("Failed to read ally entry");
        }
        Ally ally(pos, std::max(1, hp), dmg);
        ally.setHealth(hp);
        ally.setSkipNextMove(skip != 0);
        allies_.push_back(ally);
    }

    std::size_t towerCount = 0;
    if (!(in >> towerCount)) {
        throw InvalidDataException("Failed to read tower count");
    }
    towers_.clear();
    for (std::size_t i = 0; i < towerCount; ++i) {
        Position pos{};
        int radius = 0;
        int damage = 0;
        int cooldownPeriod = 0;
        int health = 0;
        int cooldownRemaining = 0;
        if (!(in >> pos.x >> pos.y >> radius >> damage >> cooldownPeriod >> health >> cooldownRemaining)) {
            throw InvalidDataException("Failed to read tower entry");
        }
        // Ensure field keeps only one instance of this tower position
        field_.removeTowerAt(pos);
        field_.registerTowerAt(pos);
        EnemyTower tower(pos, radius, damage, cooldownPeriod, std::max(1, health));
        tower.setHealth(health);
        tower.setCooldownRemaining(cooldownRemaining);
        towers_.push_back(std::move(tower));
    }

    refreshTrapOverlay();
    refreshAllyOverlay();
}

bool GameWorld::tryMoveAllyTowards(Ally& ally, const std::vector<std::vector<int>>& distanceMap) {
    if (distanceMap.empty()) {
        return false;
    }
    if (!field_.contains(ally.position())) {
        return false;
    }

    Position next = bestStepTowards(ally.position(), distanceMap);
    if (next == ally.position()) {
        return false;
    }
    if (!isCellOccupiableByAlly(next)) {
        return false;
    }

    ally.setPosition(next);
    if (field_.isCellSlow(next)) {
        ally.applySlowForNextTurn();
    }
    return true;
}

Position GameWorld::bestStepTowards(const Position& from,
                                    const std::vector<std::vector<int>>& distanceMap) const {
    if (distanceMap.empty() || !field_.contains(from)) {
        return from;
    }

    const int currentValue = distanceMap[from.y][from.x];
    int bestValue = currentValue;
    Position best = from;

    static const Position offsets[] = {
        { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1}
    };

    for (const auto& offset : offsets) {
        Position candidate{from.x + offset.x, from.y + offset.y};
        if (!field_.contains(candidate)) {
            continue;
        }
        if (!field_.isCellPassable(candidate)) {
            continue;
        }
        int value = distanceMap[candidate.y][candidate.x];
        if (value == std::numeric_limits<int>::max()) {
            continue;
        }
        if (value < bestValue) {
            bestValue = value;
            best = candidate;
        }
    }

    return best;
}

bool GameWorld::isCellOccupiableByAlly(const Position& cell) const noexcept {
    if (!field_.contains(cell)) {
        return false;
    }
    if (!field_.isCellPassable(cell)) {
        return false;
    }
    if (field_.hasEnemyAt(cell) || field_.hasNestAt(cell) || field_.hasTowerAt(cell)) {
        return false;
    }
    auto playerPosOpt = field_.playerPosition();
    if (playerPosOpt.has_value() && *playerPosOpt == cell) {
        return false;
    }
    return !isAllyAt(cell);
}

std::vector<Position> GameWorld::enemyPositions() const {
    return field_.enemyPositions();
}

std::vector<std::vector<int>> GameWorld::buildDistanceMapToEnemies() const {
    const int width = field_.width();
    const int height = field_.height();
    if (width <= 0 || height <= 0) {
        return {};
    }

    auto enemies = enemyPositions();
    if (enemies.empty()) {
        return {};
    }

    const int INF = std::numeric_limits<int>::max();
    std::vector<std::vector<int>> dist(
        height, std::vector<int>(width, INF)
    );

    std::queue<Position> q;
    for (const auto& enemyPos : enemies) {
        if (!field_.contains(enemyPos)) {
            continue;
        }
        if (dist[enemyPos.y][enemyPos.x] == 0) {
            continue;
        }
        dist[enemyPos.y][enemyPos.x] = 0;
        q.push(enemyPos);
    }
    if (q.empty()) {
        return {};
    }

    static const Position offsets[] = {
        { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1}
    };
    auto playerPos = field_.playerPosition();

    while (!q.empty()) {
        Position cur = q.front();
        q.pop();

        const int baseDistance = dist[cur.y][cur.x];
        for (const auto& offset : offsets) {
            Position next{cur.x + offset.x, cur.y + offset.y};
            if (!field_.contains(next)) {
                continue;
            }
            if (!field_.isCellPassable(next)) {
                continue;
            }
            if (field_.hasTowerAt(next) || field_.hasNestAt(next)) {
                continue;
            }
            if (playerPos.has_value() && *playerPos == next) {
                continue;
            }
            if (dist[next.y][next.x] <= baseDistance + 1) {
                continue;
            }

            dist[next.y][next.x] = baseDistance + 1;
            q.push(next);
        }
    }

    return dist;
}

bool GameWorld::tryAddSpellToHand(std::unique_ptr<SpellBase> spell) {
    if (!spell) {
        return false;
    }
    if (hand_.size() >= hand_.capacity()) {
        return false;
    }
    const std::string spellName = spell->name();
    const int hotkey = spell->hotkey();
    if (hand_.addSpell(std::move(spell))) {
        publishEvent("Spell added [" + std::to_string(hotkey) + "] " + spellName,
                     GameEvent::Type::SpellGranted);
        return true;
    }
    return false;
}

void GameWorld::publishEvent(const std::string& message, GameEvent::Type type) const {
    events_.publish(GameEvent::makeMessage(type, message));
}







