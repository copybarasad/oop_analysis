#pragma once
#include <utility>


class Game;


template <typename Renderer>
class RenderController {
    Renderer renderer;
    Game* game;

public:
    RenderController(Game* game, Renderer&& renderer)
        : game(game), renderer(std::move(renderer)) {}

    Renderer& getRenderer() { return renderer; }

	void render()
	{
        if (game->isResizeRequested()) {
            renderer.resizeWindow(
                game->getDesiredWidth(),
                game->getDesiredHeight(),
                0
            );
            game->clearResizeRequest();
        }

        if (game->isThemeChangeRequested()) {
            renderer.setTheme(game->getRequestedTheme());
            game->clearThemeChangeRequest();
        }

		game->getCurrentState()->render(renderer);
	}
};
