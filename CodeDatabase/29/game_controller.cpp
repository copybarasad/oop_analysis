#include "game_controller.hpp"
#include "input.hpp"
#include "game.hpp"
#include "exceptions.hpp"
#include "event_bus.hpp"
#include "command.hpp"
#include <fstream>
#include <iostream>
#include <cctype>
#include <sstream>
#include <algorithm>

GameController::GameController(Input& input, const std::string& save_file, EventBus* bus)
	: input_(input),
	  save_manager_(save_file),
	  game_(),
	  campaign_setup_(),
	  has_campaign_setup_(false),
	  exit_requested_(false),
	  campaign_running_(false),
	  current_level_index_(0),
	  spell_power_bonus_(0),
	  controller_rng_(std::random_device{}()),
	  event_bus_(bus) {
	std::string err;
	std::ifstream keymap_file("keymap.txt");
	if (keymap_file.good()) {
		if (!command_parser_.LoadKeymap("keymap.txt", err)) {
			std::cout << "Keymap load error: " << err << ", using default controls.\n";
		}
	}
}

GameController::~GameController() = default;

void GameController::SetEventBus(EventBus* bus) noexcept {
	event_bus_ = bus;
	if (game_) {
		game_->SetEventBus(event_bus_);
	}
}

void GameController::Run() {
	exit_requested_ = false;
	while (!exit_requested_) {
		switch (ShowMainMenu()) {
			case MenuChoice::kNewGame: {
				CampaignSetup setup = RequestCampaignSetup();
				StartCampaign(setup);
				RunCampaignLoop();
				break;
			}
			case MenuChoice::kLoadGame:
				if (TryLoadFromFile()) {
					RunCampaignLoop();
				}
				break;
			case MenuChoice::kExit:
				exit_requested_ = true;
				break;
		}
	}
}

GameController::MenuChoice GameController::ShowMainMenu() {
	while (true) {
		std::cout << "\n1) Новая игра\n2) Загрузить\n3) Выход\n";
		std::string choice = NormalizeCommand(input_.ReadLine("Выбор:"));
		if (choice == "1" || choice == "NEW" || choice == "N") {
			return MenuChoice::kNewGame;
		}
		if (choice == "2" || choice == "LOAD" || choice == "L") {
			return MenuChoice::kLoadGame;
		}
		if (choice == "3" || choice == "EXIT" || choice == "E") {
			return MenuChoice::kExit;
		}
		std::cout << "Введите 1, 2 или 3.\n";
	}
}

GameController::CampaignSetup GameController::RequestCampaignSetup() {
	std::cout << "Настройка новой кампании.\n";
	CampaignSetup setup;
	setup.width = input_.ReadIntRange("Ширина поля", 10, 25);
	setup.height = input_.ReadIntRange("Высота поля", 10, 25);
	setup.wall_density = input_.ReadDoubleRange("Плотность стен", 0.0, 0.5);
	setup.player_name = input_.ReadNonEmpty("Имя героя:");
	setup.player_hp = input_.ReadIntRange("HP героя", 1, 100000);
	setup.melee_damage = input_.ReadIntRange("Урон ближнего боя", 1, 10000);
	setup.ranged_damage = input_.ReadIntRange("Урон дальнего боя", 1, 10000);
	setup.enemy_count = input_.ReadIntRange("Количество врагов", 0, setup.width * setup.height - 1);
	if (setup.enemy_count > 0) {
		setup.enemy_hp = input_.ReadIntRange("HP врагов", 1, 100000);
		setup.enemy_damage = input_.ReadIntRange("Урон врагов", 0, 10000);
	} else {
		setup.enemy_hp = 1;
		setup.enemy_damage = 0;
	}
	return setup;
}

GameController::LevelConfig GameController::BuildLevelConfig(int level_index) const {
	LevelConfig cfg;
	if (!has_campaign_setup_) return cfg;
	cfg.width = std::min(25, campaign_setup_.width + level_index - 1);
	cfg.height = std::min(25, campaign_setup_.height + (level_index - 1) / 2);
	cfg.wall_density = std::min(0.45, campaign_setup_.wall_density + 0.02 * (level_index - 1));
	cfg.wall_density = std::max(0.05, cfg.wall_density);
	cfg.enemy_count = std::max(1, campaign_setup_.enemy_count + (level_index - 1) * 2);
	cfg.enemy_hp = std::max(1, campaign_setup_.enemy_hp + (level_index - 1) * 5);
	cfg.enemy_damage = std::max(0, campaign_setup_.enemy_damage + (level_index - 1) * 2);
	return cfg;
}

void GameController::StartCampaign(const CampaignSetup& setup) {
	campaign_setup_ = setup;
	has_campaign_setup_ = true;
	current_level_index_ = 1;
	spell_power_bonus_ = 0;
	LevelConfig level = BuildLevelConfig(current_level_index_);
	unsigned seed = static_cast<unsigned>(controller_rng_());
	game_ = std::make_unique<Game>(
		level.width,
		level.height,
		level.wall_density,
		setup.player_name,
		setup.player_hp,
		setup.melee_damage,
		setup.ranged_damage,
		level.enemy_count,
		level.enemy_hp,
		level.enemy_damage,
		seed);
	game_->SetEventBus(event_bus_);
	game_->SetSpellPowerBonus(spell_power_bonus_);
	campaign_running_ = true;
}

void GameController::RunCampaignLoop() {
	if (!game_) return;
	campaign_running_ = true;
	while (campaign_running_ && !exit_requested_) {
		if (!RunSingleLevel()) break;
		HandleVictory();
	}
	campaign_running_ = false;
}

bool GameController::RunSingleLevel() {
	if (!game_) return false;
	while (!exit_requested_) {
		renderer_.Draw(*game_);
		if (!HandlePlayerPhase()) {
			if (exit_requested_) return false;
			continue;
		}
		if (game_->IsVictory()) {
			return true;
		}
		if (!HandleEnemiesPhase()) {
			return false;
		}
		if (game_->IsVictory()) {
			return true;
		}
	}
	return false;
}

bool GameController::HandlePlayerPhase() {
	if (!game_) return false;
	while (!exit_requested_) {
		std::string command = input_.ReadLine("Команда (WASD/U/L/R/V/T, номер карты, SAVE, LOAD, EXIT):");
		std::string normalized = NormalizeCommand(command);
		UserCommand parsed = command_parser_.Parse(command);
		std::string legacy = CommandToLegacyString(parsed);
		if (parsed.type == UserCommandType::Save || normalized == "SAVE") {
			SaveGame();
			continue;
		}
		if (parsed.type == UserCommandType::Load || normalized == "LOAD") {
			LoadGameDuringSession();
			continue;
		}
		if (parsed.type == UserCommandType::Exit || normalized == "EXIT") {
			exit_requested_ = true;
			return false;
		}
		if (legacy.empty() && normalized.empty()) {
			continue;
		}
		std::string to_use = legacy.empty() ? command : legacy;
		bool consumed = game_->PlayerTurn(to_use, input_);
		if (consumed) {
			return true;
		}
	}
	return false;
}

bool GameController::HandleEnemiesPhase() {
	if (!game_) return false;
	bool dead = game_->EnemiesTurn();
	if (!dead) {
		return true;
	}
	return HandleDefeat();
}

bool GameController::HandleDefeat() {
	std::cout << "Герой пал. Введите RESTART для новой игры или EXIT для выхода.\n";
	while (!exit_requested_) {
		std::string response = NormalizeCommand(input_.ReadLine("Действие:"));
		if (response == "RESTART") {
			if (has_campaign_setup_) {
				StartCampaign(campaign_setup_);
				return true;
			}
			std::cout << "Нет настроек кампании для рестарта.\n";
			return false;
		}
		if (response == "EXIT") {
			exit_requested_ = true;
			return false;
		}
		std::cout << "Введите RESTART или EXIT.\n";
	}
	return false;
}

void GameController::HandleVictory() {
	std::cout << "Уровень пройден!\n";
	ApplyPostLevelRewards();
	++current_level_index_;
	LevelConfig next = BuildLevelConfig(current_level_index_);
	unsigned seed = static_cast<unsigned>(controller_rng_());
	game_->StartLevel(next.width, next.height, next.wall_density,
	                  next.enemy_count, next.enemy_hp, next.enemy_damage, seed);
	game_->SetSpellPowerBonus(spell_power_bonus_);
}

void GameController::ApplyPostLevelRewards() {
	if (!game_) return;
	game_->PlayerRef().RestoreHealthToFull();
	game_->DiscardHalfHand(controller_rng_);
	OfferUpgrade();
}

void GameController::OfferUpgrade() {
	if (!game_) return;
	std::cout << "Выберите улучшение:\n"
	             "1) +10 к максимальному здоровью\n"
	             "2) +2 к урону ближнего боя\n"
	             "3) +2 к урону дальнего боя\n"
	             "4) Усилить заклинания\n";
	int choice = input_.ReadIntRange("Номер улучшения", 1, 4);
	switch (choice) {
		case 1:
			game_->PlayerRef().IncreaseMaxHealth(10);
			break;
		case 2:
			game_->PlayerRef().IncreaseMeleeDamage(2);
			break;
		case 3:
			game_->PlayerRef().IncreaseRangedDamage(2);
			break;
		case 4:
			++spell_power_bonus_;
			game_->SetSpellPowerBonus(spell_power_bonus_);
			break;
	}
}

void GameController::SaveGame() {
	if (!game_) {
		std::cout << "Нет активной игры для сохранения.\n";
		return;
	}
	try {
		SaveData data;
		PersistCampaignState(data);
		save_manager_.save(data);
		std::cout << "Сохранение выполнено.\n";
	} catch (const GameError& e) {
		std::cout << "Ошибка сохранения: " << e.what() << "\n";
	}
}

void GameController::LoadGameDuringSession() {
	if (TryLoadFromFile()) {
		std::cout << "Загрузка завершена.\n";
	}
}

bool GameController::TryLoadFromFile() {
	try {
		SaveData data = save_manager_.load();
		if (!data.hasActiveGame) {
			std::cout << "Нет сохранённой игры.\n";
			return false;
		}
		RestoreFromSave(data);
		return true;
	} catch (const GameError& e) {
		std::cout << "Ошибка загрузки: " << e.what() << "\n";
		return false;
	}
}

void GameController::RestoreFromSave(const SaveData& data) {
	campaign_setup_.width = data.baseWidth;
	campaign_setup_.height = data.baseHeight;
	campaign_setup_.wall_density = data.baseWallDensity;
	campaign_setup_.enemy_count = data.baseEnemyCount;
	campaign_setup_.enemy_hp = data.baseEnemyHp;
	campaign_setup_.enemy_damage = data.baseEnemyDamage;
	campaign_setup_.player_name = data.playerName;
	campaign_setup_.player_hp = data.playerMaxHealth;
	campaign_setup_.melee_damage = data.playerMeleeDamage;
	campaign_setup_.ranged_damage = data.playerRangedDamage;
	has_campaign_setup_ = true;
	current_level_index_ = data.currentLevelIndex;
	spell_power_bonus_ = data.spellPowerBonus;
	if (!data.controllerRngState.empty()) {
		std::istringstream rng_stream(data.controllerRngState);
		rng_stream >> controller_rng_;
	}
	unsigned seed = static_cast<unsigned>(controller_rng_());
	int init_enemy_count = std::max(1, data.baseEnemyCount);
	int init_enemy_hp = std::max(1, data.baseEnemyHp);
	int init_enemy_dmg = std::max(0, data.baseEnemyDamage);
	game_ = std::make_unique<Game>(
		std::max(10, data.boardWidth),
		std::max(10, data.boardHeight),
		data.wallDensity,
		data.playerName,
		data.playerMaxHealth,
		data.playerMeleeDamage,
		data.playerRangedDamage,
		init_enemy_count,
		init_enemy_hp,
		init_enemy_dmg,
		seed);
	game_->SetEventBus(event_bus_);
	game_->LoadState(data);
	game_->SetSpellPowerBonus(spell_power_bonus_);
	campaign_running_ = true;
}

void GameController::PersistCampaignState(SaveData& data) const {
	data.currentLevelIndex = current_level_index_;
	data.spellPowerBonus = spell_power_bonus_;
	data.baseWidth = campaign_setup_.width;
	data.baseHeight = campaign_setup_.height;
	data.baseWallDensity = campaign_setup_.wall_density;
	data.baseEnemyCount = campaign_setup_.enemy_count;
	data.baseEnemyHp = campaign_setup_.enemy_hp;
	data.baseEnemyDamage = campaign_setup_.enemy_damage;
	std::ostringstream rng_stream;
	rng_stream << controller_rng_;
	data.controllerRngState = rng_stream.str();
	if (game_) {
		game_->SaveState(data);
	}
}

std::string GameController::NormalizeCommand(const std::string& command) const {
	std::string result;
	for (char ch : command) {
		if (std::isspace(static_cast<unsigned char>(ch))) continue;
		result.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(ch))));
	}
	return result;
}

std::string GameController::CommandToLegacyString(const UserCommand& cmd) const {
	switch (cmd.type) {
		case UserCommandType::MoveUp: return "W";
		case UserCommandType::MoveDown: return "S";
		case UserCommandType::MoveLeft: return "A";
		case UserCommandType::MoveRight: return "D";
		case UserCommandType::AttackUp: return "U";
		case UserCommandType::AttackDown: return "V";
		case UserCommandType::AttackLeft: return "L";
		case UserCommandType::AttackRight: return "R";
		case UserCommandType::ToggleMode: return "T";
		case UserCommandType::Cast:
			if (cmd.slot > 0) {
				return std::to_string(cmd.slot) + " " + std::to_string(cmd.targetX) + " " + std::to_string(cmd.targetY);
			}
			return {};
		default:
			return {};
	}
}
