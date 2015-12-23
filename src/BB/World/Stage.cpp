#include "BB/World/Stage.h"

namespace bb {
    Stage* Stage::create(luabridge::lua_State* L, std::string name) {
        Stage* stage = new Stage();
        stage->m_name = name;
        std::string file = "assets/data/world/stages/" + stage->m_name + ".lua";
        using namespace luabridge;
        if(luaL_loadfile(L, file.c_str()) || lua_pcall(L, 0, 0, 0)) {
            LogHandler::log<Stage>(ERR, "Stage \"" + file + "\" not found");
            return nullptr;
        }
        LuaRef luaBackgroundTexture = getGlobal(L, "backgroundTexture");
        stage->m_backgroundTexture = luaBackgroundTexture.cast<std::string>();
        LuaRef luaObjectTexture = getGlobal(L, "objectTexture");
        stage->m_objectTexture = luaObjectTexture.cast<std::string>();
        LuaRef luatileSet = getGlobal(L, "tileSet");
        stage->m_tileSet = luatileSet.cast<std::string>();
        LuaRef luaDisplayNames = getGlobal(L, "displayNames");
        std::string nameID, displayName;
        for(int i = 1; i <= luaDisplayNames.length(); i++) {
            LuaRef luaDisplayName = luaDisplayNames[i];
            nameID = luaDisplayName["name"].cast<std::string>();
            displayName = luaDisplayName["displayName"].cast<std::string>();
            stage->m_displayNames[nameID] = displayName;
        }
        return stage;
    }

    Stage::Stage() {
    }

    std::string Stage::getBackgroundTexture() {
        return m_backgroundTexture;
    }

    std::string Stage::getObjectTexture() {
        return m_objectTexture;
    }

    std::string Stage::getTileSet() {
        return m_tileSet;
    }

    std::string Stage::getDisplayName(std::string name) {
        if(m_displayNames.find(name) != m_displayNames.end())
            return m_displayNames[name];
        return "NULL";
    }
}