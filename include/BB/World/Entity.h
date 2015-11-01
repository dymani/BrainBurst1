#ifndef ENTITY_H
#define ENTITY_H

#include <typeindex>
#include <map>
#include <SFML/Graphics.hpp>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include "BB/Handler/LogHandler.h"

namespace bb {
    class IComponent;
    class GameStateGame;

    class Entity {
    public:
        static Entity* create(GameStateGame& game, int entity, luabridge::lua_State* L, luabridge::LuaRef& luaEntity);
        Entity(int id);
        Entity(Entity& entity, int id);
        ~Entity();
        int getId();
        void addComponent(std::type_index type, IComponent* component);
        template <typename T>
        T* getComponent() {
            auto it = m_components.find(std::type_index(typeid(T)));
            if(it != m_components.end()) {
                return dynamic_cast<T*>(it->second);
            }
            return nullptr;
        }
        template <typename T>
        void removeComponent() {
            auto it = m_components.find(std::type_index(typeid(T)));
            if(it != m_components.end()) {
                delete it->second;
                m_components.erase(it);
            }
        }
        void setCoord(sf::Vector2f coord);
        sf::Vector2f getCoord();
    private:
        int const m_id;
        std::map<std::type_index, IComponent*> m_components;
        sf::Vector2f m_coord;
    };
}

#endif