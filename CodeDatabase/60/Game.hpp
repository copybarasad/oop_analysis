#pragma once
#include <Field.hpp>
#include <ScreenSize.hpp>
#include <Menu.hpp>

class Game {
    ScreenSize screen;
    Field field;
    Menu menu;
    
public:
    Game();
    bool help();
    void start();
    void initScreen();
    void cleanup();
};