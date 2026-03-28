#include "Field.hpp"
#include "EnemyController.hpp"
#include "NestController.hpp"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <random>
#include <sstream>
#include <limits>
#include "GameExceptions.hpp"

Field::Field(int width, int height,
             unsigned int seed,
             double wallProbability,
             double slowProbability)
	: width_(width),
	  height_(height),
	  grid_(height, std::vector<Cell>(width, Cell(Cell::Type::Floor))),
	  player_(1, 0, 0),
	  seed_(seed ? seed : static_cast<unsigned int>(std::random_device{}())),
	  rng_(seed_),
	  wallProb_(wallProbability),
	  slowProb_(slowProbability)
{
	generateObstacles_();
}


Field::Field(const Field& other)
	: width_(other.width_),
	height_(other.height_),
	grid_(other.grid_),
	playerPos_(other.playerPos_),
	player_(other.player_),
	enemies_(other.enemies_),
	nests_(other.nests_),
	towerPositions_(other.towerPositions_),
	allyOverlay_(other.allyOverlay_),
	trapOverlay_(other.trapOverlay_),
	pendingAllyDamage_(other.pendingAllyDamage_),
	seed_(other.seed_),
	rng_(other.rng_),
	wallProb_(other.wallProb_),
	slowProb_(other.slowProb_),
	enemySpawnHp_(other.enemySpawnHp_),
	enemySpawnDmg_(other.enemySpawnDmg_),
	lastPlayerDamage_(other.lastPlayerDamage_)
{}

Field::Field(Field&& other) noexcept
	: width_(other.width_),
	height_(other.height_),
	grid_(std::move(other.grid_)),
	playerPos_(std::move(other.playerPos_)),
	player_(std::move(other.player_)),
	enemies_(std::move(other.enemies_)),
	nests_(std::move(other.nests_)),
	towerPositions_(std::move(other.towerPositions_)),
	allyOverlay_(std::move(other.allyOverlay_)),
	trapOverlay_(std::move(other.trapOverlay_)),
	pendingAllyDamage_(std::move(other.pendingAllyDamage_)),
	seed_(other.seed_),
	rng_(std::move(other.rng_)),
	wallProb_(other.wallProb_),
	slowProb_(other.slowProb_),
	enemySpawnHp_(other.enemySpawnHp_),
	enemySpawnDmg_(other.enemySpawnDmg_),
	lastPlayerDamage_(other.lastPlayerDamage_)
{}

Field& Field::operator=(const Field& other) {
	if (this != &other) {
		width_ = other.width_;
		height_ = other.height_;
		grid_ = other.grid_;
		playerPos_ = other.playerPos_;
		player_ = other.player_;
		enemies_ = other.enemies_;
		nests_ = other.nests_;
		towerPositions_ = other.towerPositions_;
		allyOverlay_ = other.allyOverlay_;
		trapOverlay_ = other.trapOverlay_;
		pendingAllyDamage_ = other.pendingAllyDamage_;
		seed_ = other.seed_;
		rng_ = other.rng_;
		wallProb_ = other.wallProb_;
		slowProb_ = other.slowProb_;
		enemySpawnHp_ = other.enemySpawnHp_;
		enemySpawnDmg_ = other.enemySpawnDmg_;
		lastPlayerDamage_ = other.lastPlayerDamage_;
	}
	return *this;
}

Field& Field::operator=(Field&& other) noexcept {
	if (this != &other) {
		width_ = other.width_;
		height_ = other.height_;
		grid_ = std::move(other.grid_);
		playerPos_ = std::move(other.playerPos_);
		player_ = std::move(other.player_);
		enemies_ = std::move(other.enemies_);
		nests_ = std::move(other.nests_);
		towerPositions_ = std::move(other.towerPositions_);
		allyOverlay_ = std::move(other.allyOverlay_);
		trapOverlay_ = std::move(other.trapOverlay_);
		pendingAllyDamage_ = std::move(other.pendingAllyDamage_);
		seed_ = other.seed_;
		rng_ = std::move(other.rng_);
		wallProb_ = other.wallProb_;
		slowProb_ = other.slowProb_;
		enemySpawnHp_ = other.enemySpawnHp_;
		enemySpawnDmg_ = other.enemySpawnDmg_;
		lastPlayerDamage_ = other.lastPlayerDamage_;
	}
	return *this;
}

Field::~Field() = default;

int Field::width() const noexcept { return width_; }
int Field::height() const noexcept { return height_; }

void Field::placePlayer(const Player& player, const Position& pos) {
	if (!isInside(pos)) throw std::invalid_argument("placePlayer: out of bounds");
	if (!cellIsFreeAndPassable(pos)) throw std::invalid_argument("placePlayer: occupied");
	if (at(pos).isSlow()) throw std::invalid_argument("placePlayer: cannot spawn on slow cell");
	player_ = player;
	playerPos_ = pos;
}

void Field::addEnemy(const Enemy& enemy, const Position& pos) {
	if (!isInside(pos)) throw std::invalid_argument("addEnemy: out of bounds");
	if (!cellIsFreeAndPassable(pos)) throw std::invalid_argument("addEnemy: not free or not passable");
	enemies_.push_back(EnemyEntry{ enemy, pos });
}

void Field::addNest(const EnemyNest& nest, const Position& pos) {
	if (!isInside(pos)) throw std::invalid_argument("addNest: out of bounds");
	if (!cellIsFreeAndPassable(pos)) throw std::invalid_argument("addNest: not free or not passable");
	nests_.push_back(NestEntry{ nest, pos, nest.spawnPeriod(), nest.maxHealth() });
}

bool Field::isGameOver() const noexcept {
	return playerPos_.has_value() && !player_.isAlive();
}

void Field::advanceTurn() {
	if (!playerPos_.has_value() || !player_.isAlive()) {
		return;
	}
	EnemyController::advanceEnemies(*this);
	NestController::processNests(*this);
}

char Field::symbolAt(const Position& p) const noexcept {
	char ch = '.';
	const Cell& c = at(p);
	if (c.type() == Cell::Type::Wall) ch = '#';
	else if (c.type() == Cell::Type::Slow) ch = '~';

	if (playerPos_.has_value() && *playerPos_ == p) ch = 'P';
	for (const auto& allyPos : allyOverlay_) if (allyPos == p) { ch = 'A'; break; }
	for (const auto& trapPos : trapOverlay_) if (trapPos == p) { ch = '^'; break; }
	for (const auto& e : enemies_) if (e.pos == p) { ch = 'E'; break; }
	for (const auto& n : nests_)   if (n.pos == p) { ch = 'N'; break; }
	for (const auto& t : towerPositions_) if (t == p) { ch = 'T'; break; }
	return ch;
}

int Field::enemyCount() const noexcept {
	return static_cast<int>(enemies_.size());
}

int Field::lastPlayerDamage() const noexcept {
	return lastPlayerDamage_;
}

int Field::enemySpawnHp() const noexcept {
	return enemySpawnHp_;
}

int Field::enemySpawnDamage() const noexcept {
	return enemySpawnDmg_;
}



bool Field::isVictory() const noexcept {
	if (!player_.isAlive()) return false;
	return enemies_.empty() && nests_.empty() && towerPositions_.empty();
}

const Player& Field::player() const noexcept {
	return player_;
}

bool Field::isInside(const Position& p) const noexcept {
	return p.x >= 0 && p.x < width_ && p.y >= 0 && p.y < height_;
}

bool Field::cellIsPassable(const Position& p) const noexcept {
	return at(p).isPassable();
}

bool Field::cellIsFreeAndPassable(const Position& p) const noexcept {
	if (!cellIsPassable(p)) return false;
	if (playerPos_.has_value() && *playerPos_ == p) return false;
	for (const auto& e : enemies_) if (e.pos == p) return false;
	for (const auto& n : nests_) if (n.pos == p) return false;
	for (const auto& t : towerPositions_) if (t == p) return false;
	if (hasAllyAt(p)) return false;
	return true;
}

int Field::findEnemyAt(const Position& p) const noexcept {
	for (int i = 0; i < static_cast<int>(enemies_.size()); ++i) {
		if (enemies_[i].pos == p) return i;
	}
	return -1;
}

int Field::findNestAt(const Position& p) const noexcept {
	for (int i = 0; i < static_cast<int>(nests_.size()); ++i) {
		if (nests_[i].pos == p) return i;
	}
	return -1;
}

Cell& Field::at(const Position& p) {
	return grid_[p.y][p.x];
}
const Cell& Field::at(const Position& p) const {
	return grid_[p.y][p.x];
}

Position Field::stepOf(Direction dir, const Position& from) const noexcept {
    switch (dir) {
        case Direction::Up:    return Position{ from.x, from.y - 1 };
        case Direction::Down:  return Position{ from.x, from.y + 1 };
        case Direction::Left:  return Position{ from.x - 1, from.y };
        case Direction::Right: return Position{ from.x + 1, from.y };
    }
    return from;
}

void Field::setEnemySpawnStats(int hp, int dmg) {
	if (hp <= 0) throw std::invalid_argument("setEnemySpawnStats: hp must be > 0");
	if (dmg < 0) throw std::invalid_argument("setEnemySpawnStats: dmg must be >= 0");
	enemySpawnHp_ = hp;
	enemySpawnDmg_ = dmg;
}

void Field::generateObstacles_() {
	std::bernoulli_distribution wallD(wallProb_);
	std::bernoulli_distribution slowD(slowProb_);

	for (int y = 0; y < height_; ++y) {
		for (int x = 0; x < width_; ++x) {
			if (wallD(rng_)) {
				grid_[y][x] = Cell(Cell::Type::Wall);
			} else {
				grid_[y][x] = slowD(rng_) ? Cell(Cell::Type::Slow)
				                          : Cell(Cell::Type::Floor);
			}
		}
	}
	int cx = width_ / 2, cy = height_ / 2;
	for (int dy = -1; dy <= 1; ++dy)
		for (int dx = -1; dx <= 1; ++dx) {
			Position p{ cx + dx, cy + dy };
			if (isInside(p)) grid_[p.y][p.x] = Cell(Cell::Type::Floor);
		}
}
Position Field::randomFreeCell_() {
	std::vector<Position> freeCells;
	freeCells.reserve(width_ * height_);

	for (int y = 0; y < height_; ++y) {
		for (int x = 0; x < width_; ++x) {
			Position p{x, y};
			if (cellIsFreeAndPassable(p)) {
				freeCells.push_back(p);
			}
		}
	}
	if (freeCells.empty()) {
		throw std::runtime_error("No free passable cells to place entity");
	}
	std::uniform_int_distribution<size_t> pick(0, freeCells.size() - 1);
	return freeCells[pick(rng_)];
}

Position Field::randomFreeNonSlowCell_() {
	std::vector<Position> candidates;
	candidates.reserve(width_ * height_);

	for (int y = 0; y < height_; ++y) {
		for (int x = 0; x < width_; ++x) {
			Position p{x, y};
			if (cellIsFreeAndPassable(p) && !at(p).isSlow()) {
				candidates.push_back(p);
			}
		}
	}
	if (candidates.empty()) {
		throw std::runtime_error("No free non-slow cells to place player");
	}
	std::uniform_int_distribution<size_t> pick(0, candidates.size() - 1);
	return candidates[pick(rng_)];
}

void Field::placePlayerRandom(const Player& player) {
	Position p = randomFreeNonSlowCell_();
	placePlayer(player, p);
}
void Field::addEnemyRandom(const Enemy& enemy) {
	Position p = randomFreeCell_();
	addEnemy(enemy, p);
}

void Field::addNestRandom(const EnemyNest& nest) {
	Position p = randomFreeCell_();
	addNest(nest, p);
}

bool Field::tryPlacePlayerRandom(const Player& player) {
	try {
		placePlayerRandom(player);
		return true;
	} catch (...) { return false; }
}

bool Field::tryAddEnemyRandom(const Enemy& enemy) {
	try {
		addEnemyRandom(enemy);
		return true;
	} catch (...) { return false; }
}

bool Field::tryAddNestRandom(const EnemyNest& nest) {
	try {
		addNestRandom(nest);
		return true;
	} catch (...) { return false; }
}

std::optional<Position> Field::playerPosition() const noexcept {
	return playerPos_;
}

bool Field::contains(const Position& p) const noexcept {
	return isInside(p);
}

bool Field::isCellPassable(const Position& p) const noexcept {
	return isInside(p) && cellIsPassable(p);
}

bool Field::isCellSlow(const Position& p) const noexcept {
	if (!contains(p)) return false;
	return at(p).isSlow();
}

bool Field::hasEnemyAt(const Position& p) const noexcept {
	return findEnemyAt(p) != -1;
}

bool Field::hasNestAt(const Position& p) const noexcept {
	return findNestAt(p) != -1;
}

bool Field::hasTowerAt(const Position& p) const noexcept {
	try {
		const std::size_t n = towerPositions_.size();
		for (std::size_t i = 0; i < n; ++i) {
			if (towerPositions_[i] == p) {
				return true;
			}
		}
		return false;
	} catch (...) {
		return false;
	}
}

bool Field::isCellFreeForStructure(const Position& p) const noexcept {
	if (!isInside(p) || !cellIsPassable(p)) return false;
	if (playerPos_.has_value() && *playerPos_ == p) return false;
	return !hasEnemyAt(p) && !hasNestAt(p) && !hasTowerAt(p);
}

bool Field::dealDamageToEnemyAt(const Position& p, int damage) {
	if (damage < 0) throw std::invalid_argument("dealDamageToEnemyAt: damage < 0");
	int idx = findEnemyAt(p);
	if (idx == -1) {
		return false;
	}
	applyDamageToEnemy_(idx, damage);
	return true;
}

bool Field::dealDamageToNestAt(const Position& p, int damage) {
	if (damage < 0) throw std::invalid_argument("dealDamageToNestAt: damage < 0");
	int idx = findNestAt(p);
	if (idx == -1) {
		return false;
	}
	if (damage == 0) {
		return false;
	}
	NestEntry& entry = nests_[idx];
	extern bool g_verbose;
	if (g_verbose) {
		std::cout << "[DBG] dealDamageToNestAt: pos=(" << entry.pos.x << "," << entry.pos.y << ")"
		          << " hp(before)=" << entry.health
		          << " applying dmg=" << damage << std::endl;
	}
	entry.health -= damage;
	if (entry.health <= 0) {
		if (g_verbose) {
			std::cout << "[DBG] Nest destroyed at (" << entry.pos.x << "," << entry.pos.y << ")\n";
		}
		nests_.erase(nests_.begin() + idx);
	} else if (g_verbose) {
		std::cout << "[DBG] Nest hp(after)=" << entry.health << std::endl;
	}
	return true;
}

int Field::enemyDamageAt(const Position& p) const noexcept {
	for (const auto& e : enemies_) {
		if (e.pos == p) return e.enemy.damage();
	}
	return -1;
}

bool Field::removeNestAt(const Position& p) {
	int idx = findNestAt(p);
	if (idx == -1) {
		return false;
	}
	nests_.erase(nests_.begin() + idx);
	return true;
}

bool Field::registerTowerAt(const Position& p) {
	if (!isCellFreeForStructure(p)) {
		return false;
	}
	towerPositions_.push_back(p);
	return true;
}

void Field::removeTowerAt(const Position& p) {
	auto it = std::remove(towerPositions_.begin(), towerPositions_.end(), p);
	towerPositions_.erase(it, towerPositions_.end());
}

std::vector<Position> Field::enemyPositions() const {
	std::vector<Position> out;
	out.reserve(enemies_.size());
	for (const auto& e : enemies_) {
		out.push_back(e.pos);
	}
	return out;
}

bool Field::hasAllyAt(const Position& p) const noexcept {
	for (const auto& a : allyOverlay_) {
		if (a == p) return true;
	}
	return false;
}

bool Field::hasTrapAt(const Position& p) const noexcept {
	for (const auto& t : trapOverlay_) {
		if (t == p) return true;
	}
	return false;
}

bool Field::applyDamageToPlayer(int damage) {
	if (damage < 0) throw std::invalid_argument("applyDamageToPlayer: damage < 0");
	if (!player_.isAlive()) {
		return false;
	}
	player_.takeDamage(damage);
	lastPlayerDamage_ = damage;
	return true;
}

void Field::setAllyOverlay(const std::vector<Position>& allies) {
	allyOverlay_ = allies;
}

void Field::setTrapOverlay(const std::vector<Position>& traps) {
	trapOverlay_ = traps;
}

void Field::recordAllyDamageRequest(const Position& p, int damage) {
	if (damage < 0) return;
	if (!isInside(p)) return;
	pendingAllyDamage_.push_back(std::make_pair(p, damage));
}

std::vector<std::pair<Position,int>> Field::drainAllyDamageRequests() {
	std::vector<std::pair<Position,int>> out = std::move(pendingAllyDamage_);
	pendingAllyDamage_.clear();
	return out;
}

void Field::applyDamageToEnemy_(int index, int damage) {
	if (index < 0 || index >= static_cast<int>(enemies_.size()))
		throw std::out_of_range("applyDamageToEnemy_: bad enemy index");
	if (damage < 0) throw std::invalid_argument("applyDamageToEnemy_: damage < 0");

	extern bool g_verbose;
	if (g_verbose) {
		std::cout << "[DBG] applyDamageToEnemy_: enemies.size=" << enemies_.size()
				  << " index=" << index
				  << " pos=(" << enemies_[index].pos.x << "," << enemies_[index].pos.y << ")"
				  << " enemy.damage=" << enemies_[index].enemy.damage()
				  << " enemy.isAlive(before)=" << enemies_[index].enemy.isAlive()
				  << " applying dmg=" << damage << std::endl;
	}
	enemies_[index].enemy.takeDamage(damage);
	if (g_verbose) {
		std::cout << "[DBG] after takeDamage: enemy.isAlive=" << enemies_[index].enemy.isAlive()
				  << " enemies.size(beforeRemoval)=" << enemies_.size()
				  << std::endl;
	}
	if (!enemies_[index].enemy.isAlive()) {
		int beforeKills = player_.kills();
		player_.addKill();
		if (g_verbose) std::cout << "[DBG] Enemy died at pos=(" << enemies_[index].pos.x << "," << enemies_[index].pos.y << ")"
								  << " -> removing index=" << index
								  << " (enemies.size before)=" << enemies_.size()
								  << " player.kills: " << beforeKills << " -> " << player_.kills()
								  << std::endl;
		enemies_.erase(enemies_.begin() + index);
		if (g_verbose) std::cout << "[DBG] enemies.size after removal=" << enemies_.size() << std::endl;
	}
}

Player& Field::player() noexcept {
    return player_;
}

void Field::save(std::ostream& out) const {
	out << "FIELD_V1\n";
	out << width_ << ' ' << height_ << '\n';
	out << wallProb_ << ' ' << slowProb_ << '\n';
	out << seed_ << '\n';
	std::ostringstream rngState;
	rngState << rng_;
	out << rngState.str() << '\n';
	for (int y = 0; y < height_; ++y) {
		for (int x = 0; x < width_; ++x) {
			const Cell& cell = grid_[y][x];
			char symbol = '.';
			if (cell.type() == Cell::Type::Wall) symbol = '#';
			else if (cell.type() == Cell::Type::Slow) symbol = '~';
			out << symbol;
		}
		out << '\n';
	}

	out << enemySpawnHp_ << ' ' << enemySpawnDmg_ << '\n';
	out << lastPlayerDamage_ << '\n';
	out << static_cast<int>(playerPos_.has_value()) << '\n';

	if (playerPos_) {
		out << playerPos_->x << ' ' << playerPos_->y << '\n';
		out << player_.maxHealth() << ' '
		    << player_.currentHealth() << ' '
		    << player_.meleeDamage() << ' '
		    << player_.rangedDamage() << ' '
		    << player_.rangedRange() << ' '
		    << player_.kills() << ' '
		    << static_cast<int>(player_.mode()) << ' '
		    << static_cast<int>(player_.isMovementBlocked()) << '\n';
	}

	out << enemies_.size() << '\n';
	for (const auto& entry : enemies_) {
		out << entry.pos.x << ' ' << entry.pos.y << ' '
		    << entry.enemy.health() << ' '
		    << entry.enemy.damage() << ' '
		    << static_cast<int>(entry.skipMoveNextTurn) << '\n';
	}

	out << nests_.size() << '\n';
	for (const auto& entry : nests_) {
		out << entry.pos.x << ' ' << entry.pos.y << ' '
		    << entry.nest.spawnPeriod() << ' '
		    << entry.health << ' '
		    << entry.counter << '\n';
	}

	out << towerPositions_.size() << '\n';
	for (const auto& pos : towerPositions_) {
		out << pos.x << ' ' << pos.y << '\n';
	}

	out << allyOverlay_.size() << '\n';
	for (const auto& pos : allyOverlay_) {
		out << pos.x << ' ' << pos.y << '\n';
	}

	out << trapOverlay_.size() << '\n';
	for (const auto& pos : trapOverlay_) {
		out << pos.x << ' ' << pos.y << '\n';
	}

	out << pendingAllyDamage_.size() << '\n';
	for (const auto& request : pendingAllyDamage_) {
		out << request.first.x << ' ' << request.first.y << ' ' << request.second << '\n';
	}
}

Field Field::load(std::istream& in) {
	constexpr const char* header = "FIELD_V1";
	std::string token;
	if (!(in >> token) || token != header) {
		throw InvalidDataException("Field header mismatch");
	}

	int width = 0;
	int height = 0;
	double wallProb = 0.0;
	double slowProb = 0.0;
	unsigned int seed = 0;
	if (!(in >> width >> height)) {
		throw InvalidDataException("Failed to read field dimensions");
	}
	if (!(in >> wallProb >> slowProb)) {
		throw InvalidDataException("Failed to read field probabilities");
	}
	if (!(in >> seed)) {
		throw InvalidDataException("Failed to read field seed");
	}
	Field field(width, height, seed, wallProb, slowProb);

	std::string line;
	in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	if (!std::getline(in, line)) {
		throw InvalidDataException("Failed to read RNG state");
	}
	if (!line.empty()) {
		std::istringstream rngState(line);
		rngState >> field.rng_;
	}

	for (int y = 0; y < height; ++y) {
		if (!std::getline(in, line) || static_cast<int>(line.size()) < width) {
			throw InvalidDataException("Corrupted grid row");
		}
		for (int x = 0; x < width; ++x) {
			char c = line[x];
			switch (c) {
			case '#': field.grid_[y][x] = Cell(Cell::Type::Wall); break;
			case '~': field.grid_[y][x] = Cell(Cell::Type::Slow); break;
			default : field.grid_[y][x] = Cell(Cell::Type::Floor); break;
			}
		}
	}

	if (!(in >> field.enemySpawnHp_ >> field.enemySpawnDmg_)) {
		throw InvalidDataException("Failed to read spawn stats");
	}
	if (!(in >> field.lastPlayerDamage_)) {
		throw InvalidDataException("Failed to read last player damage");
	}

	int hasPlayer = 0;
	if (!(in >> hasPlayer)) {
		throw InvalidDataException("Failed to read player presence");
	}
	if (hasPlayer) {
		Position pos{};
		if (!(in >> pos.x >> pos.y)) {
			throw InvalidDataException("Failed to read player position");
		}
		field.playerPos_ = pos;

		int maxHealth = 0;
		int currentHealth = 0;
		int meleeDamage = 0;
		int rangedDamage = 0;
		int rangedRange = 0;
		int kills = 0;
		int mode = 0;
		int blocked = 0;
		if (!(in >> maxHealth >> currentHealth >> meleeDamage >> rangedDamage
		      >> rangedRange >> kills >> mode >> blocked)) {
			throw InvalidDataException("Failed to read player stats");
		}
		field.player_ = Player(maxHealth, meleeDamage, rangedDamage);
		field.player_.setRangedRange(rangedRange);
		field.player_.setCurrentHealth(currentHealth);
		field.player_.setKills(kills);
		if (mode == static_cast<int>(Player::CombatMode::Ranged)) {
			field.player_.setMode(Player::CombatMode::Ranged);
		} else {
			field.player_.setMode(Player::CombatMode::Melee);
		}
		field.player_.setMovementBlocked(blocked != 0);
	} else {
		field.playerPos_.reset();
	}

	std::size_t enemyCount = 0;
	if (!(in >> enemyCount)) {
		throw InvalidDataException("Failed to read enemy count");
	}
	field.enemies_.clear();
	for (std::size_t i = 0; i < enemyCount; ++i) {
		Position pos{};
		int health = 0;
		int damage = 0;
		int skip = 0;
		if (!(in >> pos.x >> pos.y >> health >> damage >> skip)) {
			throw InvalidDataException("Failed to read enemy entry");
		}
		int safeHealth = std::max(health, 1);
		EnemyEntry entry{ Enemy(safeHealth, damage), pos, skip != 0 };
		entry.enemy.setHealth(health);
		field.enemies_.push_back(entry);
	}

	std::size_t nestCount = 0;
	if (!(in >> nestCount)) {
		throw InvalidDataException("Failed to read nest count");
	}
	field.nests_.clear();
	for (std::size_t i = 0; i < nestCount; ++i) {
		Position pos{};
		int spawnPeriod = 0;
		int health = 0;
		int counter = 0;
		if (!(in >> pos.x >> pos.y >> spawnPeriod >> health >> counter)) {
			throw InvalidDataException("Failed to read nest entry");
		}
		Field::NestEntry entry{ EnemyNest(spawnPeriod), pos, counter, health };
		field.nests_.push_back(entry);
	}

	std::size_t towerCount = 0;
	if (!(in >> towerCount)) {
		throw InvalidDataException("Failed to read tower count");
	}
	field.towerPositions_.clear();
	for (std::size_t i = 0; i < towerCount; ++i) {
		Position pos{};
		if (!(in >> pos.x >> pos.y)) {
			throw InvalidDataException("Failed to read tower position");
		}
		field.towerPositions_.push_back(pos);
	}

	std::size_t allyOverlayCount = 0;
	if (!(in >> allyOverlayCount)) {
		throw InvalidDataException("Failed to read ally overlay count");
	}
	field.allyOverlay_.clear();
	for (std::size_t i = 0; i < allyOverlayCount; ++i) {
		Position pos{};
		if (!(in >> pos.x >> pos.y)) {
			throw InvalidDataException("Failed to read ally overlay");
		}
		field.allyOverlay_.push_back(pos);
	}

	std::size_t trapOverlayCount = 0;
	if (!(in >> trapOverlayCount)) {
		throw InvalidDataException("Failed to read trap overlay count");
	}
	field.trapOverlay_.clear();
	for (std::size_t i = 0; i < trapOverlayCount; ++i) {
		Position pos{};
		if (!(in >> pos.x >> pos.y)) {
			throw InvalidDataException("Failed to read trap overlay");
		}
		field.trapOverlay_.push_back(pos);
	}

	std::size_t damageRequests = 0;
	if (!(in >> damageRequests)) {
		throw InvalidDataException("Failed to read ally damage requests");
	}
	field.pendingAllyDamage_.clear();
	for (std::size_t i = 0; i < damageRequests; ++i) {
		Position pos{};
		int damage = 0;
		if (!(in >> pos.x >> pos.y >> damage)) {
			throw InvalidDataException("Failed to read damage request");
		}
		field.pendingAllyDamage_.push_back({pos, damage});
	}

	return field;
}



