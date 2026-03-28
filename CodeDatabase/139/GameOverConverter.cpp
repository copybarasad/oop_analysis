#include "GameOverConverter.h"
#include "Commands/System/ChangeGame.h"
#include "Commands/System/ChangeState.h"


GameOverConverter::GameOverConverter(Game* game) : game(game) {}


Command* GameOverConverter::convert(const InputEvent& event)
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
		case KeyCode::R:	return new ChangeGame(game, ChangeGame::Change::RESTARTLEVEL);
		case KeyCode::Q:	return new ChangeState(game, ChangeState::State::MENU);
		default:
			return nullptr;
		}
	}

	return nullptr;
}