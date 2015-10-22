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
    class Entity;

    class GraphicsComponent: public IComponent, public sf::Drawable{
    public:
        static GraphicsComponent* create(Entity& entity, luabridge::lua_State* L, luabridge::LuaRef& luaGC);
        GraphicsComponent(Entity& entity);
        IComponent* copy();
        void addDrawable(std::string name, sf::Sprite* sprite, int z);
        void getDrawable(std::string name, sf::Sprite*& sprite);
        std::map<std::string, sf::Sprite*>& getSprites();
        void setZ(float z);
        void setSize(sf::Vector2i size);
        float getZ();
        sf::Vector2i getSize();
    private:
        Entity& m_entity;
        std::map<std::string, sf::Sprite*> m_sprites;
        std::vector<std::pair<int, sf::Drawable*>> m_drawables;
        sf::Vector2i m_size;
        float m_z;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };
}

#endif
