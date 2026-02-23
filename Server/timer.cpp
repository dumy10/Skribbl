module timer;

using namespace skribbl;

Timer::Timer(int startTime) : m_timeLeft{ startTime }, m_startTime{ startTime }
{
	// Empty
}

Timer::Timer(Timer&& other) noexcept
	: m_startTime{ other.m_startTime }
{
	// Stop the source timer first
	other.StopTicking();

	// Transfer state
	m_timeLeft.store(other.m_timeLeft.exchange(kMaximumTimeLeft));
	m_isTicking.store(false); // Both should be stopped
	m_timerWorkerThread = std::move(other.m_timerWorkerThread);
}

Timer& Timer::operator=(Timer&& other) noexcept
{
	if (this != &other) {
		StopTicking();

		m_timeLeft = other.m_timeLeft.load();
		m_isTicking = other.m_isTicking.load();
		m_timerWorkerThread = std::move(other.m_timerWorkerThread);
		m_startTime = other.m_startTime;
	}
	return *this;
}

const int Timer::GetTimeLeft() const noexcept
{
	return m_timeLeft;
}

void Timer::StartTicking()
{
    if (m_isTicking.exchange(true)) {
        return; // exchange(true) returns the OLD value; if it was already true, exit.
    }

    m_timerWorkerThread = std::jthread([this](std::stop_token stoken) {
        while (!stoken.stop_requested() && m_timeLeft > kMinimumTimeLeft) {
            // Using wait_for on a condition variable with a stop_token 
            // is the "pro" way to sleep while remaining interruptible.
            // For simplicity here, we stick to sleep, but check the token:

            std::this_thread::sleep_for(std::chrono::milliseconds(kTimerInterval));

            if (stoken.stop_requested()) break;

            m_timeLeft -= kTimeDecrement;

            if (m_timeLeft <= kMinimumTimeLeft) {
                m_timeLeft = kMinimumTimeLeft;
                m_isTicking = false;
                break;
            }
        }
        m_isTicking = false;
        });
}

void Timer::StopTicking()
{
	m_isTicking = false;
	if (m_timerWorkerThread.joinable()) {
		m_timerWorkerThread.request_stop();
	}
}

void Timer::ResetTimer()
{
	m_timeLeft = m_startTime;
}


