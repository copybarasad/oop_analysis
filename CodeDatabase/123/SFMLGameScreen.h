#ifndef SFML_GAME_SCREEN
#define SFML_GAME_SCREEN

#include "SFMLScreen.h"
#include "GameScreen.h"
#include "../HasFont.h"
#include "../HandInterface.h"
#include "../Renderers/SingleWindow.h"

#define CELL_SPRITE_SIZE 32
#define CHARACTER_SPRITE_SIZE 48

class SFMLGameScreen : public SFMLScreen, public GameScreen, public HasFont
{
private:
    Texture mCellsTexture;
    Texture mCharacterTexture;
    vector<IntRect> mCellRect;
    vector<IntRect> mCharactersRect;
    HandInterface mHand;
    Vector2f mOrigin;

public:
    SFMLGameScreen(Game *game);

    void render() override;

private:
    Vector2i getCellPos(Vector2i pos);
};

#endif