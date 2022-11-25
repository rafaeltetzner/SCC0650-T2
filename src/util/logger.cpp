#include "logger.h"

#include <stdarg.h>
#include <stdio.h>


#define MAX_LOG_LEN 1024*1024 / 2
static char message_buffer[MAX_LOG_LEN] = {0};

void logger::print(logger::level level, const char* message, ...)
{
    static const char* levels_format[] =
    {
        "\033[38;5;0m\033[48;5;196m[FATAL]: %s\033[0m\n",
                    "\033[38;5;196m[ERROR]: %s\033[0m\n",
                    "\033[38;5;214m[WARN] : %s\033[0m\n",
                     "\033[38;5;34m[INFO] : %s\033[0m\n",
                     "\033[38;5;21m[DEBUG]: %s\033[0m\n",
                      "\033[38;5;0m[TRACE]: %s\033[0m\n"
    };

    // build message with given va_args
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(message_buffer, MAX_LOG_LEN, message, arg_ptr);
    va_end(arg_ptr);

    // logs message
    printf(levels_format[level], message_buffer);
}
