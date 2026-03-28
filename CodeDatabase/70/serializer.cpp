#include "serializer.h"

Serializer::Serializer(){}

std::ostream& operator << (std::ostream &os, const Field &field);
std::istream& operator >> (std::istream& in, Field& field);

std::ostream& operator << (std::ostream &os, const Hero &hero);
std::istream& operator >> (std::istream& in, Hero& hero);

std::ostream& operator << (std::ostream &os, const EnemyManager &enemyManager);
std::istream& operator >> (std::istream& in, EnemyManager& enemyManager);

std::ostream& operator << (std::ostream &os, const EnemyBuildingManager &enemyBuildingManager);
std::istream& operator >> (std::istream& in, EnemyBuildingManager& enemyBuildingManager);

std::ostream& operator << (std::ostream &os, const std::pair<int, int> &pair)
{
    os << pair.first << " " << pair.second << " ";
    return os;
}
std::istream& operator >> (std::istream& in, std::pair<int, int>& pair)
{
    in >> pair.first >> pair.second;
    return in;
}

Serializer& Serializer::getInstance()
{
    static Serializer instance;
    return instance;
}

void Serializer::saveGame(std::string filename, const Game* game)
{
	std::ofstream outFile(filename);
    if (outFile.is_open()) {
    	outFile << game->getField() << std::endl;
		outFile << game->getHero() << std::endl;
		outFile << game->getEnemyManager() << std::endl;
		outFile << game->getEnemyBuildingManager() << std::endl;
		outFile << game->getHeroPos() << std::endl;
		outFile << game->getEnemyId() << std::endl;
		outFile << game->getHeroId() << std::endl;
        outFile << game->getStage() << std::endl;
        outFile.close();
        std::cout << "Game state saved successfully." << std::endl;
    } else {
        std::cerr << "Error: Unable to open file for saving." << std::endl;
    }
}
Game* Serializer::loadGame(std::string filename)
{
	Game* game = new Game();
    std::ifstream inFile(filename);
    if (inFile.is_open()) {
    	Field field;
    	Hero hero;
    	EnemyManager enemyManager;
    	EnemyBuildingManager enemyBuildingManager;
    	std::pair<int, int> heroPos;
    	char enemyId;
    	char heroId;
        size_t stage;
        try
        {
            inFile >> field;
            if (!inFile.good()) throw std::runtime_error("Failed to read field data");
            inFile.ignore();

            inFile >> hero;
            if (!inFile.good()) throw std::runtime_error("Failed to read hero data");
            inFile.ignore();

            inFile >> enemyManager;
            if (!inFile.good()) throw std::runtime_error("Failed to read enemyManager data");
            inFile.ignore(); 

            inFile >> enemyBuildingManager; 
            if (!inFile.good()) throw std::runtime_error("Failed to read enemyBuildingManager data");
            inFile.ignore();

            inFile >> heroPos;
            if (!inFile.good()) throw std::runtime_error("Failed to read hero position data");
            inFile.ignore(); 

            inFile >> enemyId;
            if (!inFile.good()) throw std::runtime_error("Failed to read enemy id data");
            inFile.ignore(); 

            inFile >> heroId;
            if (!inFile.good()) throw std::runtime_error("Failed to read hero id data");
            inFile.ignore(); 

            inFile >> stage;
            if (!inFile.good()) throw std::runtime_error("Failed to read stage data");
            inFile.ignore();

            if(enemyId < 0 or heroId < 0 or stage <=0 or stage > 10){
                throw std::invalid_argument("Game arguments are Incorrect!");
            }
            inFile.close();
        } catch (const std::invalid_argument& e) {
            std::cerr << "Incorrect parameteres were read: " << e.what() << std::endl;
            inFile.close();
            delete game;
            return nullptr;
        } catch (const std::runtime_error& e) {
            std::cerr << "Runtime Error: " << e.what() << std::endl;
            inFile.close();
            delete game;
            return nullptr;
        } catch (...) {
            std::cerr << "An unknown error occured." << std::endl;
            inFile.close();
            delete game;
            return nullptr;
        }

        game->setField(field);
        game->setHero(hero);
        game->setEnemyManager(enemyManager);
        game->setEnemyBuildingManager(enemyBuildingManager);
        game->setHeroPos(heroPos);
		game->setHeroId(heroId);
		game->setEnemyId(enemyId);
        game->setStage(stage);
        game->initSpells();

        std::cout << "Game state loaded successfully." << std::endl;
    } else {
        std::cerr << "Error: Unable to open file for loading." << std::endl;
    }
    return game;
}