export module timer;

import <atomic>;
import <thread>;

namespace skribbl
{
	export class Timer
	{
	public:
		Timer() = default;
		Timer(int startTime);
		~Timer();

		[[nodiscard]] const int GetTimeLeft() const noexcept;

		void StartTicking();
		void StopTicking();
		void ResetTimer();

		// Copy constructor and copy assignment operator
		Timer(const Timer& other) = delete;
		Timer& operator=(const Timer& other) = delete;

		// Move constructor and move assignment operator
		Timer(Timer&& other) noexcept;
		Timer& operator=(Timer&& other) noexcept;

	private:
		std::atomic<int> m_timeLeft{ kMaximumTimeLeft };
		std::atomic<bool> m_isTicking{ false };
		std::jthread m_timerWorkerThread;
		
		int m_startTime{ kMaximumTimeLeft };

	private:
		static constexpr int kTimerInterval = 1000; // Timer interval in milliseconds
		static constexpr int kTimeDecrement = 1; // Time decrement in seconds
		static constexpr int kMinimumTimeLeft = 0; // Minimum time left in seconds
		static constexpr int kMaximumTimeLeft = 60; // Maximum time left in seconds
	};
}
