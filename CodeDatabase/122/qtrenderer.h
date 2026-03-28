#ifndef QTRENDERER_H
#define QTRENDERER_H

#include <QPainter>
#include <QObject>

#include "events.h"

class QTRenderer : public QObject
{
public:
    QTRenderer();

    bool eventFilter(QObject* obj, QEvent* event) override;

    void renderPlayingState();
    void renderInitState();
    void renderLevelComplete();
    void renderLevelFailed();

private:
    void loadTextures();
    void drawSpellBar(QPainter &plt, int boardHeight, int spellBarHeight);

    void onMove(const MoveEvent& event);
    // void onAttack(const AttackEvent& event);
    void onDead(const DeadEvent& event);
    // void onUseSpell(const UseSpellEvent& event);
    void onLevelLoaded(const LevelLoadedEvent& event);
    void onNewCell(const NewCellEvent& event);
    void onWin(const WinEvent& event);
    void pause(int ms);

    QImage m_scene;
    QWidget* m_widget;
    std::map<Actor, QImage> m_actorTextures;
    std::map<CellType, QImage> m_cellTextures;
    std::map<Bar, QImage> m_spellTextures;
    bool m_sceneInitialized;
    int widthWidget;
    int heightWidget;
    int boardSize;
    int spellBarY;
    int m_cellPixels;
    int cardSize;
    int cardSpacing;
    int startX;
    int startY;
};

#endif // QTRENDERER_H
