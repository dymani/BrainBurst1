#ifndef STAGE_H
#define STAGE_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <map>
#include "BB/Handler/LogHandler.h"

namespace bb {
    class Stage {
    public:
        static Stage* create(luabridge::lua_State* L, std::string name);
        Stage();
        std::string getBackgroundTexture();
        std::string getObjectTexture();
        std::string getTileSet();
        std::string getDisplayName(std::string name);
    private:
        std::string m_name;
        std::string m_backgroundTexture, m_objectTexture, m_tileSet;
        std::map<std::string, std::string> m_displayNames;
    };
}

#endif