#pragma once
#include "GameController.h"
#include "RenderController.h"
#include "Interfaces/Game.h"
#include <functional>


template <typename Renderer, typename Reader>
class GameEngine
{
	RenderController<Renderer> renderController;
	GameController<Reader> gameController;
	Game* game;
public:
	using RendererFactory = std::function<Renderer(Game*)>;
	using ReaderFactory = std::function<Reader(Game*, Renderer&)>;

	GameEngine(Game* game, RendererFactory rendererFactory, ReaderFactory readerFactory)
		: game(game), 
		renderController(game, rendererFactory(game)), 
		gameController(game, readerFactory(game, renderController.getRenderer())) { }

	void start()
	{
		while (game->running())
		{
			gameController.handle();
			game->getCurrentState()->update();
			renderController.render();
		}
	}
};