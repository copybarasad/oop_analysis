//#pragma once
//#include <SFML/Graphics.hpp>
//#include "UseCases/ExecuteUseCase.h"
//#include "Infrastructure/Renderer.h"
//
//class Game;
//
//
//class InputControler
//{
//private:
//	Game* game;
//	bool moveEnemies = false;
//public:
//	InputControler(Game* game);
//	void handlePlayState(sf::RenderWindow& window);
//	void handleMenuState(sf::RenderWindow& window);
//	void handlePauseState(sf::RenderWindow& window);
//	void handleLevelWinState(sf::RenderWindow& window);
//	void handleLevelBetweenState(sf::RenderWindow& window);
//	void handleGameWinState(sf::RenderWindow& window);
//	void handleGameOverState(sf::RenderWindow& window);
//
//private:
//	void handleMove( sf::Keyboard::Scancode keyCode);
//	void handleAttack(sf::Keyboard::Scancode keyCode);
//	void handleSpellsButton(sf::Keyboard::Scancode keyCode);
//	void handleSpellsMouse(sf::RenderWindow& window, sf::Mouse::Button mouseButton);
//};