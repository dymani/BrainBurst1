#ifndef I_SYSTEM_H
#define I_SYSTEM_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <rapidjson/document.h>
#include "BB/Handler/LogHandler.h"

namespace bb {
    class ISystem {
    public:
        ISystem() {
        }
        virtual ~ISystem() {
        }
    protected:

    };
}

#endif