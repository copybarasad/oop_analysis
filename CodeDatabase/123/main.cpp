#include <iostream>
#include "InputSystem/Controllers/SFMLControllerMananger.h"
#include "Renderers/SFMLRender.h"
#include "Program.h"

int main()
{

    try
    {
        Program<SFMLRender, SFMLControllerMananger> pr;
        pr.startProgram();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}