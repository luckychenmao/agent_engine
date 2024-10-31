#pragma once

#include <iostream>

#include "alog/Configurator.h"
#include "alog/LogStream.h" // IWYU pragma: export
#include "alog/Logger.h"    // IWYU pragma: export
#include "util/TimeUtility.h"

#define ROOT_LOG_CONFIG() alog::Configurator::configureRootLogger()
#define LOG_CONFIG(filename)                                                                                           \
    do {                                                                                                               \
        try {                                                                                                          \
            alog::Configurator::configureLogger(filename);                                                             \
        } catch (std::exception & e) {                                                                                 \
            std::cerr << "WARN! Failed to configure logger!" << e.what() << ",use default log conf." << std::endl;     \
            ROOT_LOG_CONFIG();                                                                                         \
        }                                                                                                              \
    } while (0)

#define LOG_CONFIG_FROM_STRING(content)                                                                                \
    do {                                                                                                               \
        try {                                                                                                          \
            alog::Configurator::configureLoggerFromString(content);                                                    \
        } catch (std::exception & e) {                                                                                 \
            std::cerr << "configure logger use [" << content << "] failed" << std::endl;                               \
            ROOT_LOG_CONFIG();                                                                                         \
        }                                                                                                              \
    } while (0)

#define ROOT_LOG_SETLEVEL(level) alog::Logger::getRootLogger()->setLevel(alog::LOG_LEVEL_##level)

#define LOG_DECLARE() static alog::Logger *_logger

#define LOG_SETUP(n, c) alog::Logger *c::_logger = alog::Logger::getLogger(#n "." #c)

#define LOG_SETUP_TEMPLATE(n, c, T)                                                                                    \
    template <typename T>                                                                                              \
    alog::Logger *c<T>::_logger = alog::Logger::getLogger(#n "." #c)

#define LOG_SETUP_TEMPLATE_2(n, c, T1, T2)                                                                             \
    template <typename T1, typename T2>                                                                                \
    alog::Logger *c<T1, T2>::_logger = alog::Logger::getLogger(#n "." #c)

#define LOG_SETUP_TEMPLATE_3(n, c, T1, T2, T3)                                                                         \
    template <typename T1, typename T2, typename T3>                                                                   \
    alog::Logger *c<T1, T2, T3>::_logger = alog::Logger::getLogger(#n "." #c)

#define LOG_SETUP_TEMPLATE_WITH_TYPENAME(n, c, typename, T)                                                            \
    template <typename T>                                                                                              \
    alog::Logger *c<T>::_logger = alog::Logger::getLogger(#n "." #c)

#define LOG_SETUP_TEMPLATE_WITH_TYPENAME2(n, c, typename1, T1, typename2, T2)                                          \
    template <typename1 T1, typename2 T2>                                                                              \
    alog::Logger *c<T1, T2>::_logger = alog::Logger::getLogger(#n "." #c)

#define LOG(level, format, args...) ALOG_LOG(_logger, alog::LOG_LEVEL_##level, format, ##args)
#define SLOG(level) ALOG_STREAM(_logger, alog::LOG_LEVEL_##level)
#define SLOG_IF(level, condition) ALOG_STREAM_IF(_logger, alog::LOG_LEVEL_##level, condition)
#define SLOG_ASSERT(condition) ALOG_STREAM_ASSERT(_logger, condition)

#define SLOG_EVERY_N(level, n) ALOG_STREAM_EVERY_N(_logger, alog::LOG_LEVEL_##level, n)
#define SLOG_FIRST_N(level, n) ALOG_STREAM_FIRST_N(_logger, alog::LOG_LEVEL_##level, n)
#define SLOG_EVERY_T(level, seconds) ALOG_STREAM_EVERY_T(_logger, alog::LOG_LEVEL_##level, seconds)
#define SLOG_IF_EVERY_N(level, condition, n) ALOG_STREAM_IF_EVERY_N(_logger, alog::LOG_LEVEL_##level, condition, n)

#define CHECK_EQ(level, val1, val2) ALOG_CHECK_EQ(_logger, alog::LOG_LEVEL_##level, val1, val2)
#define CHECK_NE(level, val1, val2) ALOG_CHECK_NE(_logger, alog::LOG_LEVEL_##level, val1, val2)
#define CHECK_LE(level, val1, val2) ALOG_CHECK_LE(_logger, alog::LOG_LEVEL_##level, val1, val2)
#define CHECK_LT(level, val1, val2) ALOG_CHECK_LT(_logger, alog::LOG_LEVEL_##level, val1, val2)
#define CHECK_GE(level, val1, val2) ALOG_CHECK_GE(_logger, alog::LOG_LEVEL_##level, val1, val2)
#define CHECK_GT(level, val1, val2) ALOG_CHECK_GT(_logger, alog::LOG_LEVEL_##level, val1, val2)
#define CHECK_NOTNULL(level, val) ALOG_CHECK_NOTNULL(_logger, alog::LOG_LEVEL_##level, val)

#define CHECK_STREQ(level, s1, s2) ALOG_CHECK_STREQ(_logger, alog::LOG_LEVEL_##level, s1, s2)
#define CHECK_STRNE(level, s1, s2) ALOG_CHECK_STRNE(_logger, alog::LOG_LEVEL_##level, s1, s2)
#define CHECK_STRCASEEQ(level, s1, s2) ALOG_CHECK_STRCASEEQ(_logger, alog::LOG_LEVEL_##level, s1, s2)
#define CHECK_STRCASENE(level, s1, s2) ALOG_CHECK_STRCASENE(_logger, alog::LOG_LEVEL_##level, s1, s2)

#define CHECK_INDEX(level, I, A) ALOG_CHECK_INDEX(_logger, alog::LOG_LEVEL_##level, I, A)
#define CHECK_BOUND(level, B, A) ALOG_CHECK_BOUND(_logger, alog::LOG_LEVEL_##level, B, A)
#define CHECK_DOUBLE_EQ(level, val1, val2) ALOG_CHECK_DOUBLE_EQ(_logger, alog::LOG_LEVEL_##level, val1, val2)
#define CHECK_NEAR(level, val1, val2, margin) ALOG_CHECK_NEAR(_logger, alog::LOG_LEVEL_##level, val1, val2, margin)

#define LOG_BINARY(level, msg)                                                                                         \
    {                                                                                                                  \
        if (__builtin_expect(_logger->isLevelEnabled(alog::LOG_LEVEL_##level), 0))                                     \
            _logger->logBinaryMessage(alog::LOG_LEVEL_##level, msg);                                                   \
    }

#define DECLARE_AND_SETUP_LOGGER(n, c) static alog::Logger *_logger = alog::Logger::getLogger(#n "." #c)
#define LOG_SHUTDOWN() alog::Logger::shutdown()
#define LOG_FLUSH() alog::Logger::flushAll()

#define CONDITION_LOG(condition, true_level, false_level, format, args...)                                             \
    do {                                                                                                               \
        if (condition) {                                                                                               \
            ALOG_LOG(_logger, alog::LOG_LEVEL_##true_level, format, ##args)                                            \
        } else {                                                                                                       \
            ALOG_LOG(_logger, alog::LOG_LEVEL_##false_level, format, ##args)                                           \
        }                                                                                                              \
    } while (0);

#define INTERVAL_LOG(logInterval, level, format, args...)                                                              \
    do {                                                                                                               \
        static int logCounter;                                                                                         \
        if (0 == logCounter) {                                                                                         \
            LOG(level, format, ##args);                                                                                \
            logCounter = logInterval;                                                                                  \
        }                                                                                                              \
        logCounter--;                                                                                                  \
    } while (0)

#define INTERVAL_LOG2(logInterval, level, format, args...)                                                             \
    do {                                                                                                               \
        static int64_t logTimestamp;                                                                                   \
        int64_t now = autil::TimeUtility::currentTimeInSeconds();                                                      \
        if (now - logTimestamp > logInterval) {                                                                        \
            LOG(level, format, ##args);                                                                                \
            logTimestamp = now;                                                                                        \
        }                                                                                                              \
    } while (0)

#define MASSIVE_LOG(level, format, args...)                                                                            \
    do {                                                                                                               \
        static int64_t logCounter = 0;                                                                                 \
        if ((logCounter >> 20) == 0 || (logCounter & 127) == 0) {                                                      \
            LOG(level, format, ##args);                                                                                \
        }                                                                                                              \
        logCounter++;                                                                                                  \
    } while (0)
