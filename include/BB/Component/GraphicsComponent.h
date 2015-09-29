#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

#include <SFML/Graphics.hpp>
#include "BB/Component/IComponent.h"
#include <iostream>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

namespace bb {
    class Drawable;
    class ResourceHandler;

    class GraphicsComponent: public IComponent {
    public:
        enum Type {
            SPRITE, TEXT
        };
        GraphicsComponent();
        bool createFromLua(luabridge::lua_State* L, luabridge::LuaRef& luaGraphicsComponent,
            ResourceHandler& m_resourceHandler);
        bool initFromLua(luabridge::lua_State* L, luabridge::LuaRef& luaEntity);
        IComponent* copy();
        GraphicsComponent* addDrawable(Type type, std::string name);
        GraphicsComponent* addTextureRect(std::string name, sf::IntRect rect);
        GraphicsComponent* setZ(float z);
        GraphicsComponent* setAlign(int align);
        sf::IntRect getTextureRect(std::string name);
        std::vector<std::pair<std::string, Drawable*>>& getDrawables();
        template <typename T>
        T* getDrawable(std::string name) {
            for(auto& it : m_drawables) {
                if(it.first == name
                    &&dynamic_cast<T*>(it.second->m_drawable)) {
                    return dynamic_cast<T*>(it.second->m_drawable);
                }
            }
            return nullptr;
        }
        float getZ();
        int getAlign();
    private:
        std::vector<std::pair<std::string, Drawable*>> m_drawables;
        std::map<std::string, sf::IntRect> m_textureRects;
        float m_z;
        int m_align;
    };

    class Drawable {
    public:
        enum Type {
            SPRITE, TEXT
        };
        Drawable(Type type, sf::Drawable* drawable);
        Drawable* copy();
        sf::Drawable* m_drawable;
        Type m_type;
    };
}

#endif