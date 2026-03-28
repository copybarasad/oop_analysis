#include "UpgradeManager.h"
#include <iostream>
#include <iomanip>
#include <random>


UpgradeManager::UpgradeManager(int size){
    this->size = size;
    addRandomUps();
}

void UpgradeManager::addRandomUps(){
    for(int i = 0; i < size; i++){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 2);
        
        Upgrade up(dis(gen));
        ups.push_back(up);
    }
}

int UpgradeManager::getSize(){
    return (int)ups.size();
}

Upgrade *UpgradeManager::getUp(int ind){
    if(ind < 0 || ind >= size){
        return nullptr;
    }
    Upgrade tmp = ups[ind];
    Upgrade *tmp_ptr = &tmp;
    ups.erase(ups.begin() + ind);
    size = (int)ups.size();
    return tmp_ptr;
}

int UpgradeManager::use(Upgrade *up, int thing){
    return thing + up->getAmount();
}

void UpgradeManager::display(){
    cout << "\033[44m-------- ДОСТУПНЫЕ УЛУЧШЕНИЯ --------\033[0m" << endl << endl;
    for(int i = 0; i < size; i++){
        cout << "\033[33m================================\033[0m" << endl;
        cout << i << "\033[34m<<" << ups[i].getName() << ">>\033[0m" << endl;
        cout << "\033[34mОписание:\033[0m " << ups[i].getDescription() << endl;
        cout << "\033[34mСтоимость:\033[0m " << ups[i].getCost() << " \033[34mУлучшит показатель на:\033[0m "<< ups[i].getAmount() << endl;
        cout << "\033[33m================================\033[0m" << endl;
    }
}

void UpgradeManager::reset(){
    ups.clear();
    size = 0;
}