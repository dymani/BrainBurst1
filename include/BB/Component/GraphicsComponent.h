#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

#include <SFML/Graphics.hpp>
#include "BB/Component/IComponent.h"
#include <iostream>
#include  <typeindex>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include "BB/Handler/LogHandler.h"
#include "BB/Handler/ResourceHandler.h"

namespace bb {
    class GraphicsComponent: public IComponent {
    public:
        static GraphicsComponent* create(GameStateGame& game, int entity, luabridge::lua_State* L, luabridge::LuaRef& luaGC);
        GraphicsComponent(GameStateGame& game, int entity);
        IComponent* copy(int entity);
        void draw(sf::RenderWindow& window, sf::Vector2f offset = sf::Vector2f());
        void addDrawable(std::string name, sf::Sprite* sprite, int z);
        void addDrawable(std::string name, sf::Text* text, int z);
        void getDrawable(std::string name, sf::Sprite*& sprite);
        void getDrawable(std::string name, sf::Text*& text);
        std::map<std::string, sf::Sprite*>& getSprites();
        std::map<std::string, sf::Text*>& getTexts();
        void setZ(float z);
        void setSize(sf::Vector2i size);
        float getZ();
        sf::Vector2i getSize();
    private:
        sf::Transform m_transform;
        std::map<std::string, sf::Sprite*> m_sprites;
        std::map<std::string, sf::Text*> m_texts;
        std::vector<std::pair<int, sf::Drawable*>> m_drawables;
        sf::Vector2i m_size;
        float m_z;
    };
}

#endif
