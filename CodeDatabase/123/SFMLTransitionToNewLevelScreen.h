#ifndef SFML_TRANSITION_TO_NEW_LEVEL
#define SFML_TRANSITION_TO_NEW_LEVEL

#include "TransitionToNewLevelScreen.h"
#include "SFMLScreen.h"

class SFMLTransitionToNewLevelScreen : public TransitionToNewLevelScreen, public SFMLScreen
{
private:
    Text *mTitle;
    Text *mOptions;

public:
    SFMLTransitionToNewLevelScreen(Game *game);

    void render() override;
};

#endif