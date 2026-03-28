#pragma once
#include "game.h"
#include <fstream>
#include <iostream>

class WebRenderer {
public:
    void render(const Game& game) {
        std::ofstream f("game.html");
        if (!f.is_open()) return;

        f << "<!DOCTYPE html><html><head>";
        f << "<meta http-equiv='refresh' content='0.5'>"; 
        f << "<style>";
        f << "body { font-family: 'Courier New', monospace; background: #222; color: #eee; display: flex; flex-direction: column; align-items: center; }";
        f << ".status { font-size: 20px; margin-bottom: 10px; border: 1px solid #555; padding: 10px; }";
        f << ".field { display: grid; gap: 1px; background: #444; border: 5px solid #666; }";

        int width = game.getFieldWidth();   
        int height = game.getFieldHeight(); 

        f << ".field { grid-template-columns: repeat(" << width << ", 30px); }";
        f << ".cell { width: 30px; height: 30px; display: flex; justify-content: center; align-items: center; font-weight: bold; }";

        f << ".wall { background: #555; }";
        f << ".floor { background: #333; }";
        f << ".player { background: #00cc00; color: white; border-radius: 50%; }";
        f << ".enemy { background: #cc0000; color: white; border-radius: 50%; }";
        f << ".tower { background: #aa00aa; color: white; }";
        f << "</style></head><body>";

        f << "<div class='status'>";
        f << "HP: " << game.getPlayerHp() << " | XP: " << game.getPlayerXp() << "<br>";
        f << "Враги (HP): " << (game.isEnemyAlive() ? std::to_string(game.getEnemyHp()) : "Dead") << "<br>";
        f << "Рука:  ";

        f << "</div>";

        f << "<div class='field'>";

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {

                char symbol = ' ';
                std::string cssClass = "floor";

                bool isEntity = false;
                if (x == game.getPlayerX() && y == game.getPlayerY()) {
                    symbol = 'P'; cssClass = "player"; isEntity = true;
                } else if (game.isEnemyAt(x, y)) { 
                    symbol = 'E'; cssClass = "enemy"; isEntity = true;
                } else if (game.isTowerAt(x, y)) {
                    symbol = 'T'; cssClass = "tower"; isEntity = true;
                }

                if (!isEntity && game.isWall(x, y)) {
                    cssClass = "wall";
                }

                f << "<div class='cell " << cssClass << "'>" << symbol << "</div>";
            }
        }
        f << "</div>";

        f << "<p>Нажмите на консоль и используйте WASD для управления</p>";
        f << "</body></html>";
    }
};