#pragma once
#include <deque>

class NotifiyPopup
{
private:
	struct Notification
	{
		std::string message;
		double timer;
	};

	static inline const float m_fOffset = 10.0f;
	static inline const float m_fDuration = 3.0f;
	static inline const size_t m_nMaxDisplayCount = 5;
	static inline std::deque<Notification> m_Messages;

	static bool IsMessagesInQueue();
	static size_t GetQueueSize();

public:
	NotifiyPopup() = delete;
	NotifiyPopup(NotifiyPopup&) = delete;
	void operator=(NotifiyPopup const&) = delete;

	static void Draw();
	static void AddToQueue(std::string&& message);
};

