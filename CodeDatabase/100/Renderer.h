#pragma once
#include <vector>
#include <string>

class GameField;
class Player;
class PlayerHand;
class Character;
class EnemyBuilding;
class EnemyTower;
class Ally;

class Renderer {
public:
    virtual ~Renderer() = default;
    
    // Отрисовка поля
    virtual void drawField(const GameField& field, 
                          const std::vector<Character*>& characters,
                          const std::vector<EnemyBuilding*>& buildings,
                          const std::vector<EnemyTower*>& towers,
                          const std::vector<Ally*>& allies) = 0;
    
    // Отрисовка информации об игроке
    virtual void drawPlayerInfo(const Player& player, const PlayerHand& hand) = 0;
    
    // Вывод событий в игре
    virtual void drawMessages(const std::vector<std::string>& messages) = 0;

    // Очистка экрана
    virtual void clearScreen() = 0;
};