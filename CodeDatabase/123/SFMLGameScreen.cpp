#include "SFMLGameScreen.h"
#include "../CharacterInterface.h"
#include <stdexcept>
using namespace std;

#define CELL_TEXTURE_PATH "Textures/Cell.bmp"
#define CHARACTER_TEXTURE_PATH "Textures/Characters.png"

SFMLGameScreen::SFMLGameScreen(Game *game /*, shared_ptr<IScreen> *screen*/) : SFMLScreen(), GameScreen(game)
{
    if (!mCellsTexture.loadFromFile(CELL_TEXTURE_PATH))
    {
        throw runtime_error(format("Cannot open {}", CELL_TEXTURE_PATH));
    }
    for (int i = 0; i < mCellsTexture.getSize().x / CELL_SPRITE_SIZE; i++)
    {
        mCellRect.push_back(IntRect({i * CELL_SPRITE_SIZE, 0}, {CELL_SPRITE_SIZE, CELL_SPRITE_SIZE}));
    }

    if (!mCharacterTexture.loadFromFile(CHARACTER_TEXTURE_PATH))
    {
        throw runtime_error(format("Cannot open {}", CHARACTER_TEXTURE_PATH));
    }
    for (int i = 0; i < mCharacterTexture.getSize().x / CHARACTER_SPRITE_SIZE; i++)
    {
        mCharactersRect.push_back(IntRect({i * CHARACTER_SPRITE_SIZE, 0}, {CHARACTER_SPRITE_SIZE, CHARACTER_SPRITE_SIZE}));
    }
    // mController = new SFMLGameController(mWindow, mCurrentScreen, mGame, game->getPlayer(), &mHitPos);
    // mHand.setHand(mGame->getPlayer().get()->getHand());
}

void SFMLGameScreen::render()
{
    mOrigin = Vector2f(mWindow.getWindow()->getSize().x / 2.f - mGame->getSize().x * CELL_SPRITE_SIZE / 2.f, 0.f);
    mWindow.getWindow()->clear();
    // mGame->setPosition({mWindow.getWindow()->getSize().x / 2.f - mGame->getSize().x / 2.f, 0.f});

    // Vector2f origin(mWindow.getWindow()->getSize().x / 2.f - mMap->getSize().x * CELL_SPRITE_SIZE / 2.f, 0.f);
    // mHitPos = getCellPos(Mouse::getPosition(*mWindow));

    RectangleShape sh({CELL_SPRITE_SIZE, CELL_SPRITE_SIZE});
    sh.setTexture(&mCellsTexture);

    for (int y = 0; y < mGame->getSize().y; y++)
    {
        for (int x = 0; x < mGame->getSize().x; x++)
        {
            int i = (int)mGame->getCell({x, mGame->getSize().y - y - 1}).type;
            sh.setTextureRect(mCellRect[i]);
            sh.setPosition({mOrigin.x + sh.getTextureRect().size.x * x, mOrigin.y + sh.getTextureRect().size.y * y});
            mWindow.getWindow()->draw(sh);

            if (mGame->getCell({x, mGame->getSize().y - y - 1}).ent != nullptr)
            {
                if (!mGame->getCell({x, mGame->getSize().y - y - 1}).ent->isDead())
                {
                    i = (int)mGame->getCell({x, mGame->getSize().y - y - 1}).ent->getType() - 1;

                    Sprite ent(mCharacterTexture, mCharactersRect[i]);
                    ent.setTextureRect(mCharactersRect[i]);
                    ent.setPosition(sh.getPosition());
                    ent.setScale({CELL_SPRITE_SIZE / (float)CHARACTER_SPRITE_SIZE, CELL_SPRITE_SIZE / (float)CHARACTER_SPRITE_SIZE});
                    if (sh.getGlobalBounds().contains({(float)Mouse::getPosition(*mWindow.getWindow()).x, (float)Mouse::getPosition(*mWindow.getWindow()).y}))
                    {
                        ent.setColor(Color(128, 128, 128));
                        CharacterInterface interface(static_cast<Character *>(mGame->getCell({x, mGame->getSize().y - y - 1}).ent.get()));
                        interface.setPosition({mOrigin.x + mGame->getSize().x * CELL_SPRITE_SIZE + 5, 5});
                        interface.setSize({(float)(mWindow.getWindow()->getSize().x - mGame->getSize().x * CELL_SPRITE_SIZE - mOrigin.x), 50.f});
                        interface.render(*mWindow.getWindow());
                    }
                    mWindow.getWindow()->draw(ent);
                }
                else
                {
                    mGame->getCell({x, mGame->getSize().y - y - 1}).ent = nullptr;
                }
            }
        }
    }

    CharacterInterface player(mGame->getPlayer().get());
    player.setPosition({0., 5.});
    player.setSize({mOrigin.x, 50});
    player.render(*mWindow.getWindow());

    // mHand.setPosition({player.getPosition().x, 180});
    // mHand.render(*mWindow);

    Text txt(mFont, "Level " + to_string(mGame->getLevel()));
    txt.setPosition({mOrigin.x, mGame->getSize().y * CELL_SPRITE_SIZE + 5.f});
    mWindow.getWindow()->draw(txt);

    Vector2f pos = Vector2f(player.getPosition().x, 180);
    // pos.x = 0;
    Text txt1(mFont);
    txt1.setCharacterSize(20);

    string buff = "";
    for (size_t i = 0; i < mGame->getPlayer()->getHand()->getSize(); i++)
    {
        // buff += to_string(i + 1) + ". " + mHand->getSpell(i)->getName();
        buff += format("{}. {} (lvl {})", i + 1, mGame->getPlayer()->getHand()->getSpell(i)->getName(), mGame->getPlayer()->getHand()->getSpell(i)->getLevel() + 1);
        if (i == mGame->getPlayer()->getHand()->getSelection())
            buff += " <--";
        buff += "\n";
    }

    txt1.setString(buff);
    txt1.setPosition(pos);
    mWindow.getWindow()->draw(txt1);
    mWindow.getWindow()->display();
}

Vector2i SFMLGameScreen::getCellPos(Vector2i pos)
{
    pos -= (Vector2i)mOrigin;
    for (int y = 0; y < mGame->getSize().y; y++)
    {
        for (int x = 0; x < mGame->getSize().x; x++)
        {
            IntRect buff({CELL_SPRITE_SIZE * x, CELL_SPRITE_SIZE * y}, {CELL_SPRITE_SIZE, CELL_SPRITE_SIZE});
            if (buff.contains(pos))
                return Vector2i({x, mGame->getSize().y - y - 1});
        }
    }

    return Vector2i(-1, -1);
}
