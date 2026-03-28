#pragma once
#include "SFML/Graphics.hpp"
#include "DataTypes/InputEvent.h"


class SFMLReader
{
	sf::RenderWindow* window;

public:
	SFMLReader(sf::RenderWindow* window);
	std::optional<InputEvent> getInputEvent();
private:
	KeyCode convertSFMLKey(const sf::Event::KeyPressed* key);
	MouseButton convertSFMLMouseButton(const sf::Event::MouseButtonPressed* mouse);
};