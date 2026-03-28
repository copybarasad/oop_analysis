#include <iostream>
#include "enemy.h"
#include "player.h"
#include "field.h"
#include "node.h"
using namespace std;
Enemy::Enemy(string name, Field& field, int startX, int startY) 
    : Character(name, field, startX, startY), damagePower(1) {}

void Enemy::setDamage(int newDamage) { damagePower = newDamage; }
int Enemy::getDamage() const { return damagePower; }
void Enemy::setName(std::string newname) {name = newname;}
bool Enemy::moveTo(int dx, int dy, Player& player, std::string& message) {
    int playerX = player.getX();
    int playerY = player.getY();
    
    if (abs(playerX - x) <= 1 && abs(playerY - y) <= 1) {
        player.takeDamage(damagePower);
        message = getname() + " атаковал игрока! Урон: " + to_string(player.getDamage()) + " Осталось жизней: " + to_string(player.getLives());
        return true;
    }
    
    vector<pair<int, int>> preferredDirections; 
    if (playerX > x) preferredDirections.push_back({1, 0});
    else if (playerX < x) preferredDirections.push_back({-1, 0});
    if (playerY > y) preferredDirections.push_back({0, 1});
    else if (playerY < y) preferredDirections.push_back({0, -1});
    
    for (pair<int, int> dir : preferredDirections) {
        int testX = x + dir.first;
        int testY = y + dir.second;
        if (field.validpos(testX, testY) && field.getNode(testX, testY).isPassable()) {
            if (field.getNode(testX, testY).hasTrap()) {
                int trapDamage = field.getNode(testX, testY).triggerTrap();
                this->takeDamage(trapDamage);
                field.getNode(x, y).removeEntity();
                x = testX;
                y = testY;
                field.getNode(x, y).setEnemy();
                message = getname() + " наступил на ловушку! Урон: " + to_string(trapDamage);
                return true;
            }
            field.getNode(x, y).removeEntity();
            x = testX;
            y = testY;
            field.getNode(x, y).setEnemy();
            message = getname() + " двинулся к игроку на (" + to_string(x) + ", " + to_string(y) + ")";
            return true;
        }
    }
    
    vector<pair<int, int>> allDirections = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    for (pair<int, int> dir : allDirections) {
        int testX = x + dir.first;
        int testY = y + dir.second;
        if (field.validpos(testX, testY) && 
            field.getNode(testX, testY).isPassable()) {
            if (field.getNode(testX, testY).hasTrap()) {
                int trapDamage = field.getNode(testX, testY).triggerTrap();
                this->takeDamage(trapDamage);
                field.getNode(x, y).removeEntity();
                x = testX;
                y = testY;
                field.getNode(x, y).setEnemy();
                message = getname() + " наступил на ловушку! Урон: " + to_string(trapDamage);
                return true;
            }
            field.getNode(x, y).removeEntity();
            x = testX;
            y = testY;
            field.getNode(x, y).setEnemy();
            message = getname() + " двинулся в сторону на (" + to_string(x) + ", " + to_string(y) + ")";
            return true;
        }
    }
    
    message = getname() + " не может двигаться, все направления заблокированы!";
    return false;
}