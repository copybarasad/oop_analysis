#include "CLIVisualizer.h"

CLIVisualizer::CLIVisualizer(){};

void CLIVisualizer::showField(const Game& game)
{
	
	std::pair<int, int> heroPos = game.getHeroPos();
	std::cout << std::endl;
	for(int j = game.getField().getHeight() - 1; j >= 0; --j)
	{
		if(j>=10)
		{
			std::cout << j << "  ";	
		} else
		{
			std::cout << j << "   ";
		}
		for(int i = 0; i < game.getField().getWidth(); ++i)
		{
			if(i >= 10)
			{
				std::cout << " ";
			}
			if(i == heroPos.first and j == heroPos.second)
			{
				std::cout << "H" << " ";	
			} else if (game.getEnemyManager().isEnemyInPos(i, j)) 
			{
				std::cout << "E" << " ";	
			} else if (game.getEnemyBuildingManager().isBuildingInPos(i, j)) 
			{
				std::cout << "B" << " ";	
			} else if (game.getField().isCellTrapped(i, j))
			{
				std::cout << "T" << " ";
			} else
			{	
				std::cout << (int)(!game.getField().isCellPassable(i, j)) << " ";
			}
		}
		std::cout << std::endl;

	}
	std::cout << std::endl << "    ";
	for(int i = 0; i < game.getField().getWidth(); ++i)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl << std::endl;
}

void CLIVisualizer::showHeroStats(const Hero& hero)
{
	std::cout << std::format("Health: {}. Damage: {}. Your range: {}. Points: {}.", hero.getHealth(), hero.getDamage(), hero.getRange(), hero.getPoints()) << std::endl;
}

void CLIVisualizer::showStage(int stage)
{
	std::cout << "Stage: " << stage << "/10" << std::endl;
}

void CLIVisualizer::updateView(const Game& game)
{
	system("cls");
	showField(game);
	showStage(game.getStage());
	showHeroStats(game.getHero());
}