#include "qtaudiomanager.h"
#include "eventdispatcher.h"
#include <QDebug>

QTAudioManager::QTAudioManager() : m_player(new QMediaPlayer(this)), m_audioOutput(new QAudioOutput(this))
{
    m_player->setAudioOutput(m_audioOutput);

    EventDispatcher::instance().subscribe<AttackEvent>(
        [this](const AttackEvent &e)
        { onAttack(e); });

    EventDispatcher::instance().subscribe<UseSpellEvent>(
        [this](const UseSpellEvent& e)
        { onUseSpell(e); });

    EventDispatcher::instance().subscribe<LevelLoadedEvent>(
        [this](const LevelLoadedEvent& e)
        {onLevelLoaded(e); });
}

void QTAudioManager::stopAllSounds()
{
    m_player->stop();
}

void QTAudioManager::soundInitState()
{
    m_audioOutput->setVolume(0.7);
    m_player->setSource(QUrl("qrc:/resources/levelComplete.mp3"));
    m_player->play();
}

void QTAudioManager::soundLevelComplete()
{
    m_audioOutput->setVolume(0.7);
    m_player->setSource(QUrl("qrc:/resources/levelComplete.mp3"));
    m_player->play();
}

void QTAudioManager::soundLevelFailed()
{
    m_audioOutput->setVolume(0.5);
    m_player->setSource(QUrl("qrc:/resources/levelFailed.mp3"));
    m_player->play();
}

void QTAudioManager::onAttack(const AttackEvent& event)
{
    m_audioOutput->setVolume(1.0);
    if (event.m_damage > 0)
    {
        m_player->setSource(QUrl("qrc:/resources/attack.wav"));
        m_player->play();
    }
}

void QTAudioManager::onUseSpell(const UseSpellEvent &)
{
    m_audioOutput->setVolume(1.0);
    m_player->setSource(QUrl("qrc:/resources/useSpell.mp3"));
    m_player->play();
}

void QTAudioManager::onLevelLoaded(const LevelLoadedEvent &)
{
    m_audioOutput->setVolume(1.0);
    m_player->setSource(QUrl("qrc:/resources/loadedLevel.mp3"));
    m_player->play();
}
