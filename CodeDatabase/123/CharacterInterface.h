#ifndef ENTITY_INTERFACE
#define ENTITY_INTERFACE

#include "GameSystem/Player.h"
#include "HasFont.h"
#include "SFML/Graphics.hpp"

using namespace sf;

class CharacterInterface : public HasFont
{
private:
    static Texture mCharacterTexture;
    static bool mIsDataLoad;
    // static Font mFont;
    Character *mCharacter;
    Vector2f mPos;
    Vector2f mSize;

public:
    CharacterInterface(Character *ent);

    // CharacterInterface(shared_ptr<Entity> ent);

    void setTexture(Texture &texture);

    void setPosition(Vector2f pos);

    Vector2f getPosition() const;

    void setSize(Vector2f size);

    Vector2f getSize() const;

    void render(RenderWindow &window);
};

#endif