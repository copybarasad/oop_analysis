#include "console/consoleRender.h"


void ConsoleRenderer::drawField(const Field &field)
{
    for (auto &y : field.getField())
    {
        std::string line;
        for (auto &x : y)
        {
            

            switch (x.getOcupant())
            {
            case PLAYER:
                line += "P ";
                break;
            case ENEMY:
                line += "E ";
                break;
            case ENEMYHUT:
                line += "H ";
                break;
            case TOWER:
                line += "T ";
                break;
            case ALLY:
                line += "A ";
                break;

            default:
                switch (x.getType())
                {
                case TRAP:
                    line += "^ ";
                    break;
                case SLOW:
                    line += "~ ";
                    break;
                case WALL:
                    line += "# ";
                    break;
                default:
                    if (x.getReady() == true)
                    {
                        line += "# ";
                    }
                    else
                    {
                        line += ". ";
                    }
                    break;
                }
                break;
            }
        }
        std::cout << line << std::endl;
    }
    std::cout << std::string(50, '_') << std::endl;
}


void ConsoleRenderer::drawPlayer(const Player &player)
{
    std::cout << std::string(50, '_') << std::endl;
    std::cout << "Level: " << player.getLevel() << " | Score: " << player.getScore() << std::endl;
    std::cout << "Health: " << player.getHealth() << " | Weapon: " << player.getWeapon() << std::endl;
    std::cout << std::string(50, '_') << std::endl;
}

void ConsoleRenderer::drawEnemies(const std::vector<Position> &enemies)
{
    if (enemies.empty()){
        std::cout << "Рядом нет врагов" << std::endl;
        return;
    }

    int i = 0;
    for (auto en : enemies){
        std::cout << i++ << "Enemy: " << '(' << en.x << ", " << en.y << ')' << std::endl;
    }
}

void ConsoleRenderer::drawLevel(int level)
{
    std::cout << "Game Level: " << level << std::endl;
    std::cout << std::string(60, '_') << std::endl;
}

void ConsoleRenderer::drawGameOver(const Player &player)
{
    std::cout << std::string(50, '_') << std::endl;
    std::cout << std::string(20, '_') << "GAME OVER" << std::string(20, '_') << std::endl;
    std::cout << std::string(20, '_') << "SCORE: "
                << (player.getLevel() - 1) * 100 + player.getScore()
                << std::string(20, '_') << std::endl;
    std::cout << std::string(50, '_') << std::endl;
}

void ConsoleRenderer::drawHand(const Hand &hand)
{
    auto &spells = hand.getSpells();
    if (spells.empty())
    {
        std::cout << "No spells available!" << std::endl;
        return;
    }
    int i = 1;
    std::cout << "Spells: " << std::endl;
    std::cout << "0. Cancel" << std::endl;
    for (auto &spell : spells)
    {
        std::cout << i++ << ". " << spell->name() << std::endl;
    }
}

void ConsoleRenderer::drawImproveOptions()
{
    std::cout << "Choose improvement:" << std::endl;
    std::cout << "| Health H | Damage D | Spell S |" << std::endl;
}

void ConsoleRenderer::showStartMenu()
{
    std::cout << "For new game input - N" << std::endl;
    std::cout << "For load game input - L" << std::endl;
}


void ConsoleRenderer::drawSaveDir(std::string folderPath){
    namespace fs = std::filesystem;

    int i = 0;
    for (const auto &entry : fs::directory_iterator(folderPath))
    {
        if (entry.is_regular_file())
        {
            std::string fileName = entry.path().filename().string();
            std::cout << i++ << ". " << fileName << std::endl;
        }
    }
}

void ConsoleRenderer::drawSave(){
    std::cout << "Input file name: " << std::endl;
}