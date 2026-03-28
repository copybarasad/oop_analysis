#include "MenuConverter.h"
#include "Commands/System/ChangeGame.h"


MenuConverter::MenuConverter(Game* game) : game(game) {}


Command* MenuConverter::convert(const InputEvent& event)
{
	if (event.device == InputDevice::System)
	{
		switch (event.systemEvent)
		{
		case SystemEventType::WindowClosed:	return new ChangeGame(game, ChangeGame::Change::EXITGAME);;
		default:
			return nullptr;
		}
	}

	if (event.device == InputDevice::Keyboard)
	{
		switch (event.key)
		{
		case KeyCode::Num1: return new ChangeGame(game, ChangeGame::Change::RESETLEVEL);
		case KeyCode::Num2:	return new ChangeGame(game, ChangeGame::Change::CONTINUEGAME);
		case KeyCode::Num3: return new ChangeGame(game, ChangeGame::Change::LOADGAME);
		case KeyCode::Q:	return new ChangeGame(game, ChangeGame::Change::EXITGAME);
		default:
			return nullptr;
		}
	}

	return nullptr;
}