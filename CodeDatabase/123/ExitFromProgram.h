#ifndef EXIT_FROM_PROGRAM
#define EXIT_FROM_PROGRAM

#include "SFML/Graphics.hpp"
#include "ICommand.h"
#include "../../Renderers/SingleWindow.h"

template <typename T>
class ExitFromProgram : public ICommand
{
    // ExitFromSFMLProgram() { mWindow = win; }

    bool execute() override
    {
        return true;
    }
};

class ExitFromSFMLProgram : public ExitFromProgram<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>
{
public:
    bool execute() override
    {
        SingleWindow::getInstance().getWindow()->close();
        return false;
    }
};

#endif