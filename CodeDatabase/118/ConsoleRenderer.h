#ifndef CONSOLE_RENDERER
#define CONSOLE_RENDERER

#include "GameLogic.h"

#include <cstdlib>
#include <format>

class ConsoleRenderer
{
public:
	void DrawStartMenu();
	void DrawMainWindow(const int, const std::shared_ptr<GameLogic>&);
	void DrawSpellShopMenu();
	void DrawLoseLevelMenu(const std::string&, const std::string&);
	void DrawEndGame();
	void DrawMessage(const std::string);
	void Update();
};

#endif

