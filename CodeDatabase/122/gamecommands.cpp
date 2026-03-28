#include "gamecommands.h"

MoveCommand::MoveCommand(int x, int y) : m_dx(x), m_dy(y) {}

GameState MoveCommand::execute(GameModel &gameModel)
{
    auto& player = gameModel.getPlayers().front();
    bool canMove = true;

    if (player->getEffect() == Player::DELAYED)
    {
        player->setEffect(Player::NO_EFFECT);
        canMove = false;
    }
    if (canMove)
    {
        int oldXPos = player->getXPos();
        int oldYPos = player->getYPos();
        CellType oldCell = gameModel.getBoard().currentCell(oldXPos, oldYPos).getType();
        if (gameModel.moveCharacter(*player, gameModel.getBoard(), m_dx, m_dy))
            EventDispatcher::instance().notify(MoveEvent(Actor::PLAYER, oldCell, {oldXPos, oldYPos}, {oldXPos + m_dx, oldYPos + m_dy}));
    }
    return GameState::PLAYING_STATE;
}



GameState AttackCommand::execute(GameModel &gameModel)
{
    gameModel.attackEnemies();
    return GameState::PLAYING_STATE;
}



BuyCardCommand::BuyCardCommand(CardType type) : m_type(type) {}

GameState BuyCardCommand::execute(GameModel &gameModel)
{
    auto& hands = gameModel.getPlayers().front()->getHands();
    hands.buyCard(m_type);
    int newCount = hands.getCountCards(m_type);
    EventDispatcher::instance().notify(BuyCardEvent(m_type, newCount));
    return GameState::PLAYING_STATE;
}



GameState ChangeTypeBattleCommand::execute(GameModel &gameModel)
{
    gameModel.getPlayers().front()->changeTypeBattle();
    return GameState::PLAYING_STATE;
}



UseDamageCardCommand::UseDamageCardCommand(CardType type) : m_type(type ){}

GameState UseDamageCardCommand::execute(GameModel &gameModel)
{
    auto& hands = gameModel.getPlayers().front()->getHands();

    if (!hands.hasCard(m_type)) return GameState::PLAYING_STATE;

    if (hands.inUpgradeMode())
    {
        UpgradeSpellConfig config(hands.getCard(m_type));
        hands.useCard(CardType::UPGRADE_CARD, config);
        hands.setUpgradeMode(false);
    }
    else
    {
        hands.setSelectedSpell(m_type);
        hands.setTargetMode(true);
    }
    return GameState::PLAYING_STATE;
}



GameState UseUpgradeCardCommand::execute(GameModel &gameModel)
{
    auto& hands = gameModel.getPlayers().front()->getHands();

    if (hands.inUpgradeMode() && hands.getCountCards(CardType::UPGRADE_CARD) >= 2)
    {
        UpgradeSpellConfig config(hands.getCard(CardType::UPGRADE_CARD));
        hands.useCard(CardType::UPGRADE_CARD, config);
        hands.setUpgradeMode(false);
    }

    if (hands.hasCard(CardType::UPGRADE_CARD))
    {
        hands.setUpgradeMode(true);
    }
    return GameState::PLAYING_STATE;
}



UseSpellCommand::UseSpellCommand(std::pair<int, int> targetPos, int widgetSize) : m_targetPos(targetPos), m_widgetSize(widgetSize) {}

GameState UseSpellCommand::execute(GameModel &gameModel)
{
    auto& hands = gameModel.getPlayers().front()->getHands();
    if (!hands.inTargetMode()) return GameState::PLAYING_STATE;

    int boardSize = gameModel.getBoard().getSize();
    int cellSize = m_widgetSize / boardSize;
    int boardX = m_targetPos.first / cellSize;
    int boardY = m_targetPos.second / cellSize;

    if (boardX < 0 || boardX >= boardSize || boardY < 0 || boardY >= boardSize) return GameState::PLAYING_STATE;

    PlayerSpellConfig config(&gameModel, {boardX, boardY});
    int oldCount = hands.getCountCards(hands.getSelectedSpell());
    hands.useCard(hands.getSelectedSpell(), config);
    int newCount = hands.getCountCards(hands.getSelectedSpell());
    if (oldCount != newCount)
    {
        EventDispatcher::instance().notify(UseSpellEvent(hands.getSelectedSpell(), newCount));
    }
    hands.setSelectedSpell(CardType::NONE);
    hands.setTargetMode(false);
    gameModel.isLogicActive() = false;
    return GameState::PLAYING_STATE;
}



GameState SaveGameCommand::execute(GameModel &gameModel)
{
    try
    {
        serializer.save(gameModel, "save.json");
        gameModel.isLogicActive() = false;
    }
    catch (const SaveLoadException& e)
    {
        qDebug() << e.what();
    }
    return GameState::PLAYING_STATE;
}

GameSerializer SaveGameCommand::serializer;



GameState LoadGameCommand::execute(GameModel &gameModel)
{
    try
    {
        serializer.load(gameModel, "save.json");
    }
    catch (const SaveLoadException& e)
    {
        qDebug() << e.what();
    }
    return GameState::PLAYING_STATE;
}

GameSerializer LoadGameCommand::serializer;



GameState NewGameCommand::execute(GameModel &gameModel)
{
    gameModel.getLevel() = 0;
    gameModel.newLevel(CharacterConfig::Health, CharacterConfig::Mana);
    return GameState::PLAYING_STATE;
}



GameState UpgradeHealthCommand::execute(GameModel &gameModel)
{
    gameModel.newLevel(CharacterConfig::Health + 50, gameModel.getPlayers().front()->getHands().getMana() + 50); //buff
    return GameState::PLAYING_STATE;
}



GameState UpgradeCardCommand::execute(GameModel &gameModel)
{
    gameModel.newLevel(CharacterConfig::Health, gameModel.getPlayers().front()->getHands().getMana() + 50);
    gameModel.getPlayers().front()->getHands().upgradeRandomCard();
    return GameState::PLAYING_STATE;
}
