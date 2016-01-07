#ifndef I_SYSTEM_H
#define I_SYSTEM_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <rapidjson/document.h>
#include <map>
#include <typeindex>
#include "BB/Handler/LogHandler.h"

namespace bb {
    class IComponent;
    class Entity;

    class ISystem {
    public:
        ISystem() {
        }
        virtual ~ISystem() {
        }
        virtual void createList(std::map<std::type_index, std::map<int, IComponent*>*>& lists) = 0;
        virtual void createComponent(luabridge::LuaRef& luaE,
            std::map<std::type_index, IComponent*>& list) = 0;
        virtual void createComponent(rapidjson::Value& jsonE, std::map<std::type_index, IComponent*>& list,
            Entity* entity) = 0;
    protected:

    };
}

#endif