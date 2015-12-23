#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include "BB/Handler/LogHandler.h"
#include <rapidjson/document.h>
#include "BB/World/System/ISystem.h"
#include "BB/World/Component/GraphicsComponent.h"

namespace bb {
    class GameStateGame;

    class GraphicsSystem : public ISystem {
    public:
        GraphicsSystem(GameStateGame& game);
        IComponent* createComponent(luabridge::LuaRef& luaE);
        IComponent* createComponent(IComponent* component, rapidjson::Value& jsonE);
        void draw(const double dt);
        void setViewCoord(float x, float y);
        sf::Vector2f getViewCoord();
        sf::Vector2f mapViewPixel(sf::Vector2f pixel, bool addView);
        sf::Vector2f mapPixelToCoords(sf::Vector2f pixel);
        sf::Vector2f mapCoordsToPixel(sf::Vector2f coord);
        int getTextureSize();
        float getTileSize();
        sf::View& getView();
    private:
        GameStateGame& m_game;
        sf::View m_view;
        sf::Vector2f m_viewCoord;
        int m_textureSize;
        float m_tileSize;
        std::map<std::string, GraphicsComponent> m_graphicsComponent;
    };
}

#endif