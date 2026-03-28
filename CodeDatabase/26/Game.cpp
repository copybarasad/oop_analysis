#include "Game.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <sstream>
#include <string>

#include "Enemy.hpp"
#include "EnemyNest.hpp"
#include "EventBus.hpp"
#include "SpellBase.hpp"
#include "SpellLibrary.hpp"

namespace {
constexpr const char* kSaveHeader = "GAME_SAVE_V1";

Direction directionFromCommand(GameCommand::Type type) {
	switch (type) {
	case GameCommand::Type::MoveUp:
	case GameCommand::Type::AttackUp:
		return Direction::Up;
	case GameCommand::Type::MoveDown:
	case GameCommand::Type::AttackDown:
		return Direction::Down;
	case GameCommand::Type::MoveLeft:
	case GameCommand::Type::AttackLeft:
		return Direction::Left;
	case GameCommand::Type::MoveRight:
	case GameCommand::Type::AttackRight:
		return Direction::Right;
	default:
		return Direction::Up;
	}
}

int spellHotkeyFromCommand(GameCommand::Type type) {
	switch (type) {
	case GameCommand::Type::CastSpell1: return 1;
	case GameCommand::Type::CastSpell2: return 2;
	case GameCommand::Type::CastSpell3: return 3;
	case GameCommand::Type::CastSpell4: return 4;
	case GameCommand::Type::CastSpell5: return 5;
	default: return -1;
	}
}
} // namespace

Game::Game(EventBus& events, const ControlScheme& controls)
	: currentLevelIndex_(0),
	  rng_(static_cast<unsigned>(std::random_device{}())),
	  campaignActive_(false),
	  exitRequested_(false),
	  lastSavePath_("save.dat"),
	  events_(events),
	  phase_(Phase::MainMenu),
	  controls_(controls) {
	initLevels();
	resetProgress();
	showMainMenu();
}

bool Game::isExitRequested() const noexcept {
	return exitRequested_;
}

void Game::handleCommand(const GameCommand& command) {
	if (command.type() == GameCommand::Type::None) {
		return;
	}
	if (command.type() == GameCommand::Type::ExitApplication) {
		exitRequested_ = true;
		publishEvent("Exit requested", GameEvent::Type::SystemMessage);
		return;
	}

	bool forceRender = false;
	switch (phase_) {
	case Phase::MainMenu:
		forceRender = handleMenuCommand(command.type());
		break;
	case Phase::InLevel:
		forceRender = handleGameplayCommand(command);
		break;
	}

	if (forceRender) {
		requestRender();
	}
}

void Game::initLevels() {
	levels_.clear();
	levels_.push_back(LevelConfig{5, 5, 5, 2, 2, 1, 0, 7});
	levels_.push_back(LevelConfig{9, 9, 6, 2, 3, 2, 1, 7});
	levels_.push_back(LevelConfig{13, 13, 7, 3, 4, 3, 2, 7});
	levels_.push_back(LevelConfig{17, 17, 8, 3, 5, 4, 3, 7});
	levels_.push_back(LevelConfig{21, 21, 10, 4, 6, 5, 4, 7});
	levels_.push_back(LevelConfig{25, 25, 12, 5, 7, 6, 5, 7});
	levelPlayerStats_.assign(levels_.size(), std::nullopt);
}

void Game::resetProgress() {
	progress_ = PlayerProgress{50, 6, 5, 3, 12, 3, 1};
	currentLevelIndex_ = 0;
	retainedSpellHotkeys_.clear();
	levelPlayerStats_.assign(levels_.size(), std::nullopt);
}

void Game::showMainMenu() {
	const std::string startKey = keyLabel(GameCommand::Type::StartCampaign, 'n');
	const std::string loadKey = keyLabel(GameCommand::Type::LoadCampaignFromMenu, 'o');
	const std::string exitKey = keyLabel(GameCommand::Type::ExitApplication, 'x');
	std::cout << "\n======= Main Menu =======\n"
	          << "[" << startKey << "] Start new campaign\n"
	          << "[" << loadKey << "] Load campaign\n"
	          << "[" << exitKey << "] Exit\n";
	std::cout.flush();
}

bool Game::handleMenuCommand(GameCommand::Type type) {
	switch (type) {
	case GameCommand::Type::StartCampaign:
		startNewCampaign();
		return true;
	case GameCommand::Type::LoadCampaignFromMenu:
		if (promptLoadGame()) {
			return true;
		}
		return false;
	case GameCommand::Type::QuitToMenu:
		exitRequested_ = true;
		return false;
	default:
		std::cout << "Select one of the menu commands.\n";
		return false;
	}
}

bool Game::handleGameplayCommand(const GameCommand& command) {
	if (!field_ || !world_) {
		std::cout << "No active level.\n";
		return false;
	}

	Player& player = field_->player();
	bool spentTurn = false;
	bool requiresRender = false;
	switch (command.type()) {
	case GameCommand::Type::MoveUp:
	case GameCommand::Type::MoveDown:
	case GameCommand::Type::MoveLeft:
	case GameCommand::Type::MoveRight: {
		auto before = field_->playerPosition();
		if (player.move(*field_, directionFromCommand(command.type()))) {
			spentTurn = true;
			auto after = field_->playerPosition();
			if (before.has_value() && after.has_value()) {
				publishEvent("Player moved to (" + std::to_string(after->x) + "," +
				             std::to_string(after->y) + ")",
				             GameEvent::Type::PlayerMoved);
			}
		} else {
			std::cout << "You cannot move there.\n";
		}
		break;
	}
	case GameCommand::Type::AttackUp:
	case GameCommand::Type::AttackDown:
	case GameCommand::Type::AttackLeft:
	case GameCommand::Type::AttackRight: {
		const bool hit = player.attack(*field_, directionFromCommand(command.type()));
		spentTurn = true;
		if (hit) {
			publishEvent("Player attack dealing " + std::to_string(player.attackDamage()) + " damage",
			             GameEvent::Type::PlayerAttacked);
		} else {
			std::cout << "Nothing to hit in that direction.\n";
		}
		break;
	}
	case GameCommand::Type::SwitchMode:
		player.switchCombatMode();
		std::cout << "Switched combat mode.\n";
		requiresRender = true;
		break;
	case GameCommand::Type::CastSpell1:
	case GameCommand::Type::CastSpell2:
	case GameCommand::Type::CastSpell3:
	case GameCommand::Type::CastSpell4:
	case GameCommand::Type::CastSpell5: {
		int hotkey = spellHotkeyFromCommand(command.type());
		if (hotkey == -1) {
			break;
		}
		auto& hand = world_->hand();
		int index = hand.findSpellIndexByHotkey(hotkey);
		if (index == -1) {
			std::cout << "No spell is bound to key " << hotkey << ".\n";
			break;
		}
		SpellBase::SpellContext ctx{*world_, field_->player(),
		                            command.hasTarget() ? command.target() : Position{0, 0},
		                            command.hasTarget()};
		if (command.hasTarget() == false &&
		    (command.type() == GameCommand::Type::CastSpell1 ||
		     command.type() == GameCommand::Type::CastSpell2 ||
		     command.type() == GameCommand::Type::CastSpell3)) {
			std::cout << "This spell requires coordinates.\n";
			break;
		}
		if (hand.castSpell(static_cast<std::size_t>(index), ctx)) {
			spentTurn = true;
			publishEvent("Spell cast via hotkey " + std::to_string(hotkey),
			             GameEvent::Type::SpellCast);
		} else {
			std::cout << "The spell fizzled.\n";
		}
		break;
	}
	case GameCommand::Type::SaveGame:
		promptSaveGame();
		break;
	case GameCommand::Type::LoadGame:
		if (promptLoadGame(false)) {
			requiresRender = true;
		}
		break;
	case GameCommand::Type::QuitToMenu:
		enterMainMenu();
		return false;
	default:
		std::cout << "Command is not available during gameplay.\n";
		break;
	}

	if (spentTurn) {
		applyTimeStep();
		requiresRender = true;
		if (field_ && field_->isVictory()) {
			publishEvent("Level cleared", GameEvent::Type::SystemMessage);
			dropHalfOfHand();
			field_.reset();
			world_.reset();
			advanceToNextLevel();
			if (phase_ == Phase::MainMenu) {
				requiresRender = false;
			}
		} else if (field_ && field_->isGameOver()) {
			publishEvent("Player defeated", GameEvent::Type::SystemMessage);
			bool retry = handleDefeatMenu();
			if (retry) {
				setupLevel(currentLevelIndex_);
				requiresRender = true;
			} else {
				enterMainMenu();
				requiresRender = false;
			}
		}
	}

	return requiresRender;
}

void Game::startNewCampaign() {
	resetProgress();
	campaignActive_ = true;
	phase_ = Phase::InLevel;
	setupLevel(currentLevelIndex_);
	publishEvent("Campaign started", GameEvent::Type::SystemMessage);
}

bool Game::promptLoadGame(bool startImmediately) {
	std::string defaultPath = lastSavePath_.empty() ? "save.dat" : lastSavePath_;
	std::string path = prompt("Enter save file path (default: " + defaultPath + "): ");
	if (path.empty()) {
		path = defaultPath;
	}
	try {
		loadFromFile(path);
		std::cout << "Save loaded from \"" << path << "\".\n";
		if (startImmediately) {
			phase_ = Phase::InLevel;
			campaignActive_ = true;
			requestRender();
		}
		publishEvent("Game loaded from " + path, GameEvent::Type::SystemMessage);
		return true;
	} catch (const GameException& ex) {
		std::cout << ex.what() << "\n";
	} catch (const std::exception& ex) {
		std::cout << "Unexpected error while loading: " << ex.what() << "\n";
	}
	return false;
}

void Game::promptSaveGame() {
	if (!field_ || !world_) {
		std::cout << "Nothing to save at the moment.\n";
		return;
	}
	std::string defaultPath = lastSavePath_.empty() ? "save.dat" : lastSavePath_;
	std::string path = prompt("Enter save file path (default: " + defaultPath + "): ");
	if (path.empty()) {
		path = defaultPath;
	}
	try {
		saveToFile(path);
		lastSavePath_ = path;
		std::cout << "Game saved to \"" << path << "\".\n";
		publishEvent("Game saved to " + path, GameEvent::Type::SystemMessage);
	} catch (const GameException& ex) {
		std::cout << ex.what() << "\n";
	} catch (const std::exception& ex) {
		std::cout << "Unexpected error while saving: " << ex.what() << "\n";
	}
}

void Game::saveToFile(const std::string& path) {
	if (!field_ || !world_) {
		throw SaveLoadException("Nothing to save at the moment");
	}

	std::ofstream out(path, std::ios::trunc);
	if (!out) {
		throw SaveLoadException("Cannot open \"" + path + "\" for writing");
	}

	std::ostringstream rngState;
	rngState << rng_;

	out << kSaveHeader << '\n';
	out << currentLevelIndex_ << '\n';
	out << progress_.maxHealth << ' '
	    << progress_.meleeDamage << ' '
	    << progress_.rangedDamage << ' '
	    << progress_.rangedRange << ' '
	    << progress_.allyHp << ' '
	    << progress_.allyDamage << ' '
	    << progress_.allyCount << '\n';
	out << rngState.str() << '\n';
	out << "PLAN_V1 " << levelPlayerStats_.size() << '\n';
	for (const auto& snapshot : levelPlayerStats_) {
		if (snapshot.has_value()) {
			out << 1 << ' ' << snapshot->hp << ' ' << snapshot->melee << ' ' << snapshot->ranged << '\n';
		} else {
			out << 0 << '\n';
		}
	}

	field_->save(out);
	world_->save(out);
}

void Game::loadFromFile(const std::string& path) {
	std::ifstream in(path);
	if (!in) {
		throw SaveLoadException("Cannot open \"" + path + "\" for reading");
	}

	std::string header;
	if (!(in >> header) || header != kSaveHeader) {
		throw InvalidDataException("Invalid save header");
	}
	if (!(in >> currentLevelIndex_)) {
		throw InvalidDataException("Failed to read level index");
	}
	in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::string progressLine;
	if (!std::getline(in, progressLine)) {
		throw InvalidDataException("Failed to read player progress line");
	}
	std::istringstream progressStream(progressLine);
	if (!(progressStream >> progress_.maxHealth >> progress_.meleeDamage
	      >> progress_.rangedDamage >> progress_.rangedRange)) {
		throw InvalidDataException("Failed to read player progress values");
	}
	int allyHp = 12;
	int allyDamage = 3;
	int allyCount = 1;
	if (progressStream >> allyHp >> allyDamage >> allyCount) {
		progress_.allyHp = allyHp;
		progress_.allyDamage = allyDamage;
		progress_.allyCount = allyCount;
	} else {
		progress_.allyHp = 12;
		progress_.allyDamage = 3;
		progress_.allyCount = 1;
	}

	std::string rngLine;
	if (!std::getline(in, rngLine)) {
		throw InvalidDataException("Failed to read RNG state");
	}
	if (!rngLine.empty()) {
		std::istringstream rngState(rngLine);
		rngState >> rng_;
	}

	levelPlayerStats_.assign(levels_.size(), std::nullopt);
	std::streampos planPos = in.tellg();
	std::string planToken;
	if (!(in >> planToken)) {
		throw InvalidDataException("Failed to read level stats token");
	}

	std::size_t storedPlanCount = 0;
	if (planToken == "FIELD_V1") {
		in.seekg(planPos);
	} else {
		if (planToken == "PLAN_V1") {
			if (!(in >> storedPlanCount)) {
				throw InvalidDataException("Failed to read level stats count");
			}
		} else {
			try {
				storedPlanCount = static_cast<std::size_t>(std::stoull(planToken));
			} catch (...) {
				throw InvalidDataException("Corrupted level stats marker");
			}
		}
		for (std::size_t i = 0; i < storedPlanCount; ++i) {
			int flag = 0;
			if (!(in >> flag)) {
				throw InvalidDataException("Failed to read level stats flag");
			}
			if (flag != 0) {
				PlayerStatsSnapshot snapshot{};
				if (!(in >> snapshot.hp >> snapshot.melee >> snapshot.ranged)) {
					throw InvalidDataException("Failed to read level stats values");
				}
				if (i < levelPlayerStats_.size()) {
					levelPlayerStats_[i] = snapshot;
				}
			}
		}
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	SpellLibrary::configureSummonStats(progress_.allyHp, progress_.allyDamage, progress_.allyCount);
	SpellLibrary::configureDirectRange(progress_.rangedRange);
	SpellLibrary::configureAreaRange(progress_.rangedRange);

	Field loadedField = Field::load(in);
	field_ = std::make_unique<Field>(std::move(loadedField));

	std::string worldData((std::istreambuf_iterator<char>(in)),
	                      std::istreambuf_iterator<char>());
	if (worldData.empty()) {
		throw InvalidDataException("Missing GameWorld data");
	}
	std::istringstream worldStream(worldData);

	world_ = std::make_unique<GameWorld>(*field_, field_->player(),
	                                     levels_[currentLevelIndex_].handCapacity,
	                                     0u,
	                                     levels_[currentLevelIndex_].towerCount,
	                                     currentLevelIndex_ + 1,
	                                     static_cast<int>(levels_.size()),
	                                     buildLevelPlan(),
	                                     events_,
	                                     &controls_);
	world_->load(worldStream);

	retainedSpellHotkeys_.clear();
	lastSavePath_ = path;
	phase_ = Phase::InLevel;
	campaignActive_ = true;
}

void Game::setupLevel(int levelIndex) {
	const LevelConfig& cfg = levels_[static_cast<std::size_t>(levelIndex)];
	unsigned seed = rng_();
	if (levelIndex >= static_cast<int>(levelPlayerStats_.size())) {
		levelPlayerStats_.resize(levels_.size());
	}
	levelPlayerStats_[levelIndex] = PlayerStatsSnapshot{progress_.maxHealth, progress_.meleeDamage, progress_.rangedDamage};
	SpellLibrary::configureSummonStats(progress_.allyHp, progress_.allyDamage, progress_.allyCount);
	SpellLibrary::configureDirectRange(progress_.rangedRange);
	SpellLibrary::configureAreaRange(progress_.rangedRange);

	field_ = std::make_unique<Field>(cfg.width, cfg.height, seed, 0.12, 0.10);

	Player base(progress_.maxHealth, progress_.meleeDamage, progress_.rangedDamage);
	base.setRangedRange(progress_.rangedRange);
	field_->player() = base;

	if (!field_->tryPlacePlayerRandom(field_->player())) {
		throw GameException("Failed to place the player on the battlefield");
	}

	field_->setEnemySpawnStats(cfg.enemyHp, cfg.enemyDamage);

	for (int i = 0; i < cfg.enemyCount; ++i) {
		int attempts = 0;
		while (!field_->tryAddEnemyRandom(Enemy(cfg.enemyHp, cfg.enemyDamage))) {
			if (++attempts > 40) {
				break;
			}
		}
	}

	for (int i = 0; i < cfg.nestCount; ++i) {
		int attempts = 0;
		while (!field_->tryAddNestRandom(EnemyNest(4))) {
			if (++attempts > 40) {
				break;
			}
		}
	}

	world_ = std::make_unique<GameWorld>(*field_, field_->player(),
	                                     cfg.handCapacity,
	                                     rng_(),
	                                     cfg.towerCount,
	                                     levelIndex + 1,
	                                     static_cast<int>(levels_.size()),
	                                     buildLevelPlan(),
	                                     events_,
	                                     &controls_);

	if (!retainedSpellHotkeys_.empty()) {
		auto& hand = world_->hand();
		for (int hotkey : retainedSpellHotkeys_) {
			auto spell = SpellLibrary::createSpellByHotkey(hotkey);
			if (spell) {
				if (!hand.addSpell(std::move(spell))) {
					break;
				}
			}
		}
		retainedSpellHotkeys_.clear();
	}
	if (world_->hand().size() == 0) {
		world_->giveRandomStartingSpell();
	}

	requestRender();
}

void Game::applyTimeStep() {
	field_->advanceTurn();
	world_->advanceWorldTurn();
	world_->tryAwardSpell(field_->player());
}

void Game::dropHalfOfHand() {
	retainedSpellHotkeys_.clear();
	if (!world_) {
		return;
	}
	auto& hand = world_->hand();
	const std::size_t count = hand.size();
	if (count == 0) {
		return;
	}
	if (count > 1) {
		std::vector<std::size_t> indices(count);
		std::iota(indices.begin(), indices.end(), 0);
		std::shuffle(indices.begin(), indices.end(), rng_);
		const std::size_t removeCount = count / 2;
		std::vector<std::size_t> toRemove(indices.begin(), indices.begin() + removeCount);
		std::sort(toRemove.begin(), toRemove.end(), [](std::size_t lhs, std::size_t rhs) {
			return lhs > rhs;
		});
		for (std::size_t idx : toRemove) {
			if (idx < hand.size()) {
				hand.removeAt(idx);
			}
		}
	}
	const auto& cards = hand.cards();
	for (const auto& card : cards) {
		if (card) {
			retainedSpellHotkeys_.push_back(card->hotkey());
		}
	}
}

void Game::applyUpgrade() {
	std::cout << "\nChoose an upgrade:\n"
	          << "1) +15 max health\n"
	          << "2) +3 melee damage\n"
	          << "3) +3 ranged damage\n"
	          << "4) +1 ranged range\n"
	          << "Choice: ";
	std::string choice;
	if (!std::getline(std::cin, choice)) {
		return;
	}
	if (choice == "1") {
		progress_.maxHealth += 15;
		std::cout << "Max health increased.\n";
	} else if (choice == "2") {
		progress_.meleeDamage += 3;
		std::cout << "Melee damage increased.\n";
	} else if (choice == "3") {
		progress_.rangedDamage += 3;
		std::cout << "Ranged damage increased.\n";
	} else if (choice == "4") {
		progress_.rangedRange += 1;
		std::cout << "Ranged range increased.\n";
	} else {
		std::cout << "Upgrade skipped.\n";
	}
}

bool Game::handleDefeatMenu() {
	while (true) {
		std::cout << "You were defeated. Retry level? (y/n): ";
		std::string line;
		if (!std::getline(std::cin, line)) {
			return false;
		}
		if (line == "y" || line == "Y") {
			return true;
		}
		if (line == "n" || line == "N") {
			return false;
		}
		std::cout << "Please answer y or n.\n";
	}
}

void Game::advanceToNextLevel() {
	applyUpgrade();
	progress_.allyHp += 2;
	progress_.allyDamage += 1;
	currentLevelIndex_ += 1;
	if (currentLevelIndex_ >= static_cast<int>(levels_.size())) {
		std::cout << "Campaign complete!\n";
		publishEvent("Campaign complete", GameEvent::Type::SystemMessage);
		enterMainMenu();
	} else {
		setupLevel(currentLevelIndex_);
	}
}

std::string Game::prompt(const std::string& message) const {
	std::cout << message;
	std::string line;
	std::getline(std::cin, line);
	return line;
}

int Game::randomInt(int minInclusive, int maxInclusive) {
	std::uniform_int_distribution<int> dist(minInclusive, maxInclusive);
	return dist(rng_);
}

void Game::requestRender() const {
	if (field_ && world_) {
		events_.publish(GameEvent::stateChanged(field_.get(), world_.get()));
	}
}

void Game::publishEvent(const std::string& message, GameEvent::Type type) const {
	events_.publish(GameEvent::makeMessage(type, message));
}

void Game::enterMainMenu() {
	campaignActive_ = false;
	field_.reset();
	world_.reset();
	phase_ = Phase::MainMenu;
	showMainMenu();
}

std::string Game::keyLabel(GameCommand::Type type, char fallback) const {
	int code = controls_.keyFor(type);
	if (code <= 0) {
		return std::string(1, fallback);
	}
	return std::string(1, static_cast<char>(code));
}

std::vector<GameWorld::LevelPlanEntry> Game::buildLevelPlan() const {
	std::vector<GameWorld::LevelPlanEntry> plan;
	plan.reserve(levels_.size());
	for (std::size_t i = 0; i < levels_.size(); ++i) {
		const LevelConfig& cfg = levels_[i];
		GameWorld::LevelPlanEntry entry;
		entry.levelNumber = static_cast<int>(i) + 1;
		entry.width = cfg.width;
		entry.height = cfg.height;
		entry.enemyHp = cfg.enemyHp;
		entry.enemyDamage = cfg.enemyDamage;
		if (i < levelPlayerStats_.size() && levelPlayerStats_[i].has_value()) {
			entry.playerHp = levelPlayerStats_[i]->hp;
			entry.playerMelee = levelPlayerStats_[i]->melee;
			entry.playerRanged = levelPlayerStats_[i]->ranged;
		}
		plan.push_back(entry);
	}
	return plan;
}
