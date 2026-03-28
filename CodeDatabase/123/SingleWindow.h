#ifndef WINDOW
#define WINDOW

#include <SFML/Graphics.hpp>
using namespace sf;

class SingleWindow
{
private:
    RenderWindow *mWindow;

    SingleWindow()
    {
        if (!mWindow)
        {
            mWindow = new RenderWindow(sf::VideoMode({1280, 900}), "Game");
        }
    }

public:
    SingleWindow(const SingleWindow &) = delete;
    SingleWindow &operator=(const SingleWindow &) = delete;

    static SingleWindow &getInstance()
    {
        static SingleWindow singletone;
        return singletone;
    }

    RenderWindow *getWindow() const
    {
        return mWindow;
    }
};

#endif