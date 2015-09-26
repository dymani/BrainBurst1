#include "BB/Handler/ResourceHandler.h"

namespace bb {
    ResourceHandler::ResourceHandler(Game& game): m_game(game) {

    }

    void ResourceHandler::load(luabridge::lua_State* L) {
        using namespace luabridge;
        if(luaL_loadfile(L, "assets/data/resource.lua") || lua_pcall(L, 0, 0, 0)) {
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
        LuaRef luaSounds = getGlobal(L, "sounds");
        if(!luaSounds.isTable()) {
            std::cerr << "Error while getting \"sounds\" in resource.lua.\n";
            return;
        }
        for(int i = 1; i <= luaSounds.length(); i++) {
            LuaRef luaSound = luaSounds[i];
            if(!luaSound.isTable()) {
                std::cerr << "Error while getting \"sound\" " << i << " in resource.lua.\n";
                return;
            }
            LuaRef luaName = luaSound["name"];
            LuaRef luaFile = luaSound["file"];
            if(!luaName.isString() || !luaFile.isString()) {
                std::cerr << "Error while getting \"sound\" " << i << " in resource.lua.\n";
                return;
            }
            name = luaName.cast<std::string>();
            file = luaFile.cast<std::string>();
            m_soundBuffersLoading[name] = file;
        }
    }

    sf::Texture& ResourceHandler::getTexture(std::string name) {
        auto textureIt = m_texturesLoading.find(name);
        if(textureIt != m_texturesLoading.end()) {
            sf::Texture texture;
            if(texture.loadFromFile("assets/textures/" + m_texturesLoading[name])) {
                m_textures[name] = texture;
                m_texturesLoading.erase(textureIt);
            } else {
                std::cerr << "Error while loading texture " + name + ".\n";
            }
        } else {
            auto it = m_textures.find(name);
            if(it == m_textures.end()) {
                std::cerr << "Error while getting texture " + name + ".\n";
            }
        }
        return m_textures[name];
    }

    sf::SoundBuffer& ResourceHandler::getSoundBuffer(std::string name) {
        auto soundBufferIt = m_soundBuffersLoading.find(name);
        if(soundBufferIt != m_soundBuffersLoading.end()) {
            sf::SoundBuffer soundBffer;
            if(soundBffer.loadFromFile("assets/sounds/" + m_soundBuffersLoading[name])) {
                m_soundBuffers[name] = soundBffer;
                m_soundBuffersLoading.erase(soundBufferIt);
            } else {
                std::cerr << "Error while loading sound " + name + ".\n";
            }
            return m_soundBuffers[name];
        } else {
            auto it = m_soundBuffers.find(name);
            if(it == m_soundBuffers.end()) {
                std::cerr << "Error while getting sound " + name + ".\n";
            }
            return m_soundBuffers[name];
        }
    }
}