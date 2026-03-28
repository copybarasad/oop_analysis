#ifndef CLI_RENDER_HPP
#define CLI_RENDER_HPP

#include <iostream>
#include <memory>
#include <vector>

enum class Symbols : char {
    Player = 'P',
    Enemy  = 'E',
    Grass  = '*',
    Wall   = '#',
    Trap   = '^',
    Tower  = 'T'
};  

class eManager;

class CLIRenderer {
public:
    void draw(eManager& eManager);

    void renderMainMenu();
    void renderPauseMenu();
    void renderSaveMenu(const std::vector<std::string>& saves);
    void renderSaveNamePrompt();
    void renderSaveSuccess();
    
    void renderNextLevel(int level);
    
    void renderMessage(const std::string& message);
    void renderError(const std::string& error);
};  

#endif