#ifndef SFML_PAUSE_SCREEN
#define SFML_PAUSE_SCREEN

#include "PauseScreen.h"
#include "SFMLScreen.h"

class SFMLPauseScreen : public PauseScreen, public SFMLScreen
{
private:
    Text *mTitle;
    Text *mOptions;

public:
    SFMLPauseScreen(/*shared_ptr<IScreen> *screen*/);
    // {
    //     mController = new SFMLPauseController();
    // }

    void render() override;

    ~SFMLPauseScreen();
};

#endif