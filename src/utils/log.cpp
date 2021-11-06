#include "log.h"

std::shared_ptr<spdlog::logger> Log::console_logger_;
std::shared_ptr<spdlog::logger> Log::file_logger_;


void Log::Init()
	{
        std::vector<spdlog::sink_ptr> log_sink_console;
        log_sink_console.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        log_sink_console.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/logfile.log", true));

        std::vector<spdlog::sink_ptr> log_sink_file;
        log_sink_file.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/logfile.log", true));



        log_sink_console[0]->set_pattern("%^[%T] %n: %v%$");
        log_sink_console[1]->set_pattern("[%T] [%l] %n: %v");

        log_sink_file[0]->set_pattern("[%T] [%l] %n: %v");

        console_logger_ = std::make_shared<spdlog::logger>("Console Logger", begin(log_sink_console), end(log_sink_console));
        spdlog::register_logger(console_logger_);
        console_logger_->set_level(spdlog::level::trace);
        console_logger_->flush_on(spdlog::level::trace);

        file_logger_ = std::make_shared<spdlog::logger>("File Logger", begin(log_sink_file), end(log_sink_file));
        spdlog::register_logger(file_logger_);
        file_logger_->set_level(spdlog::level::trace);
        file_logger_->flush_on(spdlog::level::trace);
	}
