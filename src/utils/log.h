#pragma once

#include "spdlog/spdlog.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "spdlog/fmt/ostr.h"

class Log
{
    static std::shared_ptr<spdlog::logger> console_logger_;
    static std::shared_ptr<spdlog::logger> file_logger_;
	
public:
	
	Log(){}
	
	static void Init();

    static std::shared_ptr<spdlog::logger>& GetConsoleLogger()
	{ 
        return console_logger_;
	}
    static std::shared_ptr<spdlog::logger>& GetFileLogger()
    {
        return file_logger_;
    }
};



#define LOG_INFO(...)     Log::GetConsoleLogger()->info(__VA_ARGS__)
#define LOG_ENGINE(...)     Log::GetConsoleLogger()->info(__VA_ARGS__)
#define LOG_DEBUG(...)     Log::GetFileLogger()->info(__VA_ARGS__)
#define LOG(...)     Log::GetConsoleLogger()->info(__VA_ARGS__)


