#include "GameArea.h"
#include "Player.h"
#include "Enemy.h"
#include "Building.h"
#include "Summon.h"

void GameArea::set_human_in_cell(Coords object_coords, Human* human)
{
	area[object_coords.y][object_coords.x].set_human(human);
}

void GameArea::set_building_in_cell(Coords object_coords, Building* building)
{
	area[object_coords.y][object_coords.x].set_building(building);
}

void GameArea::fill_area(Player* player, std::vector<Enemy*> enemyes, std::vector<Building*> buildings)
{
	double count_another_cell = (((double)height * (double)width) / 100) * 10 + 1;
	for (int i = 0; i < count_another_cell; i++) {
		int x_block = rand() % width;
		int y_block = rand() % height;
		area[x_block][y_block].set_type(CellType::BLOCKED);

		int x_slow = rand() % width;
		int y_slow = rand() % height;
		area[x_slow][y_slow].set_type(CellType::SLOW);
	}

	Coords player_coords = player->get_coords();
	set_human_in_cell(player_coords, player);
	for (auto enemy : enemyes) {
		Coords enemy_coords = enemy->get_coords();
		set_human_in_cell(enemy_coords, enemy);
	}

	for (auto building : buildings) {
		Coords building_coords = building->get_coords();
		set_building_in_cell(building_coords, building);
	}
}

void GameArea::print_area()
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			std::string obj = area[y][x].is_empty();
			if (obj == "Empty") {
				std::string type_cell = area[y][x].get_type_string();
				if (type_cell == "Basic") {
					std::cout << 'O' << ' ';
				}
				else if (type_cell == "Blocked") {
					std::cout << '#' << ' ';
				}
				else if (type_cell == "Slow") {
					std::cout << '*' << ' ';
				}
			}
			else {
				if (obj == "Player") {
					std::cout << 'P' << ' ';
				}
				else if (obj == "Summon") {
					std::cout << 'S' << ' ';
				}
				else if (obj == "Enemy") {
					std::cout << 'E' << ' ';
				}
				else if (obj == "Tower") {
					std::cout << 'T' << ' ';
				}
				else if (obj == "EnemyBuilding") {
					std::cout << 'B' << ' ';
				}
			}
		}
		std::cout << '\n';
	}
}

std::vector<std::string> GameArea::to_lines()
{
	std::vector<std::string> lines;
	lines.reserve(height);

	for (int y = 0; y < height; ++y) {
		std::string line;
		line.reserve(static_cast<size_t>(width) * 2); 

		for (int x = 0; x < width; ++x) {
			char ch = '?';

			std::string obj = area[y][x].is_empty();
			if (obj == "Empty") {
				std::string type_cell = area[y][x].get_type_string();
				if (type_cell == "Basic") {
					ch = 'O';
				}
				else if (type_cell == "Blocked") {
					ch = '#';
				}
				else if (type_cell == "Slow") {
					ch = '*';
				}
				else {
					ch = 'O';
				}
			}
			else {
				if (obj == "Player") {
					ch = 'P';
				}
				else if (obj == "Summon") {
					ch = 'S';
				}
				else if (obj == "Enemy") {
					ch = 'E';
				}
				else if (obj == "Tower") {
					ch = 'T';
				}
				else if (obj == "EnemyBuilding") {
					ch = 'B';
				}
				else {
					ch = '?';
				}
			}

			line.push_back(ch);
			if (x + 1 < width) line.push_back(' ');
		}

		lines.push_back(std::move(line));
	}

	return lines;
}

void GameArea::move_human(Coords from, Coords to, Human* human)
{

	if (human->stunned_or_not()) {
		human->set_stunned(false);
		std::cout << "You are stanned.\n";
		return;
	}


	if (!player_or_enemy(from) && area[to.y][to.x].get_trap()) {

		human->take_damage(area[to.y][to.x].get_trap()->get_damage());
		if (human->get_health() < 0) {
			area[from.y][from.x].clear();
		}
		area[to.y][to.x].clear_trap();
		return;
	}

	area[from.y][from.x].clear();
	area[to.y][to.x].set_human(human);



	if (area[to.y][to.x].get_type() == CellType::SLOW) {
		human->set_stunned(true);
	}
}

bool GameArea::player_or_enemy(Coords coords)
{
	return area[coords.y][coords.x].player_or_enemy();
}

bool GameArea::apply_damage(Coords target, double damage, Coords attacker_coords, bool is_melee) {

	Object* enemy = area[target.y][target.x].get_object();
	if (!enemy) return false;

	double exp = get_coast_exp_enemy(target);
	enemy->take_damage(damage);
	double enemy_health = enemy->get_health();
	
	Object* attacker = area[attacker_coords.y][attacker_coords.x].get_object();

	if (enemy_health <= 0) {
		if (attacker && dynamic_cast<Player*>(attacker)) {
			Player* player_ptr = static_cast<Player*>(attacker);
			player_ptr->up_exp(exp);
		}
		area[target.y][target.x].clear();

		if (attacker && is_melee) {
			Coords old_coords = attacker->get_coords();
			move_human(old_coords, target, static_cast<Human*>(attacker));
		}
		return true;
	}
	return false;
}

std::string GameArea::can_move_to(Coords move_coords)
{
	if (!is_valid_coords(move_coords) || !is_valid_coords(move_coords)) {
		return std::string("Edge");
	}
	else {
		return area[move_coords.y][move_coords.x].is_empty();
	}
}

CellType GameArea::get_cell_type(Coords coords)
{
	return area[coords.y][coords.x].get_type();
}

double GameArea::get_coast_exp_enemy(Coords coords)
{
	Object* enemy = area[coords.y][coords.x].get_object();
	return enemy->get_health() / 2;
}

bool GameArea::is_valid_coords(Coords& coord) {
	return coord.x >= 0 && coord.x < width && coord.y >= 0 && coord.y < height;
}

std::vector<Coords> GameArea::find_in_radius(Coords center, int radius, const std::vector<std::string>& allowed_types) {
	std::vector<Coords> targets;
	for (int y = center.y - radius; y <= center.y + radius; ++y) {
		for (int x = center.x - radius; x <= center.x + radius; ++x) {
			Coords candidate(x, y);
			
			if (!is_valid_coords(candidate) || area[y][x].get_type() == CellType::BLOCKED) {
				continue;
			}
			std::string cell_content = can_move_to(candidate);
			if (std::find(allowed_types.begin(), allowed_types.end(), cell_content) != allowed_types.end()) {
				targets.push_back(candidate);
			}
		}
	}

	return targets;
}

void GameArea::set_trap(Trap* trap) 
{
	Coords crd = trap->get_coords();
	area[crd.y][crd.x].set_trap(trap);
}

std::string GameArea::get_type(Coords crd)
{
	return area[crd.y][crd.x].is_empty();
}

void GameArea::add_ally(double health, double damage, Coords crd)
{
	Summon* ally = new Summon(health, damage, crd);
	area[crd.y][crd.x].set_human(ally);
}

std::vector<std::vector<int>> GameArea::serialize_cell_types() {
	std::vector<std::vector<int>> types(height, std::vector<int>(width));

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			types[y][x] = static_cast<int>(area[y][x].get_type());
		}
	}

	return types;
}

void GameArea::deserialize_cell_types(std::vector<std::vector<int>>& types) {
	for (int y = 0; y < height && y < types.size(); ++y) {
		for (int x = 0; x < width && x < types[y].size(); ++x) {
			area[y][x].set_type(static_cast<CellType>(types[y][x]));
		}
	}
}

std::vector<Summon*> GameArea::find_allies() {
	std::vector<Summon*> allies;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (area[y][x].is_empty() == "Summon") {
				Human* h = area[y][x].get_human();
				if (auto ally = dynamic_cast<Summon*>(h)) {
					allies.push_back(ally);
				}
			}
		}
	}
	return allies;
}

