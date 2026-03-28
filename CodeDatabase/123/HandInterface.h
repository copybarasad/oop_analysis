#ifndef HAND_INTERFACE
#define HAND_INTERFACE

#include "GameSystem/Hand.h"
#include "HasFont.h"

class HandInterface : public HasFont
{
private:
    Vector2f mPos;
    Hand *mHand;

public:
    // HandInterface();

    void setHand(Hand *h);

    void setPosition(Vector2f pos);

    Vector2f getPosition() const;

    void render(RenderWindow &window);
};

#endif