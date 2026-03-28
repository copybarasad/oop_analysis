#include "Interfaces/Game.h"
#include "Infrastructure/SFMLReader.h"
#include "Rendering/SFMLRenderer.h"
#include "Infrastructure/GameEngine.h"
#include "Windows.h"


int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	Game game(15, 15, 30);
	game.run();
    GameEngine<SFMLRenderer, SFMLReader> engine(
		&game,
		[](Game* game) {
			return SFMLRenderer(game);
		},
		[](Game* game, SFMLRenderer& renderer)
		{
			return SFMLReader(renderer.getWindow());
		}
	);
    engine.start();
	return 0;
}