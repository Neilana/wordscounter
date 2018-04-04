#pragma once

#include <spdlog/spdlog.h>
#include <memory>

#ifdef QT_DEBUG
#define WCNT_LOG(...)                                   \
    {                                                   \
        g_logger.get()->debug(__VA_ARGS__);             \
        g_logger.get()->flush_on(spdlog::level::debug); \
    }
#else
#define WCNT_LOG(...)
#endif

extern std::shared_ptr<spdlog::logger> g_logger;
