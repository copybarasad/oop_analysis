#ifndef GAMECOMMANDS_H
#define GAMECOMMANDS_H

#include "constants.h"
#include "gamemodel.h"
#include "upgradespellconfig.h"
#include "playerspellconfig.h"
#include "gameserializer.h"

class GameCommand
{
public:
    virtual ~GameCommand() = default;
    virtual GameState execute(GameModel& gameModel) = 0;
};



class MoveCommand : public GameCommand
{
public:
    MoveCommand(int x, int y);

    GameState execute(GameModel& gameModel) override;

private:
    int m_dx, m_dy;
};



class AttackCommand : public GameCommand
{
public:
    GameState execute(GameModel& gameModel) override;
};



class BuyCardCommand : public GameCommand
{
public:
    BuyCardCommand(CardType type);

    GameState execute(GameModel& gameModel) override;

private:
    CardType m_type;
};



class ChangeTypeBattleCommand : public GameCommand
{
public:
    GameState execute(GameModel& gameModel) override;
};



class UseDamageCardCommand : public GameCommand
{
public:
    UseDamageCardCommand(CardType type);

    GameState execute(GameModel& gameModel) override;

private:
    CardType m_type;
};



class UseUpgradeCardCommand : public GameCommand
{
public:
    GameState execute(GameModel& gameModel) override;
};



class UseSpellCommand : public GameCommand
{
public:
    UseSpellCommand(std::pair<int,int> targetPos, int widgetSize);

    GameState execute(GameModel& gameModel) override;

private:
    std::pair<int,int> m_targetPos;
    int m_widgetSize;
};



class SaveGameCommand : public GameCommand
{
public:
    GameState execute(GameModel& gameModel) override;

private:
    static GameSerializer serializer;
};



class LoadGameCommand : public GameCommand
{
public:
    GameState execute(GameModel& gameModel) override;

private:
    static GameSerializer serializer;
};



class NewGameCommand : public GameCommand
{
public:
    GameState execute(GameModel& gameModel) override;
};



class UpgradeHealthCommand : public GameCommand
{
public:
    GameState execute(GameModel& gameModel) override;
};



class UpgradeCardCommand : public GameCommand
{
public:
    GameState execute(GameModel& gameModel) override;
};

#endif
