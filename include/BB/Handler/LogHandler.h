#ifndef LOG_HANDLER_H
#define LOG_HANDLER_H

#include <iostream>
#include <time.h>

namespace bb {
    class LogHandler {
    public:
        enum Type {
            ERR, WRN, INF
        };
        static void log(Type type, std::string message, std::string sender);
    private:

    };
}

#endif