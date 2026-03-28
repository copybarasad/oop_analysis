#include "game.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include "../abstractNinterfaces/defines.h"

game::game() : currentMap(NULL){}

void game::newGame()
{
    std::vector<size_t>* mapParameters;
    mapParameters = determineScenario();
    if(currentMap){
        delete currentMap;
    }
    currentMap = new map(mapParameters->at(0), mapParameters->at(1));
    currentMap->generate(mapParameters->at(2), mapParameters->at(3), mapParameters->at(4), mapParameters->at(5));
    delete mapParameters;
    play();
}

void game::continueGame()
{
    if(this->currentMap){
        if(currentMap->getHero()){
            if(currentMap->getHero()->getMenuFlag()){
                currentMap->getHero()->changeMenuFlag();
            }
        }else{
            std::cout << "Couldn't load or spawn a hero.\n";
        }
        play();
    }else{
        std::cout << "No current game map. Load a save or start a new game.\n";
    }
}

void game::close()
{
    std::cout << "The game is closing...\n";
    activeFlag = false;
}

bool game::isActive()
{
    return activeFlag;
}

void game::setCurrentMap(map* newMap)
{
    this->currentMap = newMap;
}

void game::menu()
{ 
    std::cout << MENU;
}

void game::save()
{
    listSaves();

    std::cout << "Which slot do you want to write your save in? (1-9)\n";
    size_t saveSlot = getANumber();
    if(saveSlot > SAVESLIMIT || saveSlot < 1){
        std::cout << "You can only have " << SAVESLIMIT << " saves simultaneously starting from 1\n";
    }

    std::string saveFileName = std::string(SAVEPATH) + std::string("/save") + std::to_string(saveSlot);
    if(std::filesystem::exists(saveFileName.c_str())){
        size_t choice;
        while(true){
            std::cout << "Are you sure you want to rewrite your save? (1/0)\n";
            choice = getANumber();
            if(choice == 1){
                writeASave(saveFileName.c_str());
                break;
            }
            if(!choice){
                break;
            }
        }
    }else{
        writeASave(saveFileName.c_str());
    }
}

void game::writeASave(const char* saveFileName)
{
    if(!currentMap){
        std::cout << "No current map to save\n";
        return;
    }
    std::ofstream out(saveFileName);
    if(out.is_open()){
        out << saveFileName << " info\n";
        out << "height:" << currentMap->getHeight() << '\n';
        out << "length:" << currentMap->getLength() << '\n';
        out << "tiles:\n";
        for(size_t i = 0; i < currentMap->getHeight(); i++){
            for(size_t j = 0; j < currentMap->getLength(); j++){
                tile _tile = currentMap->getTiles()[i][j];
                out << _tile.name << " ";
            }
            out << "\n";
        }
        out << "hero:\n" << currentMap->getHero()->getMapPosition().x << '\n';
        out << currentMap->getHero()->getMapPosition().y << '\n';
        out << currentMap->getHero()->getHP() << '\n';
        out << currentMap->getHero()->getAttackDmg() << '\n';
        out << currentMap->getHero()->getStance() << '\n';
        out << currentMap->getHero()->getMP() << '\n';
        out << currentMap->getHero()->getInventory().getSpellList().size() << '\n';
        for(auto x : currentMap->getHero()->getInventory().getSpellList()){
            out << x->getRank() << ',' << x->getName() << ',';
        }
        out << "\nenemies:\n" << currentMap->getEnemies().size() << '\n';
        for(auto x : currentMap->getEnemies()){
            out << x->getMapPosition().x << ' ';
            out << x->getMapPosition().y << ' ';
            out << x->getId() << ' ';
            out << x->getHP() << ' ';
            out << x->getAttackDmg() << ' ' <<'\n';
        }
        out << "castles:\n" << currentMap->getCastles().size() << '\n';
        for(auto x : currentMap->getCastles()){
            out << x->getMapPosition().x << ' ';
            out << x->getMapPosition().y << ' ';
            out << x->getId() << ' ';
            out << x->getHP() << ' ';
            out << x->getSpawnTimer() << ' ' << '\n';
        }
        out << "turn:" << currentMap->getTurn() << '\n';
        out << "difficultyLvl:" << currentMap->getDifficultyLvL() << '\n'; 
        out.close();
        std::cout << "The game is saved\n";
    }else{
        std::cout << "Couldn't open " << saveFileName << "\n";
    }
}

void game::listSaves()
{
    if(std::filesystem::exists(SAVEPATH) && std::filesystem::is_directory(SAVEPATH)){
        size_t saveSlot = 0;
        std::string saveFileName;
        for (const auto& entry : std::filesystem::directory_iterator(SAVEPATH)) {
            if (std::filesystem::is_regular_file(entry.status())) {
                saveFileName = entry.path().string();
                saveFileName.pop_back();
                if(!saveFileName.compare("./game/saves\\save")){
                    std::cout << std::to_string(++saveSlot) << ". " << entry.path() << '\n';
                }
            }
        }
    }else{
        std::cout << std::string("No correct directory ") + std::string(SAVEPATH) + "\nCreated a new directory - " + std::string(SAVEPATH) + '\n';
        std::filesystem::create_directory(SAVEPATH);
    }
}

void game::load()
{
    listSaves();
    std::cout << "Which slot do you want to load your save from?\n";
    size_t command = getANumber();
    std::string wantedSavePath = std::string(SAVEPATH) + std::string("/save") + std::to_string(command);
    if(std::filesystem::exists(wantedSavePath.c_str())){
        std::ifstream in(wantedSavePath.c_str());
        if (in.is_open()){
            std::string currentLine;
            std::getline(in, currentLine);
            if(in.good() && !currentLine.compare(wantedSavePath + std::string(" info"))){
                map* newMap = loadMap(in);
                if(!newMap){
                    std::cout << "Save file is corrupted in map part\n";
                    return;
                }
                newMap->setHero(loadHero(in));
                if(!newMap->getHero()){
                    std::cout << "Save file is corrupted in Hero part\n";
                    return;
                }
                newMap->getTiles()[newMap->getHero()->getMapPosition().x][newMap->getHero()->getMapPosition().y].address = newMap->getHero();
                newMap->getEnemies() = loadEnemies(in);
                if(newMap->getEnemies().size() == 1 && newMap->getEnemies().at(0) == NULL){
                    std::cout << "Save file is corrupted in Enemies part\n";
                    return;
                }
                for(size_t i = 0; i < newMap->getEnemies().size(); i++){
                    newMap->getTiles()[newMap->getEnemies().at(i)->getMapPosition().y][newMap->getEnemies().at(i)->getMapPosition().x].address = newMap->getEnemies().at(i);
                }
                newMap->getCastles() = loadCastles(in);
                if(newMap->getCastles().size() == 1 && newMap->getCastles().at(0) == NULL){
                    std::cout << "Save file is corrupted in Castles part\n";
                    return;
                }
                for(size_t i = 0; i < newMap->getCastles().size(); i++){
                    newMap->getTiles()[newMap->getCastles().at(i)->getMapPosition().y][newMap->getCastles().at(i)->getMapPosition().x].address = newMap->getCastles().at(i);
                }
                newMap->setTurn(loadTurn(in));
                if(!newMap->getTurn()){
                    std::cout << "Save file is corrupted in Turn part\n";
                    return;
                }
                newMap->setDifficultyLvl(loadDifficultyLvl(in));
                if(!newMap->getDifficultyLvL()){
                    std::cout << "Save file is corrupted in difficulty level part\n";
                    return;
                }
                if(currentMap){
                    delete currentMap->getHero();
                    delete currentMap;
                }
                currentMap = newMap;
                std::cout << "Save is loaded\n";
                in.close();
            }else{
                std::cout << "Save file is corrupted in header part of a save file\n";
            }
        }else{
            std::cout << "Couldn't open " << wantedSavePath << '\n';
        }
    }else{
        std::cout << "Couldn't find " << wantedSavePath << '\n';
    }
}

map *game::loadMap(std::ifstream& in)
{
    size_t height;
    size_t length;
    std::string currentLine;
    std::getline(in, currentLine, ':');
    if(in.good() && !currentLine.compare(std::string("height"))){ 
        getline(in, currentLine);
        if(in.good()){
            for(auto x : currentLine){
                if(!isdigit(x)){
                    std::cout << "Not a number in hero part of a save file\n";
                    return NULL;
                }
            }
            height = std::stoull(currentLine);
        }else{
            std::cout << "Couldn't read a value of height\n";
            return NULL;
        }
    }else{
        std::cout << "Wrong name of a block in hero part of save file\n";
        return NULL;
    }
    std::getline(in, currentLine, ':');
    if(in.good() && !currentLine.compare(std::string("length"))){ 
        getline(in, currentLine);
        if(in.good()){
            for(auto x : currentLine){
                if(!isdigit(x)){
                    std::cout << "Not a number in hero part of a save file\n";
                    return NULL;
                }
            }
            length = std::stoull(currentLine);
        }else{
            std::cout << "Couldn't read a value of length\n";
            return NULL;
        }
    }else{
        std::cout << "Wrong name of a block in hero part of save file\n";
        return NULL;
    }
    
    map* newMap = new map(height, length);
    std::getline(in, currentLine);
    std::vector<char> allTilesAlphabet = {NOTHING, BLOCK, ENEMY, TRAP, CASTLE, HERO};
    for(size_t i = 0; i < height; i++){
        for(size_t j = 0; j < length; j++){
            if(in.good()){
                std::getline(in, currentLine, ' ');
                if(std::find(allTilesAlphabet.begin(), allTilesAlphabet.end(), currentLine[0]) != allTilesAlphabet.end()){
                    newMap->getTiles()[i][j].name = currentLine[0];
                }else{
                    std::cout << "Wrong tiles symbols in map part of a save file\n";
                    return NULL;
                }
            }else{
                std::cout << "Couldn't read line in map part of a save file\n";
                return NULL;
            }
        }
        std::getline(in, currentLine);
    }
    return newMap;
}

hero *game::loadHero(std::ifstream & in)
{
    std::string currentLine;
    std::getline(in, currentLine);
    if(in.good() && !currentLine.compare(std::string("hero:"))){
        coords mapPosition;
        size_t id;
        int hp;
        size_t attack;
        size_t stance;
        size_t MP;
        size_t spellNum;
        
        for(size_t i = 0; i < 7; i++){
            std::getline(in, currentLine);
            if(in.good()){
                for(auto x : currentLine){
                    if(!isdigit(x)){
                        std::cout << "Not a number in hero part of a save file\n";
                        return NULL;
                    }
                }
                switch(i){
                    case 0:
                        mapPosition.x = std::stoull(currentLine);
                        break;
                    case 1:
                        mapPosition.y = std::stoull(currentLine);
                        break;
                    case 2:
                        hp = std::stoi(currentLine);
                        break;
                    case 3:
                        attack = std::stoull(currentLine);
                        break;
                    case 4:
                        stance = std::stoull(currentLine);
                        break;
                    case 5:
                        MP = std::stoull(currentLine);
                        break;
                    case 6:
                        spellNum = std::stoull(currentLine);
                        break;
                }
            }else{
                std::cout << "Couldn't read a line in hero part of a save file\n";
                return NULL;
            }
        }
        hero* newHero = new hero(hp, attack, MP);
        size_t rank;
        newHero->setMapPosition(mapPosition.x, mapPosition.y);
        newHero->setStance(stance);
        newHero->getInventory().freeHand(0);
        for(size_t i = 0; i < spellNum; i++){
            std::getline(in, currentLine, ',');
            if(in.good()){
                for(auto x : currentLine){
                    if(!isdigit(x)){
                        std::cout << "Not a number in hero part of a save file\n";
                        return NULL;
                    }
                    rank = std::stoull(currentLine);
                }
            }
            std::getline(in, currentLine, ',');
            if(in.good()){
                if(!currentLine.compare(ICESPEARNAME)){
                    newHero->getInventory().addSpell(1, rank);
                }else{
                    if(!currentLine.compare(FLAMESTORMNAME)){
                        newHero->getInventory().addSpell(2, rank);
                    }else{
                        std::cout << "Wrong spell name in hero part of a save file\n";
                        return NULL;
                    }
                }
            }else{
                std::cout << "Couldn't read a line in hero part of a save file\n";
                return NULL;
            }
        }
        std::getline(in, currentLine);
        return newHero;
    }else{
        std::cout << "Wrong name of a block in hero part of a save file\n";
        return NULL;
    }
    
}

std::vector<enemy *> game::loadEnemies(std::ifstream & in)
{
    std::string currentLine;
    std::getline(in, currentLine);
    if(in.good() && !currentLine.compare(std::string("enemies:"))){
        std::getline(in, currentLine);
        size_t size;
        std::vector<enemy*> newEnemies;
        enemy* newEnemy;
        size_t x;
        size_t y;
        size_t id;
        int hp;
        size_t attack;
        if(in.good()){
            for(auto x : currentLine){
                if(!isdigit(x)){
                    std::cout << "Not a number in enemies part of a save file\n";
                    return {NULL};
                }
            }
            size = std::stoull(currentLine);
            for(size_t i = 0; i < size; i++){
                for(size_t j = 0; j < 5; j++){
                    std::getline(in, currentLine, ' ');
                    if(in.good()){
                        for(auto x : currentLine){
                            if(!isdigit(x)){
                                std::cout << "Not a number in enemies part of a save file\n";
                                return {NULL};
                            }
                        }
                        switch(j){
                            case 0:
                                x = std::stoull(currentLine);
                                break;
                            case 1:
                                y = std::stoull(currentLine);
                                break;
                            case 2:
                                id = std::stoull(currentLine);
                                break;
                            case 3: 
                                hp = std::stoi(currentLine);
                                break;
                            case 4:
                                attack = std::stoull(currentLine);
                                break;
                        }
                    }
                }
                std::getline(in, currentLine);
                newEnemy = new enemy(hp, attack);
                newEnemy->setMapPosition(x,y);
                newEnemy->setId(id);
                newEnemies.push_back(newEnemy);
            }
        }
        return newEnemies;
    }else{
        std::cout << "Wrong name of a block in enemies part of save file\n";
        return {NULL};
    }
}

std::vector<castle *> game::loadCastles(std::ifstream & in)
{
std::string currentLine;
    std::getline(in, currentLine);
    if(in.good() && !currentLine.compare(std::string("castles:"))){
        std::getline(in, currentLine);
        size_t size;
        std::vector<castle*> newCastles;
        castle* newCastle;
        size_t x;
        size_t y;
        size_t id;
        int hp;
        size_t spawnTimer;
        if(in.good()){
            for(auto x : currentLine){
                if(!isdigit(x)){
                    std::cout << "Not a number in castles part of a save file\n";
                    return {NULL};
                }
            }
            size = std::stoull(currentLine);
            for(size_t i = 0; i < size; i++){
                for(size_t j = 0; j < 5; j++){
                    std::getline(in, currentLine, ' ');
                    if(in.good()){
                        for(auto x : currentLine){
                            if(!isdigit(x)){
                                std::cout << "Not a number in castles part of a save file\n";
                                return {NULL};
                            }
                        }
                        switch(j){
                            case 0:
                                x = std::stoull(currentLine);
                                break;
                            case 1:
                                y = std::stoull(currentLine);
                                break;
                            case 2:
                                id = std::stoull(currentLine);
                                break;
                            case 3: 
                                hp = std::stoi(currentLine);
                                break;
                            case 4:
                                spawnTimer = std::stoull(currentLine);
                                break;
                        }
                    }
                }
                std::getline(in, currentLine);
                newCastle = new castle(hp, spawnTimer);
                newCastle->setMapPosition(x,y);
                newCastle->setId(id);
                newCastles.push_back(newCastle);
            }
        }
        return newCastles;
    }else{
        std::cout << "Wrong name of a block in castles part of save file\n";
        return {NULL};
    }
}

size_t game::loadTurn(std::ifstream & in)
{
    std::string currentLine;
    std::getline(in, currentLine, ':');
    if(in.good() && !currentLine.compare(std::string("turn"))){ 
        getline(in, currentLine);
        if(in.good()){
            for(auto x : currentLine){
                if(!isdigit(x)){
                    std::cout << "Not a number in turn part of a save file\n";
                    return 0;
                }
            }
            return std::stoull(currentLine);
        }else{
            std::cout << "Couldn't read a value of a turn\n";
            return 0;
        }
    }else{
        std::cout << "Wrong name of a block in turn part of save file\n";
        return 0;
    }
}

size_t game::loadDifficultyLvl(std::ifstream & in)
{
    std::string currentLine;
    std::getline(in, currentLine, ':');
    if(in.good() && !currentLine.compare(std::string("difficultyLvl"))){ 
        getline(in, currentLine);
        if(in.good()){
            for(auto x : currentLine){
                if(!isdigit(x)){
                    std::cout << "Not a number in difficulty level part of a save file\n";
                    return 0;
                }
            }
            return std::stoull(currentLine);
        }else{
            std::cout << "Couldn't read a value of a difficulty level\n";
            return 0;
        }
    }else{
        std::cout << "Wrong name of a block in difficulty level part of save file\n";
        return 0;
    }
}

void game::play()
{   
    bool winFlag = 0;
    while(true){
        if(currentMap->getHero()->getHP() <= 0){
            std::cout << "End of the game\n";
            break;
        }
        if(currentMap->getEnemies().empty()){
            std::cout << "You won\n";
            winFlag = 1;
            break;
        }
        currentMap->show();
        std::cout << currentMap->getTurn()<< " turn\n";
        std::cout << "Your move. ";
        currentMap->getHero()->makeTurn(currentMap);
        if(currentMap->getHero()->getMenuFlag()){
            return;
        }
        for(enemy* x : currentMap->getEnemies()){
            x->makeTurn(currentMap);
        }
        if(!currentMap->getCastles().empty()){
            for(castle* x: currentMap->getCastles()){
                if(!(currentMap->getTurn()%x->getSpawnTimer()))
                    x->makeTurn(currentMap);
            }
        }
        currentMap->nextTurn();
    }
    if(winFlag){
        this->nextLevel();
    }
}

void game::nextLevel()
{
    std::cout << "(1) Do you want to continue and play the next level or (2) save it and go to the menu?\n";
    size_t command = getANumber();
    std::vector<size_t>* mapParameters;
    map* newMap;
    switch(command){
        case 1:
            mapParameters = determineScenario();
            newMap = new map(mapParameters->at(0), mapParameters->at(1));
            newMap->setDifficultyLvl(currentMap->getDifficultyLvL()+1);
            newMap->generate(mapParameters->at(2), mapParameters->at(3), mapParameters->at(4), mapParameters->at(5));
            currentMap->getHero()->setMapPosition(newMap->getHero()->getMapPosition().x, newMap->getHero()->getMapPosition().y);
            newMap->setHero(currentMap->getHero());
            currentMap = newMap;
            for(size_t i = 0; i < currentMap->getHero()->getInventory().getSpellList().size()/2; i++){
                srand(time(NULL));
                char random = rand()%currentMap->getHero()->getInventory().getSpellList().size();
                currentMap->getHero()->getInventory().getSpellList().erase(currentMap->getHero()->getInventory().getSpellList().begin()+random);
            }
            delete mapParameters;
            std::cout << "Your new map:\n";
            currentMap->show();
            currentMap->getHero()->buyUpgrades(currentMap);
            currentMap->getHero()->setHP(currentMap->getHero()->getMaxHP());
            this->play();
            break;
        case 2:
            this->save();
            break;
        default:
            std::cout << "Enter again\n";
    }
}

std::vector<size_t>* game::determineScenario()
{
    std::string wantedScenarioPath = (std::string(SCENARIOPATH)+std::string("/generationKeys"));
    if(std::filesystem::exists(wantedScenarioPath.c_str())){
        std::ifstream in(wantedScenarioPath.c_str());
        if (in.is_open()){
            std::string currentLine;
            srand(time(NULL));
            char random;
            random = rand()%4;
            for(size_t i = 0; i < random; i++){
                std::getline(in, currentLine);
                if(!in.good()){
                    std::cout << "Scenario file is corrupted\n";
                    restoreScenarios();
                    return determineScenario();
                }
            }
            std::vector<size_t>* scenario = new std::vector<size_t>();
            for(size_t i = 0; i < 6; i++){
                if(in.good()){
                    std::getline(in, currentLine, ' ');
                    for(auto x : currentLine){
                        if(!isdigit(x)){
                            std::cout << "Scenario file is corrupted\n";
                            restoreScenarios();
                            return determineScenario();
                        }
                    }
                    switch(i)
                    {
                    case 0:
                        scenario->push_back(std::stoull(currentLine));
                        break;
                    case 1:
                        scenario->push_back(std::stoull(currentLine));
                        break;
                    case 2:
                        scenario->push_back(std::stoull(currentLine));
                        break;
                    case 3:
                        scenario->push_back(std::stoull(currentLine));
                        break;
                    case 4:
                        scenario->push_back(std::stoull(currentLine));
                        break;
                    case 5:
                        scenario->push_back(std::stoull(currentLine));
                        break;
                    }
                }
            }
            return scenario;
        }else{
            std::cout << "Can't open a generation key file\n";
            return 0;
        }
    }else{
        std::cout << "No such path as " << wantedScenarioPath << "\nRestoring generation keys\n";
        restoreScenarios();
        return determineScenario();
    }
}

void game::restoreScenarios()
{
    std::string ScenariosFilePath = std::string(SCENARIOPATH)+std::string("/generationKeys");
    if(std::filesystem::exists(SCENARIOPATH) && std::filesystem::is_directory(SCENARIOPATH)){
        for (const auto& entry : std::filesystem::directory_iterator(SAVEPATH)) {
            if(std::filesystem::is_regular_file(entry.status()) && entry.path().compare((ScenariosFilePath).c_str())) {
                std::ofstream out(ScenariosFilePath);
                if(out.is_open()){
                    out << SCENARIOS;
                    out.close();
                }else{
                    std::cout << "Couldn't create generationKeys\n";
                }
            }
        }
    }else{
        std::cout << "Created a new directory - " + std::string(SCENARIOPATH) + '\n';
        std::filesystem::create_directory(SCENARIOPATH);
        std::ofstream out(ScenariosFilePath);
        if(out.is_open()){
            out << SCENARIOS;
            out.close();
        }else{
            std::cout << "Couldn't create generationKeys\n";
        }
    }
}

game::~game()
{
    if(currentMap){
        delete currentMap;
    }
}
