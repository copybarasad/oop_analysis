#include "JsonManager.h"


JsonManager::FileInputWrapper::FileInputWrapper(const std::string& filename) : file(filename) 
{
    if (!file)
        throw std::runtime_error(u8"Не удалось открыть файл для записи: " + filename);
}


std::ofstream& JsonManager::FileInputWrapper::get()
{
    return file;
}


JsonManager::FileInputWrapper::~FileInputWrapper()
{
    if (file.is_open())
        file.close();
}


JsonManager::FileOutputWrapper::FileOutputWrapper(const std::string& filename) : file(filename)
{
    if (!file.is_open())
        throw std::runtime_error(u8"Файл не найден: " + filename);
}


std::ifstream& JsonManager::FileOutputWrapper::get()
{
    return file;
}


JsonManager::FileOutputWrapper::~FileOutputWrapper()
{
    if (file.is_open())
        file.close();
}


JsonManager::JsonManager(const std::string & filename) : filename(filename) {}


void JsonManager::jsonDump(const GameBoard& board, unsigned short level) {

    FileInputWrapper writer(filename);
    
    auto& file = writer.get();

    nlohmann::json js = dumpGameBoard(board, level);

    file << js.dump(4);
}


nlohmann::json JsonManager::jsonLoad()
{
    FileOutputWrapper reader(filename);

    auto& file = reader.get();

    nlohmann::json js;
    file >> js;
    return js;
}


nlohmann::json JsonManager::dumpGameBoard(const GameBoard& board, unsigned short level)
{
    nlohmann::json js;

    js["level"] = level;

    if (board.getPlayer())
    {
        nlohmann::json jPlayer;
        board.getPlayer()->to_json(jPlayer);
        js["Player"] = jPlayer;
    }

    if (board.getField())
    {
        nlohmann::json jField;
        board.getField()->to_json(jField);
        js["Field"] = jField;
    }

    js["Enemies"] = nlohmann::json::array();

    for (auto& enemy : board.getEnemies())
    {
        nlohmann::json jEnemy;
        enemy.to_json(jEnemy);
        js["Enemies"].push_back(jEnemy);
    }

    if (board.getTower())
    {
        nlohmann::json jTower;
        board.getTower()->to_json(jTower);
        js["Tower"] = jTower;
    }

    if (board.getArcherTower())
    {
        nlohmann::json jArcherTower;
        board.getArcherTower()->to_json(jArcherTower);
        js["ArcherTower"] = jArcherTower;
    }

    js["Allies"] = nlohmann::json::array();

    for (auto& ally : board.getAllies())
    {
        nlohmann::json jAlly;
        ally.to_json(jAlly);
        js["Allies"].push_back(jAlly);
    }

    js["Traps"] = nlohmann::json::array();

    for (auto& trap : board.getTraps())
    {
        nlohmann::json jTrap;
        trap.to_json(jTrap);
        js["Traps"].push_back(jTrap);
    }

    return js;
}