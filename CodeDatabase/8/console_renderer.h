#pragma once
#include <iostream>
#include <string>
#include <vector> 
using namespace std;

class Game;
class Field;
class Player;
class Enemy;
class EnemyTower;
class Spell;
class Node;

class ConsoleRenderer {
private:
    char getSymbolForNode(const Node& node);
    
public:
    ConsoleRenderer() {} 
    void clearConsole();
    void render(const Game& game);
    void renderMessage(const std::string& message);
    void showPrompt(const std::string& prompt);

};