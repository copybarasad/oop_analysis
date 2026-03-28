#ifndef CHOOSE_HAND_SELECTION
#define CHOOSE_HAND_SELECTION

#include "ICommand.h"
#include "../../GameSystem//Hand.h"

class ChooseHandSelection : public ICommand
{
private:
    // Hand *mHand;
    Game *mGame;
    int mSelect;

public:
    ChooseHandSelection(Game *game, int select)
    {
        // mHand = hand;
        mGame = game;
        mSelect = select;
    }

    bool execute() override
    {
        Hand *hand = mGame->getPlayer()->getHand();
        if (mSelect < hand->getSize())
            hand->setSelection(mSelect);

        return false;
    }
};

#endif