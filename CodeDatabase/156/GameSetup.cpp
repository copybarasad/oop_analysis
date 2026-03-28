//
// Created by Artem on 24.09.2025.
//

#include "GameSetup.h"
#include "../Constants/Constants.h"
#include "Utils/ReadWrightJson.h"
#include "../GameManagment/GameMaster.h"
#include <iostream>
#include "../Logger/Logger.h"
bool chooseSetupType();



void GameSetup::start() {
    Logger::init(LogOutput::FILE, "../log.txt");

    Logger::tech("Game setup starts!");

    std::string json;
    if (chooseSetupType()){
        try{
            json = ReadWrightJson::read("../saveGame.txt");
        }catch (const UnexpectedBehaviorException& ex) {
            std::cout << "Can't open saveGame file, file damaged or not exist" << std::endl;
            Logger::tech("Can't open saveGame file, file damaged or not exist!");
            start();
        }
    }else{
        json = ReadWrightJson::read("../level0.txt");
    }
    GameMaster<GamerConsoleInputSpotter, ConsoleRenderer> gm;
    std::string keysJson = ReadWrightJson::read("../keysModel.txt");
    if(gm.startGame(json, keysJson)){
        setupLevelN("../level1.txt");
    }else{
        start();
    }

}

void GameSetup::setupLevelN(std::string fileName) {
    std::string json;
    std::string keysJson = ReadWrightJson::read("../keysModel.txt");
    json = ReadWrightJson::read(fileName);
    GameMaster<GamerConsoleInputSpotter, ConsoleRenderer> gm;
    gm.startGame(json, keysJson);
}

bool chooseSetupType(){ //true - запуск с сохранения, false - запуск с нового уровня
    std::cout << "Choose game mod:\n1.New Game\n2.Saved game" << std::endl;
    int gm = Constants::getInput<int>();
    if (gm == 1){
        return false;
    }else if(gm == 2){
        return true;
    }else{
        std::cout << "Non correct value!" << std::endl;
        return chooseSetupType();
    }
}