#ifndef GO_TO_TRANSITION_TO_NEW_LEVEL
#define GO_TO_TRANSITION_TO_NEW_LEVEL

#include "GoToScreen.h"
#include "../../Screens/TransitionToNewLevelScreen.h"
// #include "../../InputSystem/Controllers/LogicControllers/TransitionToNewLevelController.h"
#include<random>

template <typename T>
class GoToTransitionToNewLevel : public GoToScreen<T>
{
public:
    using GoToScreen<T>::GoToScreen;

    bool execute() override
    {
    std::random_device rseed;
    std::mt19937 rng(rseed());
    if (this->mGame->getPlayer()->getHand()->getSize() < 1)
        this->mGame->getPlayer()->getHand()->addSpell();
    else
    {
        vector<int> nums(this->mGame->getPlayer().get()->getHand()->getSize());
        for (size_t i = 0; i < nums.size(); i++)
        {
            nums[i] = i;
        }
        for (size_t i = 0; i < this->mGame->getPlayer().get()->getHand()->getSize() / 2; i++)
        {
            std::uniform_int_distribution<int> range(0, nums.size() - i - 1);
            nums.erase(nums.begin() + range(rng));
        }
        for (size_t i = 0; i < nums.size(); i++)
        {
            this->mGame->getPlayer().get()->getHand()->deleteSpell(nums[i]);
        }
    }
        *this->mScreen = nullptr;
        *this->mController = nullptr;
        *this->mScreen = this->mScreens.getTransitionToNewLevelScreen();
        // *this->mScreen = make_shared<TransitionToNewLevelScreen>(this->mGame);
        *this->mController = this->mScreens.getTransitionToNewLevelController();
        // *this->mController = make_shared<TransitionToNewLevelController<T>>(this->mScreen, this->mController, this->mGame);
        return true;
    }
};

#endif