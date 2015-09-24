#ifndef RESOURCE_HANDLER_H
#define RESOURCE_HANDLER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <map>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

namespace bb {
    class Game;

    class ResourceHandler {
    public:
        ResourceHandler(Game& game);
        void load(luabridge::lua_State* L);
        sf::Texture& getTexture(std::string name);
    private:
        Game& m_game;
        std::map<std::string, std::string> m_texturesLoading;
        std::map<std::string, sf::Texture> m_textures;
    };
}

#endif