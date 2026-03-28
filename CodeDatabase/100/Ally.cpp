#include "Ally.h"
#include "GameField.h"

// Конструктор
Ally::Ally(int health, int damage, int startX, int startY, char symbol) :
        Character(health, damage, startX, startY, symbol) { }

// Передвижение
void Ally::move(int enemyX, int enemyY, const GameField& field) {
    if (isAlive()) { 
        prevX = posX;
        prevY = posY;

        int deltaX = enemyX - posX; 
        int deltaY = enemyY - posY; 

        // Если по оси X расстояние ненулевое, пытаемся двигаться по ней
        if (deltaX != 0) {
            int stepX = (deltaX > 0) ? 1 : -1;
            int newX = posX + stepX;
            if (field.isCellPassable(newX, posY)) {
                posX = newX;
                return;
            }
        }

        // Если по X не получилось, пробуем по Y
        if (deltaY != 0) {
            int stepY = (deltaY > 0) ? 1 : -1;
            int newY = posY + stepY;
            if (field.isCellPassable(posX, newY)) {
                posY = newY;
                return;
            }
        }   
    } 
}
