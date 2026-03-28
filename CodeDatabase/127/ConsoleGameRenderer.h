#ifndef CONSOLEGAMERENDERER_H
#define CONSOLEGAMERENDERER_H

#include "GameRenderer.h"

// Реализация GameRenderer для консольной отрисовки
class ConsoleGameRenderer : public GameRenderer {
public:
	void render(const Game& game) override;
};

#endif 





