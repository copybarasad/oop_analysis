#include "Game.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include "GameConstants.h"
#include "SpellFactory.h"
#include "GameSaveException.h"
#include "GameLoadException.h"

Game::Game()
	: field(nullptr),
	currentLevel(1),
	exitRequested(false),
	currentSaveSlot(0) {}

void Game::run() {
	showWelcomeMessage();

	while (!exitRequested) {
		if (!handleStartMenu()) {
			break;
		}
		mainLoop();
	}
}

void Game::showWelcomeMessage() const {
	std::cout << "Добро пожаловать в игру!" << std::endl;
}

bool Game::handleStartMenu() {
	while (!exitRequested) {
		std::cout << "\n1. Новая игра\n2. Загрузить сохранение\n3. Выход\nВыбор: ";
		int choice = 0;
		if (!(std::cin >> choice)) {
			exitRequested = true;
			return false;
		}

		switch (choice) {
			case 1:
				return startNewGame();
			case 2:
				if (loadFromStorage()) {
					return true;
				}
				break;
			case 3:
				exitRequested = true;
				return false;
			default:
				std::cout << "Введите значение от 1 до 3." << std::endl;
		}
	}
	return false;
}

bool Game::startNewGame() {
	currentLevel = 1;
	resetGameState(currentLevel, nullptr);
	populateLevel(*field, currentLevel);
	currentSaveSlot = 0;
	std::cout << "Новая игра запущена. Уровень 1." << std::endl;
	return true;
}

bool Game::loadFromStorage() {
	displaySaveSlotsStatus();
	const int slot = requestSaveSlot("Выберите слот для загрузки (0 - отмена): ", true);
	if (slot == 0) {
		std::cout << "Загрузка отменена." << std::endl;
		return false;
	}
	return loadFromSlot(slot);
}

void Game::mainLoop() {
	while (!exitRequested && field) {
		if (!field->getPlayer().isAlive()) {
			handlePlayerDefeat();
			if (exitRequested || !field) {
				return;
			}
			continue;
		}

		renderState();
		if (!processCommand()) {
			break;
		}

		if (exitRequested || !field) {
			break;
		}

		if (!field->getPlayer().isAlive()) {
			handlePlayerDefeat();
			if (exitRequested) {
				break;
			}
			continue;
		}

		const int scoreRequired = 20 + (currentLevel - 1) * 15;
		if (field->getPlayer().getScore() >= scoreRequired) {
			handleLevelCompletion();
		}
	}
}

void Game::renderState() const {
	std::cout << "\n=== Уровень " << currentLevel << " ===" << std::endl;
	field->display();
	field->getPlayer().getSpellHand().display();
	showCommands();
}

void Game::showCommands() const {
	std::cout << "Команды: WASD - ход, T - сменить тип атаки, F - атака" << std::endl;
	std::cout << "C - заклинание, B - магазин, R - сохранить, L - загрузить, Q - выход" << std::endl;
}

bool Game::processCommand() {
	std::cout << "Введите команду: ";
	char command = '\0';
	if (!(std::cin >> command)) {
		exitRequested = true;
		return false;
	}

	bool actionPerformed = false;
	if (!executePlayerAction(command, actionPerformed)) {
		return false;
	}

	if (actionPerformed) {
		advanceNonPlayerTurns();
	}

	return true;
}

bool Game::executePlayerAction(char command, bool& actionPerformed) {
	const char normalized = static_cast<char>(std::tolower(static_cast<unsigned char>(command)));

	switch (normalized) {
		case 'w':
		case 'a':
		case 's':
		case 'd':
			executeMovement(normalized, actionPerformed);
			return true;
		case 't':
			field->switchPlayerAttackType();
			std::cout << "Тип атаки изменен." << std::endl;
			actionPerformed = true;
			return true;
		case 'f':
			executeAttack(actionPerformed);
			return true;
		case 'c':
			executeSpell(actionPerformed);
			return true;
		case 'b':
			openSpellShop(actionPerformed);
			return true;
		case 'r':
			handleSave();
			return true;
		case 'l':
			handleLoadDuringGame();
			return true;
		case 'q':
			exitRequested = true;
			return false;
		default:
			std::cout << "Неизвестная команда." << std::endl;
			return true;
	}
}

void Game::executeMovement(char command, bool& actionPerformed) {
	int dx = 0;
	int dy = 0;

	switch (command) {
		case 'w':
			dy = -1;
			break;
		case 's':
			dy = 1;
			break;
		case 'a':
			dx = -1;
			break;
		case 'd':
			dx = 1;
			break;
		default:
			return;
	}

	actionPerformed = field->movePlayer(dx, dy);
	if (!actionPerformed) {
		std::cout << "Ход невозможен." << std::endl;
	}
}

void Game::executeAttack(bool& actionPerformed) {
	DirectionOffset direction = requestDirection("Введите направление атаки (WASD): ");
	if (!direction.valid) {
		return;
	}

	actionPerformed = field->attackEnemy(direction.dx, direction.dy);
	if (!actionPerformed) {
		std::cout << "В этой стороне нет врага." << std::endl;
	}
}

void Game::executeSpell(bool& actionPerformed) {
	SpellHand& hand = field->getPlayer().getSpellHand();
	if (hand.getSize() == 0) {
		std::cout << "Нет заклинаний для использования." << std::endl;
		return;
	}

	int index = requestMenuChoice(1, hand.getSize(), "Выберите заклинание: ") - 1;
	Spell* spell = hand.getSpell(index);
	if (!spell) {
		std::cout << "Заклинание не найдено." << std::endl;
		return;
	}

	int targetX = field->getPlayerPosition().x;
	int targetY = field->getPlayerPosition().y;

	if (spell->getId() != SpellId::ENHANCEMENT) {
		DirectionOffset direction = requestDirection("Введите направление применения (WASD): ");
		if (!direction.valid) {
			return;
		}
		targetX += direction.dx;
		targetY += direction.dy;
	}

	if (field->castSpell(index, targetX, targetY)) {
		actionPerformed = true;
	} else {
		std::cout << "Заклинание не сработало." << std::endl;
	}
}

void Game::openSpellShop(bool& actionPerformed) {
	field->displaySpellShop();
	std::cout << "Очки игрока: " << field->getPlayer().getScore() << std::endl;

	int choice = requestMenuChoice(0, 5, "Выбор: ");
	if (choice == 0) {
		return;
	}

	if (!field->getPlayer().canBuySpell()) {
		std::cout << "Недостаточно очков или рука заполнена." << std::endl;
		return;
	}

	std::unique_ptr<Spell> purchase;
	switch (choice) {
		case 1:
			purchase = SpellFactory::create(SpellId::DIRECT_DAMAGE);
			break;
		case 2:
			purchase = SpellFactory::create(SpellId::AREA_DAMAGE);
			break;
		case 3:
			purchase = SpellFactory::create(SpellId::TRAP);
			break;
		case 4:
			purchase = SpellFactory::create(SpellId::SUMMON);
			break;
		case 5:
			purchase = SpellFactory::create(SpellId::ENHANCEMENT);
			break;
		default:
			break;
	}

	if (purchase) {
		field->getPlayer().buySpell(std::move(purchase));
		std::cout << "Заклинание добавлено в руку." << std::endl;
		actionPerformed = true;
	}
}

void Game::handleSave() {
	if (!field) {
		return;
	}

	displaySaveSlotsStatus();
	const int slot = requestSaveSlot("Выберите слот для сохранения (0 - отмена): ", true);
	if (slot == 0) {
		std::cout << "Сохранение отменено." << std::endl;
		return;
	}

	try {
		GameSnapshot snapshot{};
		snapshot.level = currentLevel;
		snapshot.field = field->captureSnapshot();
		persistence.save(snapshot, buildSavePath(slot));
		currentSaveSlot = slot;
		std::cout << "Игра сохранена в слоте " << slot << "." << std::endl;
	} catch (const GameSaveException& exception) {
		std::cout << "Ошибка сохранения: " << exception.what() << std::endl;
	}
}

void Game::handleLoadDuringGame() {
	if (!loadFromStorage()) {
		std::cout << "Не удалось загрузить игру." << std::endl;
	}
}

Game::DirectionOffset Game::requestDirection(const std::string& prompt) {
	std::cout << prompt;
	char input = '\0';
	if (!(std::cin >> input)) {
		exitRequested = true;
		return {0, 0, false};
	}

	const char normalized = static_cast<char>(std::tolower(static_cast<unsigned char>(input)));
	switch (normalized) {
		case 'w':
			return {0, -1, true};
		case 's':
			return {0, 1, true};
		case 'a':
			return {-1, 0, true};
		case 'd':
			return {1, 0, true};
		default:
			std::cout << "Неверное направление." << std::endl;
			return {0, 0, false};
	}
}

void Game::advanceNonPlayerTurns() {
	field->performAllyTurn();
	if (!field->getPlayer().isAlive()) {
		return;
	}

	field->performEnemyTurn();
	if (!field->getPlayer().isAlive()) {
		return;
	}

	field->performBuildingTurn();
	field->resolveTraps();

	if (!field->getPlayer().isAlive()) {
		return;
	}

	field->performTowerTurn();
}

void Game::handlePlayerDefeat() {
	std::cout << "Вы пали в бою." << std::endl;
	while (!exitRequested) {
		std::cout << "1. Начать заново\n2. Выход\nВыбор: ";
		int choice = 0;
		if (!(std::cin >> choice)) {
			exitRequested = true;
			return;
		}

		if (choice == 1) {
			startNewGame();
			return;
		}

		if (choice == 2) {
			exitRequested = true;
			return;
		}

		std::cout << "Введите 1 или 2." << std::endl;
	}
}

void Game::handleLevelCompletion() {
	std::cout << "Уровень " << currentLevel << " завершен!" << std::endl;
	Player& player = field->getPlayer();
	player.restoreFullHealth();
	player.getSpellHand().discardHalfRandomly();
	std::cout << "Здоровье восстановлено, половина заклинаний удалена." << std::endl;

	offerLevelUpOptions();
	PlayerState updatedState = player.createState();

	currentLevel++;
	resetGameState(currentLevel, &updatedState);
	populateLevel(*field, currentLevel);
	std::cout << "Начинается уровень " << currentLevel << "!" << std::endl;
}

void Game::populateLevel(GameField& gameField, int level) const {
	const int enemyCount = 3 + level * 2;
	const int enemyHealth = GameConstants::DEFAULT_ENEMY_HEALTH + level * 10;
	const int enemyDamage = GameConstants::DEFAULT_ENEMY_DAMAGE + level * 2;

	for (int i = 0; i < enemyCount; ++i) {
		Position spawn = gameField.findRandomFreeCell();
		if (spawn.x == gameField.getPlayerPosition().x && spawn.y == gameField.getPlayerPosition().y) {
			break;
		}
		gameField.addEnemy(spawn.x, spawn.y, enemyHealth, enemyDamage);
	}

	const int buildingCount = std::min(2 + level / 2, 5);
	for (int i = 0; i < buildingCount; ++i) {
		Position spawn = gameField.findRandomFreeCell();
		if (spawn.x == gameField.getPlayerPosition().x && spawn.y == gameField.getPlayerPosition().y) {
			break;
		}
		const int spawnTurns = std::max(1, GameConstants::DEFAULT_SPAWN_TURNS - level / 3);
		const int spawnHealth = GameConstants::DEFAULT_SPAWN_HEALTH + level * 5;
		const int spawnDamage = GameConstants::DEFAULT_SPAWN_DAMAGE + level * 2;
		gameField.addBuilding(spawn.x, spawn.y, spawnTurns, spawnHealth, spawnDamage);
	}

	const int towerCount = std::min(1 + level / 2, 4);
	for (int i = 0; i < towerCount; ++i) {
		Position spawn = gameField.findRandomFreeCell();
		if (spawn.x == gameField.getPlayerPosition().x && spawn.y == gameField.getPlayerPosition().y) {
			break;
		}
		const int range = 3 + level / 3;
		const int damage = 8 + level * 2;
		gameField.addTower(spawn.x, spawn.y, range, damage);
	}
}

void Game::offerLevelUpOptions() {
	bool upgraded = false;
	Player& player = field->getPlayer();

	while (!upgraded) {
		std::cout << "Выберите улучшение:" << std::endl;
		std::cout << "1. +20 к максимальному здоровью" << std::endl;
		std::cout << "2. +5 к ближнему урону" << std::endl;
		std::cout << "3. +5 к дальнему урону" << std::endl;
		std::cout << "4. Улучшить заклинание" << std::endl;
		std::cout << "5. Увеличить размер руки заклинаний" << std::endl;

		int choice = requestMenuChoice(1, 5, "Ваш выбор: ");
		switch (choice) {
			case 1:
				player.increaseMaxHealth(20);
				upgraded = true;
				break;
			case 2:
				player.increaseMeleeDamage(5);
				upgraded = true;
				break;
			case 3:
				player.increaseRangedDamage(5);
				upgraded = true;
				break;
			case 4:
				upgradeSpell(player);
				upgraded = true;
				break;
			case 5:
				player.getSpellHand().increaseCapacity(1);
				upgraded = true;
				break;
			default:
				break;
		}
	}
}

void Game::upgradeSpell(Player& player) {
	SpellHand& hand = player.getSpellHand();
	if (hand.getSize() == 0) {
		std::cout << "Нет заклинаний для улучшения." << std::endl;
		return;
	}

	hand.display();
	int index = requestMenuChoice(1, hand.getSize(), "Выберите заклинание для улучшения: ") - 1;
	Spell* spell = hand.getSpell(index);
	if (!spell) {
		std::cout << "Неверный выбор заклинания." << std::endl;
		return;
	}

	if (hand.replaceSpell(index, SpellFactory::createUpgraded(spell->getId()))) {
		std::cout << "Заклинание улучшено." << std::endl;
	} else {
		std::cout << "Не удалось улучшить заклинание." << std::endl;
	}
}

int Game::requestMenuChoice(int minValue, int maxValue, const std::string& prompt) {
	while (true) {
		std::cout << prompt;
		int choice = 0;
		if (!(std::cin >> choice)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Введите число." << std::endl;
			continue;
		}

		if (choice >= minValue && choice <= maxValue) {
			return choice;
		}

		std::cout << "Введите значение от " << minValue << " до " << maxValue << "." << std::endl;
	}
}

void Game::resetGameState(int level, const PlayerState* state) {
	const int widthGrowth = (level - 1) * FIELD_GROWTH_STEP;
	const int width = std::min(GameConstants::MAX_FIELD_SIZE, BASE_FIELD_WIDTH + widthGrowth);
	const int height = std::min(GameConstants::MAX_FIELD_SIZE, BASE_FIELD_HEIGHT + widthGrowth);

	auto newField = std::make_unique<GameField>(width, height);
	if (state) {
		newField->getPlayer().applyState(*state);
		newField->getPlayer().restoreFullHealth();
	}

	field = std::move(newField);
}

void Game::displaySaveSlotsStatus() const {
	std::cout << "Слоты сохранения:" << std::endl;
	for (int slot = 1; slot <= MAX_SAVE_SLOTS; ++slot) {
		const bool exists = persistence.hasSave(buildSavePath(slot));
		std::cout << slot << ". " << (exists ? "Занят" : "Пуст") << std::endl;
	}
}

std::string Game::buildSavePath(int slot) const {
	return "saves/slot" + std::to_string(slot) + ".sav";
}

bool Game::loadFromSlot(int slot) {
	const std::string path = buildSavePath(slot);
	if (!persistence.hasSave(path)) {
		std::cout << "В слоте " << slot << " нет сохранения." << std::endl;
		return false;
	}

	try {
		GameSnapshot snapshot = persistence.load(path);
		currentLevel = snapshot.level;

		auto loadedField = std::make_unique<GameField>(
			std::max(snapshot.field.width, GameConstants::MIN_FIELD_SIZE),
			std::max(snapshot.field.height, GameConstants::MIN_FIELD_SIZE));
		loadedField->restoreFromSnapshot(snapshot.field);
		field = std::move(loadedField);
		currentSaveSlot = slot;
		std::cout << "Загружено сохранение из слота " << slot
				  << ". Текущий уровень: " << currentLevel << std::endl;
		return true;
	} catch (const GameLoadException& exception) {
		std::cout << "Ошибка загрузки: " << exception.what() << std::endl;
		return false;
	}
}

int Game::requestSaveSlot(const std::string& prompt, bool allowCancel) {
	const int minValue = allowCancel ? 0 : 1;
	return requestMenuChoice(minValue, MAX_SAVE_SLOTS, prompt);
}

