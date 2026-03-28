#ifndef GAMESERIALIZER_H
#define GAMESERIALIZER_H

#include <string>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "gamemodel.h"
#include "poisontrap.h"
#include "healthpoint.h"
#include "saveloadexceptions.h"
#include "jsonfile.h"

class GameSerializer
{
public:
    GameSerializer() = default;

    void save(GameModel& model, const std::string& path);
    void load(GameModel& model, const std::string& path);

private:
    QJsonObject serializePlayer(GameModel& model) const;
    void deserializePlayer(GameModel& model, const QJsonObject& j);

    QJsonArray serializeEnemies(GameModel& model) const;
    void deserializeEnemies(GameModel& model, const QJsonArray& j);

    QJsonArray serializeArchers(GameModel& model) const;
    void deserializeArchers(GameModel& model, const QJsonArray& j);

    QJsonObject serializeBoard(GameModel& model) const;
    void deserializeBoard(GameModel& model, const QJsonObject& j);

    QJsonObject serializeTower(GameModel& model) const;
    void deserializeTower(GameModel& model, const QJsonObject& j);

    QJsonObject serializeBuilding(GameModel& model) const;
    void deserializeBuilding(GameModel& model, const QJsonObject& j);
};

#endif // GAMESERIALIZER_H
