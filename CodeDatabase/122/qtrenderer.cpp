#include <QWidget>
#include <QApplication>
#include <QTimer>
#include <iostream>
#include "qtrenderer.h"
#include "eventdispatcher.h"


QTRenderer::QTRenderer()
{
    qApp->installEventFilter(this);
    loadTextures();

    EventDispatcher::instance().subscribe<MoveEvent>(
        [this](const MoveEvent &e)
        { onMove(e); });

    // EventDispatcher::instance().subscribe<AttackEvent>(
    //     [this](const AttackEvent &e)
    //     { onAttack(e); });

    // EventDispatcher::instance().subscribe<UseSpellEvent>(
    //     [this](const UseSpellEvent &e)
    //     { onUseSpell(e); });

    EventDispatcher::instance().subscribe<LevelLoadedEvent>(
        [this](const LevelLoadedEvent &e)
        { onLevelLoaded(e); });

    EventDispatcher::instance().subscribe<NewCellEvent>(
        [this](const NewCellEvent &e)
        { onNewCell(e); });

    EventDispatcher::instance().subscribe<DeadEvent>(
        [this](const DeadEvent &e)
        { onDead(e); });

    EventDispatcher::instance().subscribe<WinEvent>(
        [this](const WinEvent &e)
        { onWin(e); });
}

bool QTRenderer::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() != QEvent::Paint)
        return QObject::eventFilter(obj, event);

    QWidget* widget = qobject_cast<QWidget*>(obj);

    if (!widget)
        return false;

    if (!m_sceneInitialized)
    {
        m_widget = widget;
        qreal dpr = widget->devicePixelRatioF();

        QSize logicalSize = widget->size();
        QSize backingSize = logicalSize * dpr;

        m_scene = QImage(backingSize, QImage::Format_ARGB32_Premultiplied);
        m_scene.setDevicePixelRatio(dpr);
        m_scene.fill(Qt::transparent);

        m_sceneInitialized = true;
        widthWidget = widget->width();
        heightWidget = widget->height();
        renderInitState();
    }
    QPainter painter(widget);
    painter.drawImage(0, 0, m_scene);

    return true;
}

void QTRenderer::loadTextures()
{
    m_actorTextures[Actor::PLAYER] = QImage(":/resources/player.jpg");
    m_actorTextures[Actor::ENEMY] = QImage(":/resources/enemy.jpg");
    m_actorTextures[Actor::ARCHER] = QImage(":/resources/archer.PNG");
    m_actorTextures[Actor::DARK_TOWER] = QImage(":/resources/tower.PNG");
    m_actorTextures[Actor::BUILDING] = QImage(":/resources/building.PNG");

    m_cellTextures[CellType::SLOWED] = QImage(":/resources/water.PNG");
    m_cellTextures[CellType::BLOCKED] = QImage(":/resources/block.jpg");
    m_cellTextures[CellType::POISON_TRAP] = QImage(":/resources/poisonTrapCell.jpg");
    m_cellTextures[CellType::HEALTH_POINT] = QImage(":/resources/healthPoint.PNG");
    m_cellTextures[CellType::ORIGINAL] = QImage(":/resources/map.jpg");

    m_spellTextures[Bar::FIREBALL] = QImage(":/resources/fireballCard.jpg");
    m_spellTextures[Bar::EARTHQUAKE] = QImage(":/resources/earthquakeCard.jpg");
    m_spellTextures[Bar::POISON_TRAP] = QImage(":/resources/trapCard.PNG");
    m_spellTextures[Bar::SUMMON_ARCHER] = QImage(":/resources/archerCard.jpg");
    m_spellTextures[Bar::UPGRADE_CARD] = QImage(":/resources/upgradeCard.jpg");
    m_spellTextures[Bar::HEALTH_PLAYER] = QImage(":/resources/healthPlayer.jpg");
    m_spellTextures[Bar::HEALTH_TOWER] = QImage(":/resources/healthTower.jpg");
    m_spellTextures[Bar::MANA] = QImage(":/resources/mana.PNG");
}


void QTRenderer::renderPlayingState()
{
    // QPoint mousePos = m_targetWidget->mapFromGlobal(QCursor::pos());
    // QPainter painter(&m_scene);
    // int boardX = mousePos.x() / m_cellPixels;
    // int boardY = mousePos.y() / m_cellPixels;

    // if (boardX >= 0 && boardX < boardSize &&
    //     boardY >= 0 && boardY < boardSize)
    // {
    //     painter.setBrush(QBrush(QColor(105, 105, 105, 180)));
    //     painter.setPen(QPen(Qt::gray, 1));
    //     painter.drawRect(boardX * m_cellPixels, boardY * m_cellPixels, m_cellPixels, m_cellPixels);
    // }
}

void QTRenderer::renderInitState()
{
    QPainter painter(&m_scene);
    static QImage initState(":/resources/initState.JPEG");

    painter.drawImage(QRect(0, 0, widthWidget, heightWidget), initState);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Papyrus", 18));
    painter.drawText(QRect(0, heightWidget / 4, widthWidget, heightWidget), Qt::AlignCenter,
                     "WARRIOR\nWelcome to the game.\nPress S to continue.\nPress N to start a new game.");
}

void QTRenderer::renderLevelComplete()
{
    QPainter painter(&m_scene);
    static QImage initState(":/resources/initState.JPEG");

    painter.drawImage(QRect(0, 0, widthWidget, heightWidget), initState);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Papyrus", 18));

    painter.drawText(QRect(0, heightWidget / 4, widthWidget, heightWidget), Qt::AlignCenter,
                     "Level complete!\nPress H to improve health\n"
                     "Press U to improve random card");
}

void QTRenderer::renderLevelFailed()
{
    QPainter painter(&m_scene);

    static QImage initState(":/resources/levelFailed.JPEG");

    painter.drawImage(QRect(0, 0, widthWidget, heightWidget), initState);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Papyrus", 18));

    painter.drawText(QRect(0, heightWidget / 4 + 25, widthWidget - 50, heightWidget), Qt::AlignCenter,
                     "Level failed\n"
                     "Press N to start new game\n");
}

void QTRenderer::onMove(const MoveEvent &event)
{
    QPainter painter(&m_scene);
    painter.drawImage(QRect(event.m_oldPos.first * m_cellPixels, event.m_oldPos.second * m_cellPixels, m_cellPixels, m_cellPixels), m_cellTextures[event.m_oldCell]);
    painter.drawImage(QRect(event.m_newPos.first * m_cellPixels, event.m_newPos.second * m_cellPixels, m_cellPixels, m_cellPixels), m_actorTextures[event.m_actor]);
    // m_widget->update();
    // pause(300);
}

// void QTRenderer::onAttack(const AttackEvent &event)
// {
//     QPainter painter(&m_scene);
//     painter.setPen(Qt::white);
//     painter.setFont(QFont("Papyrus", 12));
//     int newHealth = ((event.m_oldHealth - event.m_damage)) >= 0 ? event.m_oldHealth - event.m_damage : 0;
//     switch (event.m_target) {
//     case Actor::PLAYER:
//         painter.drawText(startX + 5 * (cardSize + cardSpacing) + cardSize / 2 - 10, startY + cardSize + 5, QString("%1").arg(newHealth));
//         break;
//     case Actor::DARK_TOWER:
//         painter.drawText(startX + 6 * (cardSize + cardSpacing) + cardSize / 2 - 10, startY + cardSize + 5, QString("%1").arg(newHealth));
//     case Actor::ENEMY:
//     default:
//         break;
//     }
// }

void QTRenderer::onDead(const DeadEvent &event)
{
    QPainter painter(&m_scene);
    switch (event.m_actor) {
    case Actor::DARK_TOWER:
        painter.drawImage(QRect(event.m_targetPos.first * m_cellPixels, event.m_targetPos.second * m_cellPixels, m_cellPixels * 2, m_cellPixels * 2), m_cellTextures[CellType::ORIGINAL]);
        break;
    default:
        painter.drawImage(QRect(event.m_targetPos.first * m_cellPixels, event.m_targetPos.second * m_cellPixels, m_cellPixels, m_cellPixels), m_cellTextures[CellType::ORIGINAL]);
        break;
    }
}

void QTRenderer::onLevelLoaded(const LevelLoadedEvent& event)
{
    m_scene.fill(Qt::transparent);
    QPainter painter(&m_scene);

    int spellBarHeight = 100;

    boardSize = event.m_newBoardSize;
    spellBarY = boardSize;
    m_cellPixels = widthWidget / boardSize;

    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            painter.drawImage(QRect(i * m_cellPixels, j * m_cellPixels, m_cellPixels, m_cellPixels), m_cellTextures[CellType::ORIGINAL]);
        }
    }

    painter.drawImage(QRect((boardSize - 2) * m_cellPixels, 0, m_cellPixels * 2, m_cellPixels * 2), m_actorTextures[Actor::BUILDING]);
    painter.drawImage(QRect(0, 0, m_cellPixels * 2, m_cellPixels * 2), m_actorTextures[Actor::DARK_TOWER]);
    drawSpellBar(painter, spellBarY, spellBarHeight);
}

void QTRenderer::onNewCell(const NewCellEvent &event)
{
    QPainter painter(&m_scene);
    painter.drawImage(QRect(event.m_coord.first * m_cellPixels, event.m_coord.second * m_cellPixels, m_cellPixels, m_cellPixels), m_cellTextures[event.m_type]);
    painter.drawImage(QRect(0, 0, m_cellPixels * 2, m_cellPixels * 2), m_actorTextures[Actor::DARK_TOWER]);
    painter.drawImage(QRect((boardSize - 2) * m_cellPixels, 0, m_cellPixels * 2, m_cellPixels * 2), m_actorTextures[Actor::BUILDING]);
}

void QTRenderer::onWin(const WinEvent &)
{
    QPainter painter(&m_scene);
    static QImage initState(":/resources/initState.JPEG");

    painter.drawImage(QRect(0, 0, widthWidget, heightWidget), initState);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Papyrus", 18));
    painter.drawText(QRect(0, heightWidget / 4, widthWidget, heightWidget), Qt::AlignCenter,
                     "WARRIOR\nYou win! :)");
    m_widget->update();
}

void QTRenderer::drawSpellBar(QPainter &painter, int boardHeight, int spellBarHeight)
{
    int spellBarY = boardHeight * m_cellPixels;

    painter.setBrush(QBrush(QColor(56, 35, 26, 100)));
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, spellBarY, widthWidget, spellBarHeight);

    cardSize = 60;
    cardSpacing = 10;
    startX = (widthWidget - (7 * cardSize + 6 * cardSpacing)) / 5;
    startY = spellBarY + 20;


    for (int i = 0; i < 8; i++)
    {
        int cardX = startX + i * (cardSize + cardSpacing);
        int cardY = startY;

        painter.setBrush(QBrush(QColor(56, 35, 26, 100)));
        painter.setPen(QPen(Qt::white, 1));
        painter.drawRect(cardX, cardY, cardSize, cardSize);

        cardX += (i < 5) ? 5 : 0;
        cardY += (i < 5) ? 5 : 0;

        int size = (i < 5) ? cardSize - 11 : cardSize + 2;

        painter.drawImage(QRect(cardX, cardY, size, size),
                      m_spellTextures[static_cast<Bar>(i)]);
    }
}

void QTRenderer::pause(int ms)
{
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

// void QTRenderer::onUseSpell(const UseSpellEvent &event)
// {
//     QPainter painter(&m_scene);
//     for (int i = 0; i < 5; i++)
//     {
//         int cardX = startX + i * (cardSize + cardSpacing);
//         int cardY = startY;

//         CardType currentCard = static_cast<CardType>(i);
//         if (currentCard == event.m_card && event.m_newCount > 0)
//         {
//             painter.setBrush(QBrush(QColor(127, 18, 38, 200)));
//             painter.setPen(Qt::white);
//             painter.setFont(QFont("Papyrus", 12));
//         }
//         else
//         {
//             painter.setBrush(QBrush(QColor(56, 35, 26, 100)));
//         }
//         painter.setPen(QPen(Qt::white, 1));
//         painter.drawRect(cardX, cardY, cardSize, cardSize);

//         painter.drawImage(QRect(cardX + 5, cardY + 5, cardSize - 11, cardSize - 11),
//                       m_spellTextures[static_cast<Bar>(i)]);

//         if (currentCard == event.m_card)
//             painter.drawText(startX + i * (cardSize + cardSpacing) + cardSize / 2 - 10, startY + cardSize + 5, QString("%1").arg(event.m_newCount));
//     }
// }
