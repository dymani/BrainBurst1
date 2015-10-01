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

        LuaRef luaFonts = getGlobal(L, "fonts");
        if(!luaFonts.isTable()) {
            std::cerr << "Error while getting \"fonts\" in resource.lua.\n";
            return;
        }
        for(int i = 1; i <= luaFonts.length(); i++) {
            LuaRef luaFont = luaFonts[i];
            if(!luaFont.isTable()) {
                std::cerr << "Error while getting \"font\" " << i << " in resource.lua.\n";
                return;
            }
            LuaRef luaName = luaFont["name"];
            LuaRef luaFile = luaFont["file"];
            if(!luaName.isString() || !luaFile.isString()) {
                std::cerr << "Error while getting \"font\" " << i << " in resource.lua.\n";
                return;
            }
            name = luaName.cast<std::string>();
            file = luaFile.cast<std::string>();
            m_fontsLoading[name] = file;
        }
    }

    sf::Texture& ResourceHandler::getTexture(std::string name) {
        auto textureIt = m_texturesLoading.find(name);
        if(textureIt != m_texturesLoading.end()) {
            sf::Texture texture;
            if(texture.loadFromFile("assets/textures/" + m_texturesLoading[name])) {
                m_textures[name] = texture;
            } else {
                std::cerr << "Error while loading texture " + name + ".\n";
            }
            m_texturesLoading.erase(textureIt);
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
            sf::SoundBuffer soundBuffer;
            if(soundBuffer.loadFromFile("assets/sounds/" + m_soundBuffersLoading[name])) {
                m_soundBuffers[name] = soundBuffer;
            } else {
                std::cerr << "Error while loading sound " + name + ".\n";
            }
            m_soundBuffersLoading.erase(soundBufferIt);
            return m_soundBuffers[name];
        } else {
            auto it = m_soundBuffers.find(name);
            if(it == m_soundBuffers.end()) {
                std::cerr << "Error while getting sound " + name + ".\n";
            }
            return m_soundBuffers[name];
        }
    }

    sf::Font& ResourceHandler::getFont(std::string name) {
        auto fontIt = m_fontsLoading.find(name);
        if(fontIt != m_fontsLoading.end()) {
            sf::Font font;
            if(font.loadFromFile("assets/fonts/" + m_fontsLoading[name])) {
                m_fonts[name] = font;
            } else {
                std::cerr << "Error while loading font " + name + ".\n";
            }
            m_fontsLoading.erase(fontIt);
            return m_fonts[name];
        } else {
            auto it = m_fonts.find(name);
            if(it == m_fonts.end()) {
                std::cerr << "Error while getting font " + name + ".\n";
            }
            return m_fonts[name];
        }
    }

    bool ResourceHandler::load() {
        if(!m_texturesLoading.empty()) {
            auto& it = m_texturesLoading.begin();
            sf::Texture texture;
            if(texture.loadFromFile("assets/textures/" + it->second)) {
                m_textures[it->first] = texture;
            } else {
                std::cerr << "Error while loading texture " + it->second + ".\n";
            }
            m_texturesLoading.erase(it);
            return false;
        }
        if(!m_soundBuffersLoading.empty()) {
            auto& it = m_soundBuffersLoading.begin();
            sf::SoundBuffer soundBuffer;
            if(soundBuffer.loadFromFile("assets/sounds/" + it->second)) {
                m_soundBuffers[it->first] = soundBuffer;
            } else {
                std::cerr << "Error while loading sound " + it->second + ".\n";
            }
            m_soundBuffersLoading.erase(it);
            return false;
        }
        if(!m_fontsLoading.empty()) {
            auto& it = m_fontsLoading.begin();
            sf::Font font;
            if(font.loadFromFile("assets/fonts/" + it->second)) {
                m_fonts[it->first] = font;
            } else {
                std::cerr << "Error while loading font " + it->second + ".\n";
            }
            m_fontsLoading.erase(it);
            return false;
        }
        return true;
    }

}