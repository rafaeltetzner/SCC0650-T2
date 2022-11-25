#pragma once

/** @brief Indicates the which logging levels are active. 5 = [TRACE, DEBUG, ..., FATAL] */
#ifndef LOG_DEPTH
#   define LOG_DEPTH 5
#endif

namespace logger
{

    /** @brief represents level of logging */
    enum level
    {
        /** @brief Fatal log level, should be used when an error crashs the application */
        FATAL = 0,
        /** @brief Error log level, should be used to indicate a critical runtime error that may issue undefined behaviour */
        ERROR = 1,
        /** @brief Warning log level, should be used to indicate no critical runtime errors*/
        WARN  = 2,
        /** @brief Info log level, should be used for non-errounous informational purposes */
        INFO  = 3,
        /** @brief Debug log level, should be used for debugging purposes */
        DEBUG = 4,
        /** @brief Trace log level, should be used for verbose debugging purposes */
        TRACE = 5
    };

    /**
     * @brief Outputs message into standart output at given level
     * 
     * @param level The log level to use
     * @param message The message format
     * @param args The format arguments (like printf)
     * 
     * @return Will return false if level is unknow.
     */
    void print(level level, const char* message, ...);

    namespace log
    {

        #if LOG_DEPTH >= 5
            /**
             * @brief Logs a trace-level message into standard output
             * 
             */
            template<typename... Args> void trace(const char* message, Args... args) { print(level::TRACE, message, args...); };
        #else
            constexpr void trace(const char* message, ...) {  };
        #endif

        #if LOG_DEPTH >= 4
            /**
             * @brief Logs a debug-level message into standard output
             * 
             */
            template<typename... Args> void debug(const char* message, Args... args) { print(level::DEBUG, message, args...); };
        #else
            constexpr void debug(const char* message, ...) {  };
        #endif

        #if LOG_DEPTH >= 3
            /**
             * @brief Logs a info-level message into standard output
             * 
             */
            template<typename... Args> void info(const char* message, Args... args) { print(level::INFO, message, args...); };
        #else
            constexpr void info(const char* message, ...) {  };
        #endif

        #if LOG_DEPTH >= 2
            /**
             * @brief Logs a warn-level message into standard output
             * 
             */
            template<typename... Args> void warn(const char* message, Args... args) { print(level::WARN, message, args...); };
        #else
            constexpr void warn(const char* message, ...) {  };
        #endif

        #if LOG_DEPTH >= 1
            /**
             * @brief Logs a error-level message into standard output
             * 
             */
            template<typename... Args> void error(const char* message, Args... args) { print(level::ERROR, message, args...); };
        #else
            constexpr void error(const char* message, ...) {  };
        #endif

        #if LOG_DEPTH >= 0
            /**
             * @brief Logs a error-level message into standard output
             * 
             */
            template<typename... Args> void fatal(const char* message, Args... args) { print(level::FATAL, message, args...); };
        #else
            constexpr void fatal(const char* message, ...) {  };
        #endif

    }
}