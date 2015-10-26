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
#include "BB/Handler/GraphicsHandler.h"
#include "BB/Handler/MovementHandler.h"
#include "BB/World/Entity.h"

namespace bb {
    class Field{
    public:
        Field(ResourceHandler* resourceHandler, GraphicsHandler* graphicsHandler, luabridge::lua_State* L,
            std::string world, int height, int id);
        void update();
        void draw(sf::RenderWindow& window);
        std::vector<Entity*>& getEntities();
    private:
        std::string idString(int id);
        luabridge::lua_State* L;
        ResourceHandler* m_resourceHandler;
        GraphicsHandler* m_graphicsHandler;
        MovementHandler* m_movementHandler;
        sf::RenderStates m_states;
        sf::VertexArray m_vertices;
        std::string m_tileSet, m_objectTexture;
        int m_tiles[100];
        std::map<std::string, Entity*> m_objects;
        std::vector<Entity*> m_entities;
    };
}

#endif