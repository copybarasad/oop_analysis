#include <iostream>
#include <filesystem>
#include <fstream>
#include "../../Headers/Controllers/SystemLoadController.h"

#include "../../Headers/Containers/Hand.h"

void SystemLoadController::ToLoadingLoop()
{
    int inGame = 0;
    while (true)
    {
        if (inGame && inGame != 2)
            inGame = _controller.ToNextGameIteration();
        else
        {
            PrintIntroduction();
            std::string line;
            std::cin >> line;
            switch (ToCommand(line))
            {
                case 1:
                    _controller.Initialize();
                    _controller.PrintIntroduction();
                    inGame = 1;
                    break;
                case 2:
                    if (inGame == 0)
                    {
                        std::cout << "Игра закончена или не загружена. Начните новую игру или загрузите "
                                     "сохранение." << std::endl;
                        break;
                    }
                    _controller.PrintIntroduction();
                    inGame = 1;
                    break;
                case 3:
                    if (inGame == 0)
                    {
                        std::cout << "Игра закончена или не загружена. Начните новую игру или загрузите "
                                     "сохранение." << std::endl;
                        break;
                    }
                    SaveGame();
                    break;
                case 4:
                    if (PrintSaves() && LoadGame())
                    {
                        _controller.PrintIntroduction();
                        inGame = 1;
                    }
                    break;
                default:
                    return;
            }
        }
    }
}

void SystemLoadController::SaveGame()
{
    auto timePoint = std::chrono::system_clock::now();
    time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::string path;
    {
        std::ostringstream oss;
        oss << "Saves/Save_" << std::put_time(std::localtime(&time), "%d%m%y_%H%M%S") << ".txt";
        path = oss.str();
    }
    std::filesystem::path directory("Saves");
    if (!std::filesystem::exists(directory))
        std::filesystem::create_directory(directory);
    if (std::filesystem::exists(path))
    {
        std::cout << "Такое сохранение уже сущесвует. Пожалуйста, подождите секунду с последнего "
                     "сохранения." << std::endl;
        return;
    }
    std::ofstream out;
    out.open(path);
    if (out.is_open())
    {
        out << FieldContainer::FieldData(&_controller._currentRoot1->_container).ToString() << std::endl;
        out << Hand::HandData(&_controller._currentRoot2->_playerHand).ToString() << std::endl;
        out << Player::PlayerData(&_controller._currentRoot1->_playerCreator.GetAll().front()).ToString() <<
            std::endl;
        int i = 0;
        for (const Enemy& enemy : _controller._currentRoot1->_enemyCreator.GetAll())
            out << Enemy::EnemyData(&enemy).ToString(i++) << std::endl;
        i = 0;
        for (const StraightTower& tower : _controller._currentRoot2->_towerCreator.GetAll())
            out << StraightTower::StraightTowerData(&tower).ToString(i++) << std::endl;
    }
    else
        std::cout << "Произошла непредвиденная ошибка открытия файла." << std::endl;
    out.close();
}

bool SystemLoadController::LoadGame()
{
    std::cout << "Введите индекс: ";
    int i;
    std::cin >> i;
    std::string path;
    for (const auto& entry : std::filesystem::directory_iterator("Saves"))
    {
        if (i-- == 0)
        {
            path = entry.path().string();
            break;
        }
    }
    if (path == "")
    {
        std::cout << "Такого индекса сохранения нет" << std::endl;
        return false;
    }
    std::ifstream in;
    in.open(path);
    if (in.is_open())
    {
        std::string s{std::istreambuf_iterator(in), std::istreambuf_iterator<char>()};
        std::vector<Enemy::EnemyData> enemies;
        i = 0;
        Enemy::EnemyData enemy = Enemy::EnemyData(s, i);
        while (enemy._position != Cell())
        {
            enemies.push_back(enemy);
            enemy = Enemy::EnemyData(s, ++i);
        }
        i = 0;
        std::vector<StraightTower::StraightTowerData> towers;
        StraightTower::StraightTowerData tower = StraightTower::StraightTowerData(s, i);
        while (tower._position != Cell())
        {
            towers.push_back(tower);
            tower = StraightTower::StraightTowerData(s, ++i);
        }
        GameData data = { FieldContainer::FieldData(s),  Player::PlayerData(s), Hand::HandData(s),
            enemies, towers };
        _controller.Initialize(data);
        in.close();
        return true;
    }
    std::cout << "Произошла непредвиденная ошибка открытия файла." << std::endl;
    in.close();
    return false;
}

bool SystemLoadController::PrintSaves()
{
    int i = 0;
    std::filesystem::path directory("Saves");
    if (std::filesystem::exists(directory))
    {
        for (const auto& entry : std::filesystem::directory_iterator(directory))
            std::cout << entry.path().filename() << " - " << i++ << std::endl;
    }
    if (i == 0)
        std::cout << "Сохранений пока нет." << std::endl;
    return i > 0;
}

void SystemLoadController::PrintIntroduction()
{
    std::cout << "=========МЕНЮ=========" << std::endl;
    std::cout << "[Доступные команды] (без скобок): [c1] - начать новую игру, [c2] - продолжить текущую игру, "
                 "[с3] - сохранить текущую игру, [c4] - загрузить игру, любое другое сочетание - "
                 "выйти из игры." << std::endl;
}

int SystemLoadController::ToCommand(std::string& input)
{
    if (input == "c1")
        return 1;
    if (input == "c2")
        return 2;
    if (input == "c3")
        return 3;
    if (input == "c4")
        return 4;
    return 5;
}