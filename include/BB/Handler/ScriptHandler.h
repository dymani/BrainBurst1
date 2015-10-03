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
#include "BB/Handler/LogHandler.h"

namespace bb {
    class ResourceHandler;
    class Entity;

    class ScriptHandler {
    public:
        ScriptHandler(ResourceHandler& resourceHandler);
        void loadEntities(std::vector<Entity*>& entityList, luabridge::lua_State* L, std::string file);
    private:
        ResourceHandler& m_resourceHandler;
        std::map<std::string, Entity*> m_entityList;
        bool loadTemplates(luabridge::lua_State* L, luabridge::LuaRef& luaTemplates);
        bool loadComponents(Entity* entity, luabridge::lua_State* L, luabridge::LuaRef& luaComponents);
        bool loadEntities(std::vector<Entity*>& entityList, luabridge::lua_State* L,
            luabridge::LuaRef& luaEntities);
        Entity* getEntity(luabridge::lua_State* L, luabridge::LuaRef& luaEntity);
    };
}

#endif