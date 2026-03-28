#ifndef QTAUDIOMANAGER_H
#define QTAUDIOMANAGER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "events.h"
#include "eventdispatcher.h"

class QTAudioManager : public QObject
{
    Q_OBJECT

public:
    QTAudioManager();
    ~QTAudioManager() override = default;

    void stopAllSounds();
    void soundInitState();
    void soundLevelComplete();
    void soundLevelFailed();

private:
    void onAttack(const AttackEvent& event);
    void onUseSpell(const UseSpellEvent& event);
    void onLevelLoaded(const LevelLoadedEvent& event);

    QMediaPlayer* m_player;
    QAudioOutput* m_audioOutput;
};

#endif // QTAUDIOMANAGER_H
