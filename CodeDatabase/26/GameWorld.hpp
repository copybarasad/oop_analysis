#pragma once
#include <iosfwd>
#include <vector>
#include <random>
#include <cstddef>
#include <memory>
#include <optional>
#include "Position.hpp"
#include "SpellHand.hpp"
#include "SpellPowerState.hpp"
#include "SpellBase.hpp"
#include "Ally.hpp"
#include "EnemyTower.hpp"
#include "GameEvent.hpp"

class Field;
class Player;
class EventBus;
class ControlScheme;

class GameWorld {
public:
	struct TrapInfo {
		Position pos;
		int damage;
		int remainingLifetime;
		bool active;
	};

	struct LevelPlanEntry {
		int levelNumber;
		int width;
		int height;
		int enemyHp;
		int enemyDamage;
		std::optional<int> playerHp;
		std::optional<int> playerMelee;
		std::optional<int> playerRanged;
	};

	GameWorld(Field& field,
	          Player& player,
	          std::size_t handCapacity,
	          unsigned int randomSeed,
	          int initialTowerCount,
	          int levelNumber,
	          int totalLevels,
	          std::vector<LevelPlanEntry> levelPlan,
	          EventBus& events,
	          const ControlScheme* controls = nullptr);

	~GameWorld();

	SpellHand& hand() noexcept;
	const SpellHand& hand() const noexcept;

	SpellPowerState& powerState() noexcept;
	const SpellPowerState& powerState() const noexcept;

	std::mt19937& rng() noexcept;
	Player& playerEntity() noexcept;
	const Player& playerEntity() const noexcept;
	std::optional<Position> playerPosition() const noexcept;

	bool dealDirectDamageAt(const Position& cell,
	                        int damage,
	                        int rangeLimit);
	bool dealDirectDamageFromOrigin(const Position& origin,
	                                const Position& cell,
	                                int damage,
	                                int rangeLimit);

	bool dealAreaDamageAt(const Position& targetStart,
	                      int areaSize,
	                      int damage,
	                      int castRange);

	bool placeTrapAt(const Position& cell,
	                 int damage,
	                 int placementRange,
	                 int trapLifetime);

	bool summonAlliesNearPlayer(int allyCount,
	                            int allyHealth,
	                            int allyDamage);

	bool isPlayerInRange(const Position& towerPos,
	                     int radius) const noexcept;

	bool dealDirectDamageToPlayer(int damage);
	void advanceEnemyTowers();
	void advanceAllies();
	void advanceWorldTurn();
	void giveRandomStartingSpell();
	void tryAwardSpell(Player& player);
	int levelNumber() const noexcept;
	int totalLevels() const noexcept;
	const std::vector<LevelPlanEntry>& levelPlan() const noexcept;
	const ControlScheme* controlScheme() const noexcept;

	bool addTower(const Position& position,
	              int attackRadius,
	              int damage,
	              int cooldownPeriod,
	              int health);
	void save(std::ostream& out) const;
	void load(std::istream& in);

private:
	Field& field_;
	Player& player_;

	SpellHand hand_;
	SpellPowerState powerState_;

	std::mt19937 rng_;
	int nextSpellAwardAtKills_;

	std::vector<TrapInfo> traps_;
	std::vector<Ally> allies_;
	std::vector<EnemyTower> towers_;
	std::vector<LevelPlanEntry> levelPlan_;
	int levelNumber_;
	int totalLevels_;
	int spellAwardInterval_;
	int killsBaseline_;
	EventBus& events_;
	const ControlScheme* controls_;

	bool isCellInRangeFromPlayer(const Position& cell,
	                             int rangeLimit) const noexcept;

	bool isCellFreeForTrap(const Position& cell) const noexcept;

	std::unique_ptr<SpellBase> generateRandomSpell();
	bool damageTowerAt(const Position& cell, int damage);
	void cleanupDestroyedTowers();
	void resolveTraps();
	void ageTraps();
	void refreshAllyOverlay();
	void refreshTrapOverlay();
	bool isAllyAt(const Position& cell) const noexcept;
	bool tryMoveAllyTowards(Ally& ally, const std::vector<std::vector<int>>& distanceMap);
	Position bestStepTowards(const Position& from, const std::vector<std::vector<int>>& distanceMap) const;
	bool isCellOccupiableByAlly(const Position& cell) const noexcept;
	std::vector<Position> enemyPositions() const;
	std::vector<std::vector<int>> buildDistanceMapToEnemies() const;
	bool tryAddSpellToHand(std::unique_ptr<SpellBase> spell);
	void publishEvent(const std::string& message, GameEvent::Type type = GameEvent::Type::SystemMessage) const;
};

