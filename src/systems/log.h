#pragma once

#include "system.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace blimp
{

class Log : public System<Log>
{
public:
    virtual void init() override
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        logger_main = spdlog::stdout_color_mt("BLIMP");
        logger_main->set_level(spdlog::level::trace);
    }

    virtual void shutdown() override
    {
    }

    spdlog::logger *logger()
    {
        return logger_main.get();
    }

private:
    std::shared_ptr<spdlog::logger> logger_main;
};

} // namespace blimp

#define BLIMP_LOG(...) blimp::Log::instance()->logger()->trace(__VA_ARGS__)
#define BLIMP_WARN(...) blimp::Log::instance()->logger()->warn(__VA_ARGS__)
#define BLIMP_ERROR(...) blimp::Log::instance()->logger()->error(__VA_ARGS__)