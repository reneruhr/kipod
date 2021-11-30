#pragma once

#include "spdlog/spdlog.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "spdlog/fmt/ostr.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include <spdlog/fmt/bundled/format.h>

template<>
struct fmt::formatter<glm::mat4> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template <typename FormatContext>
    auto format(const glm::mat4& m, FormatContext& ctx) -> decltype(ctx.out()) {
        return format_to(ctx.out(),
                         "{}",
                         glm::to_string(m));
    }
};

template<>
struct fmt::formatter<glm::vec3> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template <typename FormatContext>
    auto format(const glm::vec3& m, FormatContext& ctx) -> decltype(ctx.out()) {
        return format_to(ctx.out(),
                         "{}",
                         glm::to_string(m));
    }
};

template<>
struct fmt::formatter<glm::vec4> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template <typename FormatContext>
    auto format(const glm::vec4& m, FormatContext& ctx) -> decltype(ctx.out()) {
        return format_to(ctx.out(),
                         "{}",
                         glm::to_string(m));
    }
};


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


