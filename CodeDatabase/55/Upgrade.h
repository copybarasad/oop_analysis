#pragma once
#include <string>
using namespace std;

class Upgrade{
private:
    int id;
    int cost;
    int amount;
    string name;
    string description;

public:
    Upgrade(int i);
    int getId();
    int getCost();
    int getAmount();
    bool check(int score);
    const string getName();
    const string getDescription();
};