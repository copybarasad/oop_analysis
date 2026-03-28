#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include <memory>
#include <string>
#include <random>
#include "save_manager.hpp"
#include "renderer.hpp"
#include "command_parser.hpp"

class Input;
class Game;
class EventBus;

class GameController {
public:
	GameController(Input& input, const std::string& save_file, EventBus* bus = nullptr);
	~GameController();
	void Run();
	void SetEventBus(EventBus* bus) noexcept;

private:
	struct CampaignSetup {
		int width{0};
		int height{0};
		double wall_density{0.0};
		std::string player_name;
		int player_hp{0};
		int melee_damage{0};
		int ranged_damage{0};
		int enemy_count{0};
		int enemy_hp{0};
		int enemy_damage{0};
	};

	struct LevelConfig {
		int width{0};
		int height{0};
		double wall_density{0.0};
		int enemy_count{0};
		int enemy_hp{0};
		int enemy_damage{0};
	};

	enum class MenuChoice { kNewGame, kLoadGame, kExit };

	MenuChoice ShowMainMenu();
	CampaignSetup RequestCampaignSetup();
	LevelConfig BuildLevelConfig(int level_index) const;
	void StartCampaign(const CampaignSetup& setup);
	void RunCampaignLoop();
	bool RunSingleLevel();
	bool HandlePlayerPhase();
	bool HandleEnemiesPhase();
	bool HandleDefeat();
	void HandleVictory();
	void ApplyPostLevelRewards();
	void OfferUpgrade();
	void SaveGame();
	void LoadGameDuringSession();
	bool TryLoadFromFile();
	void RestoreFromSave(const SaveData& data);
	void PersistCampaignState(SaveData& data) const;
	std::string NormalizeCommand(const std::string& command) const;

	Input& input_;
	SaveManager save_manager_;
	std::unique_ptr<Game> game_;
	CampaignSetup campaign_setup_;
	bool has_campaign_setup_;
	bool exit_requested_;
	bool campaign_running_;
	int current_level_index_;
	int spell_power_bonus_;
	std::mt19937 controller_rng_;
	EventBus* event_bus_;
	TerminalRenderer renderer_;
	CommandParser command_parser_;
	std::string CommandToLegacyString(const UserCommand& cmd) const;
};

#endif
