#include "BB/Handler/LogHandler.h"

namespace bb {
    void LogHandler::log(Type type, std::string message, std::string sender) {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        localtime_s(&tstruct, &now);
        strftime(buf, sizeof(buf), "%X", &tstruct);
        std::cerr << buf << " ";
        switch(type) {
            case ERR:
                std::cerr << "[ERROR]   ";
                break;
            case WRN:
                std::cerr << "[WARNING] ";
                break;
            case INF:
                std::cerr << "[INFO]    ";
                break;
        }
        std::cerr << message.c_str() << ". ";
        std::cerr << "(" << sender.c_str() << ")" << std::endl;
    }
}