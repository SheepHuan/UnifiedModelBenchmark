#pragma once
#include "cstdio"
#include <sstream>

namespace mfmt{


    template<typename T>
    inline void format_impl(std::stringstream& ss, const T& x) {
        ss << x;

    };

    template<typename T, typename ... TArgs>
    inline void format_impl(std::stringstream& ss, const T& x, const TArgs& ... others) {
        format_impl(ss, x);
        format_impl(ss, others...);

    };

    template<typename ... TArgs>
    inline std::string format(const TArgs& ... args) {
        std::stringstream ss ;
        format_impl(ss, args...);
        return ss.str();
    }

    template<typename ... TArgs>
    inline std::string format_string(char *fmt,const TArgs& ... args) {
        constexpr size_t oldlen = BUFSIZ;
        char buffer[oldlen];  // 默认栈上的缓冲区

        size_t newlen = snprintf(&buffer[0], oldlen, fmt, args...);
        newlen++;  // 算上终止符'\0'

        if (newlen > oldlen) {  // 默认缓冲区不够大，从堆上分配
            std::vector<char> newbuffer(newlen);
            snprintf(newbuffer.data(), newlen, fmt, args...);
            return std::string(newbuffer.data());
        }

        return buffer;
    }


} 

namespace mlog
{
    enum class LogLevel
    {
        LOG_LEVEL_DEBUG,
        LOG_LEVEL_INFO,
        LOG_LEVEL_WARNING,
        LOG_LEVEL_ERROR,
    };
    namespace detail
    {
        inline void log_callback(LogLevel level, const std::string &msg)
        {
            switch (level)
            {
            case LogLevel::LOG_LEVEL_DEBUG:
                printf("[\x1b[90mDEBUG\x1B[0m] %s\n", msg.c_str());
                break;
            case LogLevel::LOG_LEVEL_INFO:
                printf("[\x1B[32mINFO\x1B[0m] %s\n", msg.c_str());
                break;
            case LogLevel::LOG_LEVEL_WARNING:
                printf("[\x1B[33mWARN\x1B[0m] %s\n", msg.c_str());
                break;
            case LogLevel::LOG_LEVEL_ERROR:
                printf("[\x1B[31mERROR\x1B[0m] %s\n", msg.c_str());
                break;
            }
            // 刷新stdout缓冲
            std::fflush(stdout);
        }
    }
    template <typename... TArgs>
    void log(LogLevel level, const TArgs &...msg)
    {
        // TODO:未来考虑LOG LEVEL的过滤
        detail::log_callback(level, mfmt::format("  ", msg...));
    }
    template <typename... TArgs>
    inline void debug(const TArgs &...msg)
    {
        log(LogLevel::LOG_LEVEL_DEBUG, msg...);
    }

    template <typename... TArgs>
    inline void info(const TArgs &...msg)
    {
        log(LogLevel::LOG_LEVEL_INFO, msg...);
    }

    template <typename... TArgs>
    inline void warn(const TArgs &...msg)
    {
        log(LogLevel::LOG_LEVEL_WARNING, msg...);
    }

    template <typename... TArgs>
    inline void error(const TArgs &...msg)
    {
        log(LogLevel::LOG_LEVEL_ERROR, msg...);
    }
}


