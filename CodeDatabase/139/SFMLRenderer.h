#pragma once
#include "SFML/Graphics.hpp"
#include "Rendering/IRenderer.h"
#include "DataTypes/GameThemes.h"


class Game;

class SFMLRenderer : public IRenderer
{
private:
	Game* game;

	sf::RenderWindow* window;
	unsigned int width;
	unsigned int height;


	sf::Color backgroundColor;
	sf::Color playerColor;
	sf::Color enemyColor;
	sf::Color enemyDeadColor;
	sf::Color allyColor;
	sf::Color trapColor;
	sf::Color obstuctionCellsColor;
	sf::Color slowCellsColor;
	sf::Color towerColor;
	sf::Color towerDestroyedColor;
	sf::Color archerTowerColor;
public:
	SFMLRenderer(Game* game, GameTheme theme = GameTheme::DEFAULT);

	SFMLRenderer(const SFMLRenderer&) = delete;

	SFMLRenderer(SFMLRenderer&& other) noexcept;

	sf::RenderWindow* getWindow();

	void resizeWindow(const unsigned int newWidth, const unsigned int newHeight, const unsigned int expansion = 0);

	void renderMap() override;

	void renderPlayInfo() override;

	void renderMenu() override;

	void renderPause() override;

	void renderGameWin() override;

	void renderGameOver() override;

	void renderLevelWin() override;

	void renderLevelBetween() override;

	void setTheme(GameTheme theme) override;

	~SFMLRenderer();
private:
	void renderField() const;

	void renderPlayer() const;
};