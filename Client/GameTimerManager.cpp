#include "GameTimerManager.h"

GameTimerManager::GameTimerManager(QLCDNumber* timerDisplay, QObject* parent)
    : QObject(parent)
    , m_timerDisplay(timerDisplay)
    , m_updateTimer(std::make_unique<QTimer>(this))
{
    connect(m_updateTimer.get(), &QTimer::timeout, this, &GameTimerManager::UpdateTimerTick);
}

void GameTimerManager::StartUpdateTimer(int intervalMs)
{
    if (m_updateTimer) {
        m_updateTimer->start(intervalMs);
    }
}

void GameTimerManager::StopUpdateTimer()
{
    if (m_updateTimer && m_updateTimer->isActive()) {
        m_updateTimer->stop();
    }
}

bool GameTimerManager::IsUpdateTimerActive() const noexcept
{
    return m_updateTimer && m_updateTimer->isActive();
}

void GameTimerManager::StartCountdownTimer(int seconds, const QString& message, std::function<void()> onComplete)
{
    StopUpdateTimer();
    int time = seconds;

    QTimer* countdownTimer = new QTimer(this);
    connect(countdownTimer, &QTimer::timeout, [=]() mutable {
        if (m_timerDisplay) {
            m_timerDisplay->display(time);
        }
        time--;

        if (time <= 0) {
            countdownTimer->stop();
            countdownTimer->deleteLater();
            if (onComplete) {
                onComplete();
            }
        }
    });

    countdownTimer->start(1000);
}

void GameTimerManager::UpdateTimeDisplay(int seconds) noexcept
{
    if (m_timerDisplay) {
        m_timerDisplay->display(seconds);
    }
}

void GameTimerManager::UpdateTimeDisplay(const QString& timeText) noexcept
{
    if (m_timerDisplay) {
        m_timerDisplay->display(timeText);
    }
}

int GameTimerManager::GetCurrentDisplayTime() const noexcept
{
    return m_timerDisplay ? m_timerDisplay->intValue() : 0;
}
