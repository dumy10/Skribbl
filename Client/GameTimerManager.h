#ifndef GAMETIMERMANAGER_H
#define GAMETIMERMANAGER_H

#include <QTimer>
#include <QLCDNumber>
#include <QString>
#include <memory>
#include <functional>

/**
 * @brief Manages game timers including the room update timer and countdown timers.
 * 
 * This class handles periodic room updates and provides countdown timer
 * functionality with custom completion callbacks.
 */
class GameTimerManager : public QObject 
{
    Q_OBJECT

public:
    explicit GameTimerManager(QLCDNumber* timerDisplay, QObject* parent = nullptr);
    ~GameTimerManager() = default;

    // Update timer operations
    void StartUpdateTimer(int intervalMs);
    void StopUpdateTimer();
    [[nodiscard]] bool IsUpdateTimerActive() const noexcept;

    // Countdown timer operations
    void StartCountdownTimer(int seconds, const QString& message, std::function<void()> onComplete);

    // Display operations
    void UpdateTimeDisplay(int seconds) noexcept;
    void UpdateTimeDisplay(const QString& timeText) noexcept;
    [[nodiscard]] int GetCurrentDisplayTime() const noexcept;

signals:
    void UpdateTimerTick();

private:
    std::unique_ptr<QTimer> m_updateTimer;
    QLCDNumber* m_timerDisplay;
};

#endif // GAMETIMERMANAGER_H
