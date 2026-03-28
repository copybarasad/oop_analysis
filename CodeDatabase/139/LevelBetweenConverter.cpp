#include "LevelBetweenConverter.h"
#include "Commands/System/ChangeGame.h"
#include "Commands/System/ChangeState.h"


LevelBetweenConverter::LevelBetweenConverter(Game* game) : game(game) {}


Command* LevelBetweenConverter::convert(const InputEvent& event)
{
	if (event.device == InputDevice::System)
	{
		switch (event.systemEvent)
		{
		case SystemEventType::WindowClosed:	return new ChangeGame(game, ChangeGame::Change::EXITGAME);
		default:
			return nullptr;
		}
	}

	if (event.device == InputDevice::Keyboard)
	{
		switch (event.key)
		{
		case KeyCode::Num1: return new ChangeGame(game, ChangeGame::BUFF::HEALTH);
		case KeyCode::Num2: return new ChangeGame(game, ChangeGame::BUFF::DAMAGE);
		case KeyCode::Q:	return new ChangeState(game, ChangeState::State::MENU);
		default:
			return nullptr;
		}
	}

	return nullptr;
}