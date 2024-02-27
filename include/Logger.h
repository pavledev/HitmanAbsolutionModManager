#pragma once

#include <vector>
#include <string>
#include <format>

#include "Mutex.h"

#undef GetCurrentTime

class Logger
{
public:
	enum class Level
	{
		Info,
		Warning,
		Error
	};

	struct Message
	{
		std::string ToString() const
		{
			std::string level = LevelToString(this->level);

			return std::format("ID: {}, Level: {}, Time: {}, Content: {}", id, level, time, content);
		}

		int id;
		Level level;
		std::string content;
		std::string time;
	};

	static Logger& GetInstance();
	static const char* LevelToString(const Level level);
	std::vector<Message>& GetMessages();
	void ClearMessage(const unsigned int index);
	void ClearAllMessages();
	static std::string GetCurrentTime();
	static std::string GetLastError();

	template <typename... Args>
	void Log(const Level level, const std::string& format, const Args&... args)
	{
		ScopedExclusiveGuard scopedSharedGuard = ScopedExclusiveGuard(&srwLock);
		Message message;

		message.id = static_cast<int>(messages.size());
		message.level = level;
		message.content = std::vformat(format, std::make_format_args(args...));
		message.time = GetCurrentTime();

		messages.push_back(message);
	}

private:
	Logger();
	Logger(const Logger& other) = delete;
	Logger& operator=(const Logger& other) = delete;

	SRWLOCK srwLock;
	std::vector<Message> messages;
};
