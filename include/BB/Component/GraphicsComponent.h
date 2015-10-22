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

    class GraphicsComponent: public IComponent {
    public:
        static GraphicsComponent* create(Entity& entity, luabridge::lua_state* L, luabridge::LuaRef& luaGC);
        GraphicsComponent(Entity& entity);
        void addSprite(std::string name, sf::Sprite* sprite);
        template<typename T>
        T* getDrawable(std::string name);
        IComponent* copy();
        void setZ(float z);
        float getZ();
    private:
        Entity& m_entity;
        std::map<std::string, sf::Sprite*> m_sprites;
        std::vector<sf::Drawable*> m_buffer;
        float m_z;
    };
}

#endif
