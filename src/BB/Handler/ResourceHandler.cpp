#include "BB/Handler/ResourceHandler.h"

namespace bb {
    ResourceHandler::ResourceHandler(Game& game): m_game(game) {

    }

    void ResourceHandler::load(luabridge::lua_State* L) {
        using namespace luabridge;
        if(luaL_loadfile(L, "assets/data/resource.lua") ||
            lua_pcall(L, 0, 0, 0)) {
            std::cerr << "Error while loading resource.lua.\n";
            return;
        }
        LuaRef luaTextures = getGlobal(L, "textures");
        if(!luaTextures.isTable()) {
            std::cerr << "Error while getting \"textures\" in resource.lua.\n";
            return;
        }
        std::string name, file;
        for(int i = 1; i <= luaTextures.length(); i++) {
            LuaRef luaTexture = luaTextures[i];
            if(!luaTexture.isTable()) {
                std::cerr << "Error while getting \"texture\" " << i << " in resource.lua.\n";
                return;
            }
            LuaRef luaName = luaTexture["name"];
            LuaRef luaFile = luaTexture["file"];
            if(!luaName.isString() || !luaFile.isString()) {
                std::cerr << "Error while getting \"texture\" " << i << " in resource.lua.\n";
                return;
            }
            name = luaName.cast<std::string>();
            file = luaFile.cast<std::string>();
            m_texturesLoading[name] = file;
        }
        return;
    }

    sf::Texture& ResourceHandler::getTexture(std::string name) {
        auto itLoading = m_texturesLoading.find(name);
        if(itLoading != m_texturesLoading.end()) {
            sf::Texture texture;
            if(texture.loadFromFile("assets/textures/" + m_texturesLoading[name])) {
                m_textures[name] = texture;
                m_texturesLoading.erase(itLoading);
            } else {
                std::cerr << "Error while loading texture " + name + ".\n";
            }
            return m_textures[name];
        } else {
            auto it = m_textures.find(name);
            if(it == m_textures.end()) {
                std::cerr << "Error while getting texture " + name + ".\n";
            }
            return m_textures[name];

        }
    }
}