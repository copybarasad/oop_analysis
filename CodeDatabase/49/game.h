#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <cstddef>
#include <termios.h>
#include <unistd.h>
#include "../map/game_map.h"
#include "../save/save_manager.h"
#include "../render/console_renderer.h"

class Game
{
private:
	GameMap map;
	std::size_t width;
	std::size_t height;
	int current_level;
	bool is_running = true;

	const std::string SAVE_PATH = "save/savegame.dat";
	
	static constexpr int DELAY_MS = 1000;
	static constexpr int GAME_DELAY_MS = 75;
	static constexpr int INVALID_COMMAND_DELAY_MS = 500;
	static constexpr int LEVEL_TO_FINISH = 3;

	void block_input_for_seconds(int milliseconds);

	template <typename Visualizer>
	void process_enemies_turn(Visualizer& visualizer);
	
	template <typename Visualizer>
	void process_buildings_and_tower(Visualizer& visualizer);

	bool ask_restart() const;
	void reset();
	void next_level();
	void level_up_menu(Player* player);

public:
	explicit Game(std::size_t width, std::size_t height);

	int get_current_level() const { return current_level; }
	const GameMap& get_map() const { return map; }
	GameMap& get_map_mutable() { return map; }
	bool is_game_running() const { return is_running; }

	void stop();
	bool confirm_exit() const;
	void save_to_file();
	void load_from_file();

	template <typename Controller, typename Visualizer>
	bool run(Controller& controller, Visualizer& visualizer);

	void save_binary(std::ostream& os) const;
	void load_binary(std::istream& is);
};

template <typename Visualizer>
void Game::process_enemies_turn(Visualizer& visualizer)
{
	for (std::size_t i = 0; i < map.get_enemies().size(); ++i)
	{
		if (map.get_enemies()[i].is_alive())
		{
			map.get_enemies_mutable()[i].move(map, i);
			block_input_for_seconds(GAME_DELAY_MS);
			visualizer.render();
		}
	}
	map.remove_dead_enemies();
}

template <typename Visualizer>
void Game::process_buildings_and_tower(Visualizer& visualizer)
{
	if (map.get_enemy_building() && map.get_enemy_building()->is_alive())
	{
		map.get_enemy_building()->update();
		if (map.get_enemy_building()->can_spawn())
		{
			if (map.get_enemy_building()->spawn_enemy_near_building(map))
			{
				map.get_enemy_building()->reset_cooldown();
			}
		}
	}

	block_input_for_seconds(GAME_DELAY_MS);
	visualizer.render();

	if (map.get_enemy_tower())
	{
		map.get_enemy_tower()->update();
		if (map.get_enemy_tower()->is_player_in_range(map.get_player()) &&
			map.get_enemy_tower()->can_attack())
		{
			map.get_enemy_tower()->attack_player(map.get_player(), map);
		}
	}

	block_input_for_seconds(GAME_DELAY_MS);
	visualizer.render();
}

template <typename Controller, typename Visualizer>
bool Game::run(Controller& controller, Visualizer& visualizer)
{
	while (is_running)
	{
		while (map.get_player()->is_alive() &&
		       (map.get_enemy_building()->is_alive() || map.get_enemies().size() > 0))
		{
			block_input_for_seconds(GAME_DELAY_MS);
			visualizer.render();

			map.get_player()->update();
			for (std::size_t i = 0; i < map.get_player()->get_spell_hand().get_spell_count(); ++i)
			{
				map.get_player()->update_spell_cooldown(
					map.get_player()->get_spell_hand().get_spell(i));
			}

			if (!map.get_player()->can_move())
			{
				std::cout << "\nPlayer on slow trap. Press ENTER to skip turn: ";
				std::cin.get();
			}
			else
			{
				controller.process_player_turn(visualizer);
			}

			block_input_for_seconds(GAME_DELAY_MS);
			visualizer.render();

			process_enemies_turn(visualizer);
			process_buildings_and_tower(visualizer);
		}

		visualizer.render();

		if (map.get_player()->is_alive() && current_level != LEVEL_TO_FINISH)
		{
			std::cout << "\n\033[1;32mVICTORY!\033[0m  Score: "
					<< map.get_player()->get_score() << "\n";

			std::cout << "\nProceeding to next level...\n";
			block_input_for_seconds(DELAY_MS);
			next_level();
			continue;
		}
		else if (map.get_player()->is_alive() && current_level == LEVEL_TO_FINISH)
		{
			visualizer.clear();
			std::cout << "\n\033[1;33mCONGRATULATIONS!\033[0m You have conquered all levels and finished the game!\n";
			std::cout << "Your final score: " << map.get_player()->get_score() << "\n";
			std::cout << "Thank you for playing!\n";
			is_running = false;
		}
		else
		{
			std::cout << "\n\033[1;31mGAME OVER!\033[0m You died.\n";
			block_input_for_seconds(DELAY_MS);
			if (ask_restart())
			{
				reset();
				continue;
			}
			is_running = false;
		}
	}
	return false;
}

#endif