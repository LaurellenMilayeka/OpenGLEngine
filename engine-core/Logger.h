#pragma once

#include "File.h"
#include <filesystem>

struct Log
{
private:

	Engine::Filesystem::File logFile;

	Log(std::string const& path)
		: logFile(path)
	{
		if (!std::filesystem::exists("./log/"))
		{
			std::filesystem::create_directories("./log/");
		}
	}

public:
	static Log* GetInstance()
	{
		static Log* log = new Log("./log/Engine.log");
		return log;
	}

	void Write(std::string const& msg)
	{
		logFile.Write(msg + "\n");
	}
};

#define LOG_INFO(x) Log::GetInstance()->Write(std::string("[") + std::string(__FUNCTION__) + std::string(":") + std::to_string(__LINE__) + std::string("][INFO] : ") + std::string(x))
#define LOG_ERROR(x) Log::GetInstance()->Write(std::string("[") + std::string(__FUNCTION__) + std::string(":") + std::to_string(__LINE__) + std::string("][ERR ] : ") + std::string(x))
#define LOG_SUCCESS(x) Log::GetInstance()->Write(std::string("[") + std::string(__FUNCTION__) + std::string(":") + std::to_string(__LINE__) + std::string("][SUCC] : ") + std::string(x))
#define LOG_WARN(x) Log::GetInstance()->Write(std::string("[") + std::string(__FUNCTION__) + std::string(":") + std::to_string(__LINE__) + std::string("][WARN] : ") + std::string(x))

#ifndef NDEBUG
#define LOG_DEBUG(x) Log::GetInstance()->Write(std::string("[") + std::string(__FUNCTION__) + std::string(":") + std::to_string(__LINE__) + std::string("][DEBG] : ") + std::string(x))
#elif
#define LOG_DEBUG(x)
#endif
