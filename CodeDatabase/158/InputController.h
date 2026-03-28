#pragma once

#include "Input/KeyBinder.h"
#include <iostream>
#include <cctype>

enum class Direction 
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    KNOWHERE
};

class ConsoleInputSource 
{
public:
    char getChar() const 
    {
        char c;
        std::cin >> c;
        return c;
    }
};

template<typename InputSource>
class InputController 
{
private:
    InputSource source;
    KeyBinder& binder;
    
public:
    InputController(KeyBinder& kb) : binder(kb) {}
    
    char getInput() const 
    {
        return source.getChar();
    }
    
    bool processInput(char input, Direction& dir, bool& attack, bool& switchAttack,
                     bool& openShop, bool& openUpgrade, bool& openSave,
                     bool& openLoad, bool& toggleAnim, bool& keyConfig,
                     bool& backToMenu) const 
    {
        input = toupper(input);
        
        dir = Direction::KNOWHERE;
        attack = switchAttack = openShop = openUpgrade = false;
        openSave = openLoad = toggleAnim = keyConfig = backToMenu = false;
        
        if (input == binder.getKey(KeyBinder::MOVE_UP)) 
        {
            dir = Direction::UP;
            return true;
        } 
        else if (input == binder.getKey(KeyBinder::MOVE_DOWN)) 
        {
            dir = Direction::DOWN;
            return true;
        } 
        else if (input == binder.getKey(KeyBinder::MOVE_LEFT)) 
        {
            dir = Direction::LEFT;
            return true;
        } 
        else if (input == binder.getKey(KeyBinder::MOVE_RIGHT)) 
        {
            dir = Direction::RIGHT;
            return true;
        } 
        else if (input == binder.getKey(KeyBinder::ATTACK)) 
        {
            attack = true;
            return true;
        } 
        else if (input == binder.getKey(KeyBinder::SWITCH_ATTACK)) 
        {
            switchAttack = true;
            return true;
        } 
        else if (input == binder.getKey(KeyBinder::SHOP)) 
        {
            openShop = true;
            return true;
        } 
        else if (input == binder.getKey(KeyBinder::UPGRADE)) 
        {
            openUpgrade = true;
            return true;
        } 
        else if (input == binder.getKey(KeyBinder::SAVE_MENU)) 
        {
            openSave = true;
            return true;
        } 
        else if (input == binder.getKey(KeyBinder::LOAD_MENU)) 
        {
            openLoad = true;
            return true;
        } 
        else if (input == binder.getKey(KeyBinder::TOGGLE_ANIMATIONS)) 
        {
            toggleAnim = true;
            return true;
        } 
        else if (input == binder.getKey(KeyBinder::KEY_CONFIG)) 
        {
            keyConfig = true;
            return true;
        } 
        else if (input == binder.getKey(KeyBinder::BACK_TO_MENU)) 
        {
            backToMenu = true;
            return true;
        }
        
        return false;
    }
};