#include "BB/Handler/ResourceHandler.h"

namespace bb {
    ResourceHandler::ResourceHandler(Game& game): m_game(game) {

    }

    void ResourceHandler::load(luabridge::lua_State* L) {
        using namespace luabridge;
        if(luaL_loadfile(L, "assets/data/resource.lua")) {
            LogHandler::log<ResourceHandler>(ERR, "File resource.lua not found");
            assert(false);
            return;
        }
        if(lua_pcall(L, 0, 0, 0)) {
            LogHandler::log<ResourceHandler>(ERR, "File resource.lua not loaded");
            assert(false);
            return;
        }
        LuaRef luaTextures = getGlobal(L, "textures");
        if(!luaTextures.isTable()) {
            LogHandler::log<ResourceHandler>(ERR, "Incorrect data format for textures");
            assert(false);
            return;
        }
        std::string name, file;
        for(int i = 1; i <= luaTextures.length(); i++) {
            LuaRef luaTexture = luaTextures[i];
            if(!luaTexture.isTable()) {
                LogHandler::log<ResourceHandler>(ERR, "Incorrect data format for texture[" + std::to_string(i)
                    + "]");
                assert(false);
                return;
            }
            LuaRef luaName = luaTexture["name"];
            LuaRef luaFile = luaTexture["file"];
            if(!luaName.isString() || !luaFile.isString()) {
                LogHandler::log<ResourceHandler>(ERR, "Incorrect data format for texture[" + std::to_string(i)
                    + "]");
                continue;
            }
            name = luaName.cast<std::string>();
            file = luaFile.cast<std::string>();
            m_texturesLoading[name] = file;
        }

        LuaRef luaSounds = getGlobal(L, "sounds");
        if(!luaSounds.isTable()) {
            LogHandler::log<ResourceHandler>(ERR, "Incorrect data format for sounds");
            assert(false);
            return;
        }
        for(int i = 1; i <= luaSounds.length(); i++) {
            LuaRef luaSound = luaSounds[i];
            if(!luaSound.isTable()) {
                LogHandler::log<ResourceHandler>(ERR, "Incorrect data format for sound[" + std::to_string(i)
                    + "]");
                continue;
            }
            LuaRef luaName = luaSound["name"];
            LuaRef luaFile = luaSound["file"];
            if(!luaName.isString() || !luaFile.isString()) {
                LogHandler::log<ResourceHandler>(ERR, "Incorrect data format for sound[" + std::to_string(i)
                    + "]");
                continue;
            }
            name = luaName.cast<std::string>();
            file = luaFile.cast<std::string>();
            m_soundBuffersLoading[name] = file;
        }

        LuaRef luaFonts = getGlobal(L, "fonts");
        if(!luaFonts.isTable()) {
            LogHandler::log<ResourceHandler>(ERR, "Incorrect data format for fonts");
            assert(false);
            return;
        }
        for(int i = 1; i <= luaFonts.length(); i++) {
            LuaRef luaFont = luaFonts[i];
            if(!luaFont.isTable()) {
                LogHandler::log<ResourceHandler>(ERR, "Incorrect data format for font[" + std::to_string(i)
                    + "]");
                continue;
            }
            LuaRef luaName = luaFont["name"];
            LuaRef luaFile = luaFont["file"];
            if(!luaName.isString() || !luaFile.isString()) {
                LogHandler::log<ResourceHandler>(ERR, "Incorrect data format for font[" + std::to_string(i)
                    + "]");
                continue;
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
                m_textures[name].setSmooth(false);
                m_textures[name].setRepeated(true);
            } else {
                LogHandler::log<ResourceHandler>(ERR, "Texture \"" + m_texturesLoading[name] + "\" not found");
            }
            m_texturesLoading.erase(textureIt);
        } else {
            auto it = m_textures.find(name);
            if(it == m_textures.end()) {
                LogHandler::log<ResourceHandler>(ERR, "Texture \"" + name + "\" not found");
                return m_textures["NULL"];
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
                LogHandler::log<ResourceHandler>(ERR, "Sound \"" + m_soundBuffersLoading[name] + "\" not found");
            }
            m_soundBuffersLoading.erase(soundBufferIt);
            return m_soundBuffers[name];
        } else {
            auto it = m_soundBuffers.find(name);
            if(it == m_soundBuffers.end()) {
                LogHandler::log<ResourceHandler>(ERR, "Sound \"" + name + "\" not found");
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
                LogHandler::log<ResourceHandler>(ERR, "Font \"" + m_fontsLoading[name] + "\" not found");
            }
            m_fontsLoading.erase(fontIt);
            return m_fonts[name];
        } else {
            auto it = m_fonts.find(name);
            if(it == m_fonts.end()) {
                LogHandler::log<ResourceHandler>(ERR, "Font\"" + name + "\" not found");
            }
            return m_fonts[name];
        }
    }

    bool ResourceHandler::load() {
        if(!m_texturesLoading.empty()) {
            auto& it = m_texturesLoading.begin();
            sf::Texture texture;
            if(texture.loadFromFile("assets/textures/" + it->second)) {
                texture.setSmooth(true);
                m_textures[it->first] = texture;
            } else {
                LogHandler::log<ResourceHandler>(ERR, "Texture \"" + it->second + "\" not found");
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
                LogHandler::log<ResourceHandler>(ERR, "Sound \"" + it->second + "\" not found");
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
                LogHandler::log<ResourceHandler>(ERR, "Font \"" + it->second + "\" not found");
            }
            m_fontsLoading.erase(it);
            return false;
        }
        return true;
    }

}