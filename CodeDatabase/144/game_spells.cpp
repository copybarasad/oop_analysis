#include "game.h"
#include "directdamagespell.h"
#include "areadamagespell.h"
#include "trapspell.h"
#include "summonspell.h"
#include "enhancementspell.h"
#include "spell_context.h"
#include "ally.h"
#include "trap.h"
#include "damageable.h"
#include "game_enemy_action_context.h"
#include "game_logging.h"
#include "target.h"
#include "enemy.h"
#include "enemybuilding.h"
#include "enemytower.h"
#include "control_config.h"
#include <iostream>
#include <random>
#include <cctype>
#include <limits>
#include <cmath>
#include <set>
#include <sstream>

class GameSpellContext : public SpellContext {
public:
	explicit GameSpellContext(Game& game) : game(game) {}

	Player& getPlayer() override { return game.getPlayer(); }
	Field& getField() override { return game.getField(); }
	EnemyManager& getEnemyManager() override { return game.getEnemyManager(); }
	BuildingManager& getBuildingManager() override { return game.getBuildingManager(); }

	void addTrap(const Trap& trap) override { game.addTrap(trap); }
	void addAlly(const Ally& ally) override { game.addAlly(ally); }
	bool damageEntitiesAtPosition(int x, int y, int damageAmount) override {
		return game.damageEntitiesAtPosition(x, y, damageAmount);
	}

	bool isCellOccupiedByEnemy(int x, int y) const override {
		return game.isCellOccupiedByEnemy(x, y);
	}

	bool isCellOccupiedByAlly(int x, int y) const override {
		return game.isCellOccupiedByAlly(x, y);
	}

	void handleTargetDefeat(Damageable& target) override {
		GameEnemyActionContext defeatContext(game);
		target.onDefeated(defeatContext);
	}

	std::vector<SpellTarget> getTargetsInRange(int range) override {
		std::vector<SpellTarget> result;
		Player& player = game.getPlayer();
		int playerX = player.getX();
		int playerY = player.getY();
		
		for (auto& enemy : game.getEnemyManager().getEnemies()) {
			if (!enemy.isAlive()) {
				continue;
			}
			
			int distanceX = std::abs(enemy.getPositionX() - playerX);
			int distanceY = std::abs(enemy.getPositionY() - playerY);
			
			if (distanceX <= range && distanceY <= range) {
				SpellTarget target;
				target.name = "Enemy";
				target.targetObject = &enemy;
				target.positionX = enemy.getPositionX();
				target.positionY = enemy.getPositionY();
				result.push_back(target);
			}
		}
		
		for (auto& building : game.getBuildingManager().getBuildings()) {
			if (!building.isAlive()) {
				continue;
			}
			
			int distanceX = std::abs(building.getPositionX() - playerX);
			int distanceY = std::abs(building.getPositionY() - playerY);
			
			if (distanceX <= range && distanceY <= range) {
				SpellTarget target;
				target.name = "Building";
				target.targetObject = &building;
				target.positionX = building.getPositionX();
				target.positionY = building.getPositionY();
				result.push_back(target);
			}
		}
		
		for (auto& tower : game.getBuildingManager().getTowers()) {
			if (!tower.isAlive()) {
				continue;
			}
			
			int distanceX = std::abs(tower.getPositionX() - playerX);
			int distanceY = std::abs(tower.getPositionY() - playerY);
			
			if (distanceX <= range && distanceY <= range) {
				SpellTarget target;
				target.name = "Tower";
				target.targetObject = &tower;
				target.positionX = tower.getPositionX();
				target.positionY = tower.getPositionY();
				result.push_back(target);
			}
		}
		
		return result;
	}

	void displayTargets(const std::vector<SpellTarget>& targets) override {
		std::cout << "Available targets in range:\n";
		for (size_t i = 0; i < targets.size(); ++i) {
			const SpellTarget& target = targets[i];
			Damageable* obj = target.targetObject;
			std::cout << i << ". " << target.name 
			          << " at (" << target.positionX << ", " << target.positionY << ")"
			          << " HP: " << (obj ? obj->getHealth() : 0) << "\n";
		}
	}

	bool trySelectTarget(const std::vector<SpellTarget>& targets, int& selectedIndex) override {
		if (targets.empty()) {
			return false;
		}
		
		std::cout << "Select target (0-" << targets.size() - 1 << "): ";
		int choice;
		std::cin >> choice;
		
		if (choice < 0 || choice >= static_cast<int>(targets.size())) {
			return false;
		}
		
		selectedIndex = choice;
		return true;
	}

	bool requestDirection(const std::string& prompt, int& directionX, int& directionY) override {
		auto keys = getDirectionKeys();
		std::cout << prompt << " (" << keys.up << "/" << keys.down << "/" << keys.left << "/" << keys.right << "): ";
		char direction;
		std::cin >> direction;
		
		directionX = 0;
		directionY = 0;
		
		if (applyDirectionFromChar(direction, directionX, directionY)) {
			return true;
		}
		
		std::cout << "Invalid direction!\n";
		return false;
	}

	AreaSelection selectArea(int areaSize) override {
		AreaSelection selection{};
		Player& player = game.getPlayer();
		Field& field = game.getField();
		
		auto keys = getDirectionKeys();
		std::cout << "Enter direction for area damage (" << keys.up << "/" << keys.down << "/" << keys.left << "/" << keys.right << "): ";
		char direction;
		std::cin >> direction;

		int directionX = 0;
		int directionY = 0;
		int perpendicularX = 0;
		int perpendicularY = 0;
		
		char normalized = static_cast<char>(std::tolower(static_cast<unsigned char>(direction)));
		if (normalized == std::tolower(keys.up)) {
			directionY = -1;
			perpendicularX = 1;
		} else if (normalized == std::tolower(keys.down)) {
			directionY = 1;
			perpendicularX = 1;
		} else if (normalized == std::tolower(keys.left)) {
			directionX = -1;
			perpendicularY = 1;
		} else if (normalized == std::tolower(keys.right)) {
			directionX = 1;
			perpendicularY = 1;
		} else {
			std::cout << "Invalid direction!\n";
			return selection;
		}
		
		int centerOffset = (areaSize - 1) / 2;
		int offset = 0;
		int maxOffset = areaSize - 1;
		
		while (true) {
			int baseX = player.getX() + directionX;
			int baseY = player.getY() + directionY;
			int targetX = baseX - perpendicularX * centerOffset + perpendicularX * offset;
			int targetY = baseY - perpendicularY * centerOffset + perpendicularY * offset;

			if (directionX < 0) {
				targetX -= (areaSize - 1);
			}
			if (directionY < 0) {
				targetY -= (areaSize - 1);
			}
			
			std::cout << "\nArea Damage will affect " << areaSize << "x" << areaSize << " area:\n";
			if (directionX != 0) {
				std::cout << "Offset: " << offset << " (use " << keys.up << "/" << keys.down 
				          << " to move up/down, Enter to confirm, n to cancel)\n";
			} else {
				std::cout << "Offset: " << offset << " (use " << keys.left << "/" << keys.right 
				          << " to move left/right, Enter to confirm, n to cancel)\n";
			}
			displayAreaPreview(field, player, targetX, targetY, areaSize);
			
			std::cout << "\nMove area: ";
			char move;
			std::cin.ignore();
			move = std::cin.get();
			
			if (move == '\n' || move == '\r') {
				selection.cancelled = false;
				selection.targetX = targetX;
				selection.targetY = targetY;
				return selection;
			}
			
			if (move == 'n' || move == 'N') {
				return selection;
			}
			
			char normalizedMove = static_cast<char>(std::tolower(static_cast<unsigned char>(move)));
			
			if (directionX != 0) {
				if (normalizedMove == std::tolower(keys.up)) {
					if (offset > -maxOffset) {
						offset--;
					}
				} else if (normalizedMove == std::tolower(keys.down)) {
					if (offset < maxOffset) {
						offset++;
					}
				}
			} else {
				if (normalizedMove == std::tolower(keys.left)) {
					if (offset > -maxOffset) {
						offset--;
					}
				} else if (normalizedMove == std::tolower(keys.right)) {
					if (offset < maxOffset) {
						offset++;
					}
				}
			}
		}
	}

private:
	struct DirectionKeys {
		char up;
		char down;
		char left;
		char right;
	};

	DirectionKeys getDirectionKeys() const {
		const ControlConfig& cfg = game.getControls();
		auto get = [&cfg](CommandName name, char fallback) {
			auto key = cfg.keyFor(name);
			return key ? *key : fallback;
		};
		return {
			get(CommandName::MoveUp, 'w'),
			get(CommandName::MoveDown, 's'),
			get(CommandName::MoveLeft, 'a'),
			get(CommandName::MoveRight, 'd')
		};
	}

	bool applyDirectionFromChar(char input, int& dx, int& dy) const {
		auto keys = getDirectionKeys();
		char c = static_cast<char>(std::tolower(static_cast<unsigned char>(input)));
		dx = 0;
		dy = 0;
		if (c == std::tolower(keys.up)) {
			dy = -1;
			return true;
		}
		if (c == std::tolower(keys.down)) {
			dy = 1;
			return true;
		}
		if (c == std::tolower(keys.left)) {
			dx = -1;
			return true;
		}
		if (c == std::tolower(keys.right)) {
			dx = 1;
			return true;
		}
		return false;
	}

	void displayAreaPreview(Field& field, Player& player, int targetX, int targetY, int areaSize) {
		std::set<std::pair<int, int>> areaCells;
		for (int deltaX = 0; deltaX < areaSize; ++deltaX) {
			for (int deltaY = 0; deltaY < areaSize; ++deltaY) {
				int checkX = targetX + deltaX;
				int checkY = targetY + deltaY;
				if (field.isValidPosition(checkX, checkY)) {
					areaCells.insert({checkX, checkY});
				}
			}
		}
		
		int playerX = player.getX();
		int playerY = player.getY();
		
		std::cout << "\n";
		for (int y = 0; y < field.getHeight(); ++y) {
			for (int x = 0; x < field.getWidth(); ++x) {
				if (x == playerX && y == playerY) {
					std::cout << "P ";
					continue;
				}
				
				bool isInArea = areaCells.find({x, y}) != areaCells.end();
				
				if (isInArea) {
					bool hasTarget = false;
					
					for (const auto& enemy : game.getEnemyManager().getEnemies()) {
						if (enemy.isAlive() && enemy.getPositionX() == x && enemy.getPositionY() == y) {
							std::cout << "E*";
							hasTarget = true;
							break;
						}
					}
					if (hasTarget) continue;
					
					for (const auto& building : game.getBuildingManager().getBuildings()) {
						if (building.isAlive() && building.getPositionX() == x && building.getPositionY() == y) {
							std::cout << "B*";
							hasTarget = true;
							break;
						}
					}
					if (hasTarget) continue;
					
					for (const auto& tower : game.getBuildingManager().getTowers()) {
						if (tower.isAlive() && tower.getPositionX() == x && tower.getPositionY() == y) {
							std::cout << "T*";
							hasTarget = true;
							break;
						}
					}
					if (hasTarget) continue;
					
					std::cout << "* ";
					continue;
				}
				
				bool hasEnemy = false;
				for (const auto& enemy : game.getEnemyManager().getEnemies()) {
					if (enemy.isAlive() && enemy.getPositionX() == x && enemy.getPositionY() == y) {
						std::cout << "E ";
						hasEnemy = true;
						break;
					}
				}
				if (hasEnemy) continue;
				
				bool hasTower = false;
				for (const auto& tower : game.getBuildingManager().getTowers()) {
					if (tower.isAlive() && tower.getPositionX() == x && tower.getPositionY() == y) {
						std::cout << "T ";
						hasTower = true;
						break;
					}
				}
				if (hasTower) continue;
				
				bool hasBuilding = false;
				for (const auto& building : game.getBuildingManager().getBuildings()) {
					if (building.isAlive() && building.getPositionX() == x && building.getPositionY() == y) {
						std::cout << "B ";
						hasBuilding = true;
						break;
					}
				}
				if (hasBuilding) continue;
				
				switch (field.getCellType(x, y)) {
					case CellType::WALL: std::cout << "# "; break;
					case CellType::SLOW: std::cout << "~ "; break;
					case CellType::EMPTY: std::cout << ". "; break;
				}
			}
			std::cout << "\n";
		}
		std::cout << "\nLegend: * = area of effect, E*/B*/T* = targets in area\n";
	}

	void notifySpellDamage(const std::string& spellName, const std::string& targetName, int damage, int targetHealth, int targetMaxHealth) override {
		// Game решает, логировать ли это событие
		std::ostringstream ss;
		ss << spellName << " hits " << targetName << " for " << damage 
		   << " damage! HP: " << targetHealth << "/" << targetMaxHealth;
		game.logEvent(GameEventType::Damage, ss.str());
	}

	void notifySpellTargetDefeated(const std::string& targetName) override {
		// Game решает, логировать ли это событие
		std::ostringstream ss;
		ss << targetName << " defeated by spell";
		game.logEvent(GameEventType::Damage, ss.str());
	}

	void notifySpellAreaHit(const std::string& spellName, int x, int y, int damage) override {
		// Game решает, логировать ли это событие
		std::ostringstream ss;
		ss << spellName << " hits target at (" << x << ", " << y 
		   << ") for " << damage << " damage";
		game.logEvent(GameEventType::Damage, ss.str());
	}

	void notifySpellNoTargets(const std::string& spellName) override {
		// Game решает, логировать ли это событие
		std::ostringstream ss;
		ss << spellName << " exploded but hit no targets";
		game.logEvent(GameEventType::Spell, ss.str());
	}

	void notifyAllySummoned(int count, int x, int y) override {
		// Game решает, логировать ли это событие
		std::ostringstream ss;
		ss << "Ally summoned at (" << x << ", " << y << ")";
		game.logEvent(GameEventType::System, ss.str());
	}

	void notifyTrapPlaced(const std::string& spellName, int x, int y) override {
		// Game решает, логировать ли это событие
		std::ostringstream ss;
		ss << spellName << " placed at (" << x << ", " << y << ")";
		game.logEvent(GameEventType::System, ss.str());
	}

private:
	Game& game;
};

void Game::initializeSpells() {
    availableSpells.clear();
    availableSpells.push_back(std::make_unique<DirectDamageSpell>());
    availableSpells.push_back(std::make_unique<AreaDamageSpell>());
    availableSpells.push_back(std::make_unique<TrapSpell>());
    availableSpells.push_back(std::make_unique<SummonSpell>());
    availableSpells.push_back(std::make_unique<EnhancementSpell>());
    giveRandomSpell();
}

void Game::applyPermanentEnhancementsToSpell(Spell& spell) const {
	SpellType type = spell.getSpellType();
	auto it = permanentSpellEnhancements.find(type);
	if (it != permanentSpellEnhancements.end() && it->second > 0) {
		spell.applyPermanentEnhancement(it->second);
	}
}

void Game::giveRandomSpell() {
    if (player.getHand().isFull()) {
        return;
    }
    
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<> distribution(0, availableSpells.size() - 1);
    
    int randomIndex = distribution(generator);
    auto newSpell = availableSpells[randomIndex]->clone();
    applyPermanentEnhancementsToSpell(*newSpell);
    player.getHand().addSpell(std::move(newSpell));
	logEvent(GameEventType::Resource, "New spell acquired: " + availableSpells[randomIndex]->getName());
}

void Game::giveSpellForEnemyKill() {
    enemiesKilledSinceLastSpell++;
    if (enemiesKilledSinceLastSpell >= ENEMIES_PER_SPELL) {
        enemiesKilledSinceLastSpell = 0;
        giveRandomSpell();
		logEvent(GameEventType::Resource, "Reward spell for enemy kills");
    }
}

bool Game::buySpell() {
    if (player.getScore() >= SPELL_COST) {
        if (!player.getHand().isFull()) {
            player.addScore(-SPELL_COST);
            giveRandomSpell();
            return true;
        } else {
            std::cout << "Your hand is full! Cannot buy more spells.\n";
            return false;
        }
    } else {
        std::cout << "Not enough score! Need " << SPELL_COST << ", you have " << player.getScore() << "\n";
        return false;
    }
}

void Game::displaySpells() const {
    player.getHand().displaySpells();
}

bool Game::castSpell(int spellIndex) {
    Hand& hand = player.getHand();
    if (spellIndex < 0 || spellIndex >= hand.getSpellCount()) {
        std::cout << "Invalid spell index!\n";
        return false;
    }
    
    GameSpellContext context(*this);
    bool result = player.castSpellWithEnhancements(spellIndex, context);
    if (result) {
        hand.removeSpell(spellIndex);
    }
    return result;
}