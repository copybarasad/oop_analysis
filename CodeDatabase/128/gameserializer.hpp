#pragma once
#include <string>
#include <nlohmann/json.hpp>

class Game;

class GameSerializer
{
public:
    GameSerializer() = default;

    void SaveGame(Game& game, const std::string& path);
    void LoadGame(Game& game, const std::string& path);

private:
    nlohmann::json SerializePlayer(Game& game);  
    nlohmann::json SerializeField(Game& game);   
    nlohmann::json SerializeEntity(Game& game);   

    void DeserializePlayer(Game& game, const nlohmann::json& j);
    void DeserializeField(Game& game, const nlohmann::json& j);
    void DeserializeEntity(Game& game, const nlohmann::json& j);
};
