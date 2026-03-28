#pragma once
#include "Upgrade.h"
#include "Player.h"
#include <vector>


class UpgradeManager{
private:
    std::vector<Upgrade> ups;
    int size;

public:
    UpgradeManager(int size = 0);
    void addRandomUps();
    Upgrade *getUp(int ind);
    int getSize();
    int use(Upgrade *up, int thing);
    void display();
    void reset();
};