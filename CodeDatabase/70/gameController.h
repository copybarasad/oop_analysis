#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "game.h"
#include "Menus/pauseMenuController.h"
#include "Menus/inputGetter.h"
#include "command.h"

template <typename InputHandler>
class GameController
{
private:
	Game* game;
	InputHandler inputHandler;
public:
	GameController(Game* game);
	void gameCycle(int actions);
	void setGame(Game* game);
};

template <typename InputHandler>
GameController<InputHandler>::GameController(Game* game)
{
	this->game = game;
}

template <typename InputHandler>
void GameController<InputHandler>::setGame(Game* game)
{
	this->game = game;
}

template <typename InputHandler>
void GameController<InputHandler>::gameCycle(int actions)
{
	while(game->gameConditions())
	{
		while(game->stageConditions())
		{
		    for(int i = 0; i < actions; ++i)
		    {
		    	std::cout << std::format("Available actions: {}.", actions - i) << std::endl;

		    	Command command = inputHandler.getCommand();
		        switch (command.getCommandType()) 
		        {
		            case COMMAND_TYPE::PAUSE:
	            	{
		            	PauseMenuController pauseMenuController;
		                if(!pauseMenuController.menuCycle<GameController>(game, this))
		                {
		                	return;
		                }
		                i--;
		            }
	                break;
		            case COMMAND_TYPE::LEFT:
		            {
		            	if (!game->setHeroPos(game->getHeroPos().first-1, game->getHeroPos().second))
		            	{
		            		i--;
		            	}
					}
					break;
		            case COMMAND_TYPE::RIGHT:
		            {
		                if (!game->setHeroPos(game->getHeroPos().first+1, game->getHeroPos().second))
		            	{
		            		i--;
		            	}
		            }
		            break;
		            case COMMAND_TYPE::UP:
		           	{
		                if (!game->setHeroPos(game->getHeroPos().first, game->getHeroPos().second+1))
		            	{
		            		i--;
		            	}
		            }
		            break;
		           	case COMMAND_TYPE::DOWN:
		           	{
		                if (!game->setHeroPos(game->getHeroPos().first, game->getHeroPos().second-1))
		            	{
		            		i--;
		            	}
		            }
		            break;
		            case COMMAND_TYPE::SHOWENEMIES:
		            	std::cout << "=== ENEMIES ===" << std::endl;
		            	game->getEnemyManager().showEnemies();
		            	std::cout << "=== BUILDINGS ===" << std::endl;
		            	game->getEnemyBuildingManager().showBuildings();
		            	std::cout << std::endl;
		            	i--;
		                break;
		            case COMMAND_TYPE::ATTACK:
		            {
		            	std::pair<int, int> point = InputGetter::getInstance().getCell(0, 0, game->getField().getWidth(), game->getField().getHeight());
		            	bool flag = game->heroAttacks(point.first, point.second);
		            	if (!flag)
		            	{
		            		i--;
		            	}
		            	if (game->getEnemyManager().empty())
				    	{
				    		i = actions;
				    	}
		            }
		            break;
		        	case COMMAND_TYPE::SWAPRANGE:
		        		game->swapHeroRange();
		                break;

		            case COMMAND_TYPE::PASS:
		                break;

		            case COMMAND_TYPE::BUYSPELL:
		            	if(!game->buySpell())
		            	{
		            		--i;
		            	}
		                break;

		            case COMMAND_TYPE::SHOWHAND:
		            	game->getHero().showHand();
		            	--i;
		                break;

		            case COMMAND_TYPE::USESPELL:
		            	if(!game->heroUseSpell())
		            	{
		            		std::cout << "Can't use spell!" << std::endl;	
		            		--i;
		            	}
		                break;
		            default:
		                std::cout << "Internal error." << std::endl;
		                i--;
		                break;
				}
			}

	        if (!game->getEnemyManager().empty())
	    	{
	    		std::cout << "Enemy turn!" << std::endl;
				game->enemyTurn();
	    	}
		}
		game->toNextStage();
	}
	std::cout << "You lose!" << std::endl;
}

#endif