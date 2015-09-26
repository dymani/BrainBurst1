#ifndef SCRIPT_HANDLER_H
#define SCRIPT_HANDLER_H

#include <vector>
#include <iostream>
#include <map>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

namespace bb {
    class ResourceHandler;
    class Entity;

    class ScriptHandler {
    public:
        ScriptHandler(ResourceHandler& resourceHandler);
        void loadEntities(std::vector<Entity*>& entities, luabridge::lua_State* L, std::string file);
    private:
        ResourceHandler& m_resourceHandler;
        std::map<std::string, Entity*> m_entityList;
    };
}

#endif