#include "../../headers/render/GameRenderer.h"
#include "../../headers/field/GameField.h"
#include "../../headers/field/Cell.h"
#include "../../headers/field/Position.h"
#include "../../headers/objects/Player.h"
#include "../../headers/objects/Enemy.h"
#include "../../headers/objects/EnemyTower.h"
#include "../../headers/objects/Trap.h"
#include "../../headers/enums/Enums.h"
#include <iostream>

using namespace std;

void GameRenderer::renderHeader(const GameField& field, const shared_ptr<Player>& player, int currentLevel, 
                 const vector<shared_ptr<Enemy>>& enemies, const vector<shared_ptr<EnemyTower>>& towers, 
                 const vector<shared_ptr<Trap>>& traps) {
    cout << "=== TERMINAL GAME - LEVEL " << currentLevel << " ===\n";
    cout << "Field: " << field.getWidth() << "x" << field.getHeight();
    cout << " | Health: " << player->getHealth() << "/" << player->getMaxHealth() << " | Weapon: " << player->getWeaponName();
    cout << " | Damage: " << player->getDamage() << " | Score: " << player->getScore();
    cout << " | Spells: " << player->getSpellHand().getSpellCount() << "/" << player->getSpellHand().getMaxSize();
    if (player->getAtackType() == AttackType::RANGED) {
        cout << " | Range: " << player->getRangedRange();
    }
    cout << "\n";
    cout << "Enemies: " << enemies.size() << " | Towers: " << towers.size() << " | Traps: " << traps.size() << "\n";
    cout << "P - player, E - enemy, T - tower, X - trap, # - blocked cell\n";
}

void GameRenderer::renderField(const GameField& field) {
    cout << "\n";
    for(int y = 0; y < field.getHeight(); y++){
        for (int x = 0; x < field.getWidth(); x++){
            Cell cell = field.getCell(Position(x, y));
            if(cell.getHasPlayer()){
                cout << "P ";
            } 
            else if(cell.getHasEnemy()){
                cout << "E ";
            }
            else if(cell.getHasTower()){
                cout << "T ";
            }
            else if(cell.getHasTrap()){
                cout << "X ";
            }
            else if(cell.getIsBlocked()){
                cout << "# ";
            } 
            else{
                cout << ". ";
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

void GameRenderer::renderGameOver(GameStatus status, int currentLevel, int score) {
    switch (status) {
        case GameStatus::PLAYER_DIED:
            cout << "Game Over! Your score: " << score << "\n";
            break;
        case GameStatus::LEVEL_COMPLETED:
            cout << "Level " << currentLevel << " completed! Final score: " << score << "\n";
            break;
        default:
            break;
    }
}