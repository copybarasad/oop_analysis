#include "Game.h"
#include "ConsoleInputer.h"

int main()
{
	auto templates = std::vector<LevelTemplate>{
		LevelTemplate(10, 10, 200, {0, 0}, {3, 7}, {5, 5}, TOWER_DEBUFF, TOWER_HEALTH, STANDART_SPELL_RADIUS, SPAWNER_RADIUS, \
			SPAWNER_HEALTH, SPAWNER_COOLDOWN, 0,
			std::map<TypeOfCell, std::vector<std::pair<int, int>>>
			{
				{ IMPASSABLE, { {2, 2}, {2, 3}, {4, 4}, {4, 5} } },
				{ SLOWING, { {3, 4}, {4, 7} } }
			}
		),
		LevelTemplate(15, 10, 250, {0, 0}, {6, 7}, {10, 5}, TOWER_DEBUFF - 3, TOWER_HEALTH + 5, STANDART_SPELL_RADIUS, SPAWNER_RADIUS, \
			SPAWNER_HEALTH + 5, SPAWNER_COOLDOWN, 3,
			std::map<TypeOfCell, std::vector<std::pair<int, int>>>
			{
				{ IMPASSABLE, { {1, 0}, {1, 1}, {6, 6}, {5, 6}, {5, 7}, {10, 4}, {9, 4}, {9, 5} } },
				{ SLOWING, { {3, 4}, {4, 7} } }
			}
		),
		LevelTemplate(15, 15, 300, {14, 14}, {11, 7}, {9, 9}, TOWER_DEBUFF - 6, TOWER_HEALTH + 10, STANDART_SPELL_RADIUS + 1, SPAWNER_RADIUS + 1, \
			SPAWNER_HEALTH + 10, SPAWNER_COOLDOWN, 6,
			std::map<TypeOfCell, std::vector<std::pair<int, int>>>
			{
				{ IMPASSABLE, { {13, 14}, {13, 13}, {8, 8}, {9, 8}, {9, 7}, {4, 10}, {5, 10}, {5, 9} } },
				{ SLOWING, { {11, 10}, {10, 7}, {10, 9}, {4, 3} } }
			}
		),
		LevelTemplate(15, 20, 500, {0, 19}, {8, 10}, {14, 0}, TOWER_DEBUFF - 9, TOWER_HEALTH + 15, STANDART_SPELL_RADIUS + 2, SPAWNER_RADIUS + 1, \
			SPAWNER_HEALTH + 15, SPAWNER_COOLDOWN, 9,
			std::map<TypeOfCell, std::vector<std::pair<int, int>>>
			{
				{ IMPASSABLE, { {1, 19}, { 1, 18 }, { 1, 17 }, { 7, 13 }, { 7, 12 }, { 7, 11 }, { 7, 10 }, { 7, 9 }, { 8, 9 }, { 9, 9 }, { 9, 11 }, { 13, 0 }, { 13, 1 }, { 13, 2 }, { 13, 3 } } },
				{ SLOWING, { {1, 16}, {10, 9}, {10, 8}, {9, 10}, {14, 3}, {12, 0} } }
			}
		),
		LevelTemplate(25, 20, 1000, {24, 0}, {4, 4}, {0, 19}, TOWER_DEBUFF - 15, TOWER_HEALTH + 20, STANDART_SPELL_RADIUS + 3, SPAWNER_RADIUS + 3, \
			SPAWNER_HEALTH + 20, SPAWNER_COOLDOWN, 15,
			std::map<TypeOfCell, std::vector<std::pair<int, int>>>
			{
				{ IMPASSABLE, { {23, 0}, { 23, 1 }, { 24, 3 }, { 23, 3 }, { 2, 6 }, { 3, 6 }, { 4, 6 }, { 5, 6 }, { 6, 6 }, { 6, 5 }, { 6, 4 }, \
				{ 6, 3 }, { 6, 2 }, { 6, 1 }, { 6, 0 }, { 11, 11 }, { 11, 12 }, { 11, 13 }, { 4, 0 }, { 4, 1 }, { 4, 2 } } },
			
				{ SLOWING, { {24, 2}, {1, 6}, {5, 4}, {5, 3}, {4, 3}, {3, 3}, {3, 4}, {3, 5}, {4, 5}, {5, 5} } }

			}
		)
	};

	Game game = Game<ConsoleInputer, ConsoleRenderer>(templates);
	game.Run();
	return 0;
}