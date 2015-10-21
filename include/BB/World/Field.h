#ifndef FIELD_H
#define FIELD_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include "BB/Handler/LogHandler.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/World/Entity.h"

namespace bb {
    class Field: public sf::Drawable, public sf::Transformable{
    public:
        Field(ResourceHandler* resourceHandler, luabridge::lua_State* L, std::string world,
            sf::Vector2u windowSize, int id);
        void update();
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        std::string idString(int id);
        luabridge::lua_State* L;
        ResourceHandler* m_resourceHandler;
        sf::VertexArray m_vertices;
        std::string m_tileSet, m_objectTexture;
        int m_tiles[100];
        std::map<std::string, Entity*> m_objects;
        std::vector<Entity*> m_entities;
    };
}

#endif