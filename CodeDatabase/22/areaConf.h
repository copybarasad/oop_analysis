#pragma once
#include <vector>
#include <string>

struct areaConf{

    std::vector<std::string> areaNames = {"Гололед", "Иссушающее пламя", "Чума"};
    std::vector<std::string> areaDescriptions = {"Попробуй устоять на ногах", "Никто не выживет", "Пора надеть маску"};
    std::vector<int> areaDamage = {30, 100, 70};
    std::vector<int> areaRadius = {7, 4, 9};
};