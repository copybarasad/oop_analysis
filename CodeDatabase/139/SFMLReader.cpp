#include "SFMLReader.h"


SFMLReader::SFMLReader(sf::RenderWindow* window) 
{
	this->window = window;
}


KeyCode SFMLReader::convertSFMLKey(const sf::Event::KeyPressed* key) {
	switch (key->scancode) {
	case sf::Keyboard::Scancode::Num1:    return KeyCode::Num1;
	case sf::Keyboard::Scancode::Num2:    return KeyCode::Num2;
	case sf::Keyboard::Scancode::Num3:    return KeyCode::Num3;
	case sf::Keyboard::Scancode::Num4:    return KeyCode::Num4;
	case sf::Keyboard::Scancode::Num5:    return KeyCode::Num5;

	case sf::Keyboard::Scancode::LShift:  return KeyCode::LShift;

	case sf::Keyboard::Scancode::Left:    return KeyCode::Left;
	case sf::Keyboard::Scancode::Right:   return KeyCode::Right;
	case sf::Keyboard::Scancode::Up:      return KeyCode::Up;
	case sf::Keyboard::Scancode::Down:    return KeyCode::Down;

	case sf::Keyboard::Scancode::L:       return KeyCode::L;
	case sf::Keyboard::Scancode::Z:       return KeyCode::Z;
	case sf::Keyboard::Scancode::X:       return KeyCode::X;
	case sf::Keyboard::Scancode::C:       return KeyCode::C;
	case sf::Keyboard::Scancode::V:       return KeyCode::V;
	case sf::Keyboard::Scancode::B:       return KeyCode::B;
	case sf::Keyboard::Scancode::M:       return KeyCode::M;

	case sf::Keyboard::Scancode::W:       return KeyCode::W;
	case sf::Keyboard::Scancode::A:       return KeyCode::A;
	case sf::Keyboard::Scancode::S:       return KeyCode::S;
	case sf::Keyboard::Scancode::D:       return KeyCode::D;

	case sf::Keyboard::Scancode::E:       return KeyCode::E;
	case sf::Keyboard::Scancode::H:       return KeyCode::J;
	case sf::Keyboard::Scancode::J:       return KeyCode::J;
	case sf::Keyboard::Scancode::K:       return KeyCode::K;

	case sf::Keyboard::Scancode::R:       return KeyCode::R;
	case sf::Keyboard::Scancode::Q:       return KeyCode::Q;
	case sf::Keyboard::Scancode::Escape:  return KeyCode::ESC;

	default:                              return KeyCode::Unknown;

	}
}


MouseButton SFMLReader::convertSFMLMouseButton(const sf::Event::MouseButtonPressed* mouse) {
	switch (mouse->button) {
	case sf::Mouse::Button::Left: return MouseButton::Left;
	case sf::Mouse::Button::Right: return MouseButton::Right;
	default: return MouseButton::Unknown;
	}
}


std::optional<InputEvent> SFMLReader::getInputEvent()
{
	InputEvent result{};
	while (const std::optional event = (this->window)->pollEvent()) 
	{	
		if (event->is<sf::Event::Closed>())
		{
			result.device = InputDevice::System;
			result.systemEvent = SystemEventType::WindowClosed;
			return result;
		}


		if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>())
		{
			result.device = InputDevice::Keyboard;
			result.key = convertSFMLKey(keyPress);
			result.pressed = true;
			return result;
		}

		if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>())
		{
			result.device = InputDevice::Mouse;
			result.mouseButton = convertSFMLMouseButton(mousePress);
			result.x = sf::Mouse::getPosition(*(this->window)).x;
			result.y = sf::Mouse::getPosition(*(this->window)).y;
			result.pressed = true;
			return result;
		}
	}

	return std::nullopt;
}