#ifndef ICONTROLLER
#define ICONTROLLER

#include "../../Commands/ICommand.h"
#include "../../../Screens/Screens.h"
#include "../../../Screens/IScreen.h"
#include "../../../GameSystem/Game.h"

#include <map>

template <typename T>
class IController
{
protected:
    // const Screens mScreens;
    std::map<std::string, shared_ptr<ICommand>> mCommandMap;
    std::map<T, shared_ptr<ICommand>> mConcreteMap;
    Game *mGame;
    shared_ptr<IScreen> *mScreen;
    shared_ptr<IController<T>> *mController;
    Screens<T> mScreens;

public:
    IController(shared_ptr<IScreen> *screen, shared_ptr<IController<T>> *controller, Game *game)
    {
        mScreen = screen;
        mController = controller;
        mGame = game;
    }

    virtual void handleEvent(T event)
    {
        if (this->mConcreteMap.contains(event))
        {
            try
            {
                this->mConcreteMap[event]->execute();
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
        }
    }

    virtual void setDefaultCommands() = 0;

    void setCommands(std::map<std::string, T> &commands)
    {
        for (auto &x : mCommandMap)
        {
            mConcreteMap[commands[x.first]] = x.second;
        }
        if (mCommandMap.size() != mCommandMap.size())
            throw std::logic_error("Not all commands were scaned!!!");
    }

    void addCommand(pair<T, shared_ptr<ICommand>> command)
    {
        mConcreteMap[command.first] = command.second;
    }

    size_t getCommandsSize() const
    {
        return mCommandMap.size();
    }
};

#endif