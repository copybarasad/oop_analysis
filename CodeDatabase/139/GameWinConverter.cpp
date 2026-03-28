#include "GameWinConverter.h"
#include "Commands/System/ChangeGame.h"
#include "Commands/System/ChangeState.h"


GameWinConverter::GameWinConverter(Game* game) : game(game) {}


Command* GameWinConverter::convert(const InputEvent& event)
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
		case KeyCode::Q:	return new ChangeState(game, ChangeState::State::MENU);
		default:
			return nullptr;
		}
	}

	return nullptr;
}