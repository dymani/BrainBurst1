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
        GraphicsComponent(Entity& entity);
        IComponent* copy();
        void addDrawable(std::type_index type, sf::Drawable* drawable, std::string name);
        void setZ(float z);
        void setSize(sf::Vector2i size);
        std::map<std::type_index, std::pair<std::string, sf::Drawable*>>& getDrawables();
        template <typename T>
        T* getDrawable(std::string name) {
            auto& it = m_drawables.find(typeid(T));
            if(it != m_drawables.end()) {
                if(it->second.first == name) {
                    if(dynamic_cast<T*>(it->second.second)) {
                        return dynamic_cast<T*>(it->second.second);
                    }
                }
            }
            LogHandler::log(LogHandler::ERR, std::string(typeid(T).name()) + " " + name
                + "not found in graphics component", typeid(this).name());
            return nullptr;
        }
        float getZ();
        sf::Vector2i getSize();
    private:
        Entity& m_entity;
        std::map<std::type_index, std::pair<std::string, sf::Drawable*>> m_drawables;
        sf::Vector2i m_size;
        float m_z;
    };
}

#endif