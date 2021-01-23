#pragma once

#include "spdlog/spdlog.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "spdlog/fmt/ostr.h"

class Log
{
	static std::shared_ptr<spdlog::logger> s_logger;
	
public:
	
	Log(){}
	
	static void Init();

	static std::shared_ptr<spdlog::logger>& getLogger()
	{ 
		return s_logger; 
	}
};

#define LOG_INFO(...)     Log::getLogger()->info(__VA_ARGS__)
#define LOG_ENGINE(...)     Log::getLogger()->info(__VA_ARGS__)
#define LOG_DEBUG(...)     Log::getLogger()->info(__VA_ARGS__)
#define LOG(...)     Log::getLogger()->info(__VA_ARGS__)
