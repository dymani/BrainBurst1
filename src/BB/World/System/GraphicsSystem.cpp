#include "BB/World/System/GraphicsSystem.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    GraphicsSystem::GraphicsSystem(GameStateGame& game) : ISystem(), m_game(game) {
        m_view = m_game.getWindowHandler()->getWindow().getDefaultView();
        m_tileSize = m_view.getSize().x / 16;
        m_textureSize = 32;
    }

    void GraphicsSystem::createList(std::map<std::type_index, std::map<int, IComponent*>*>& lists) {
        lists[std::type_index(typeid(GraphicsComponent))] = new std::map<int, IComponent*>;
    }

    void GraphicsSystem::createComponent(luabridge::LuaRef& luaE, std::map<std::type_index,
        IComponent*>& list) {
        using namespace luabridge;
        LuaRef luaSize = luaE["size"];
        LuaRef luaComponents = luaE["components"];
        LuaRef luaGC = luaComponents["GraphicsComponent"];
        if(luaGC.isNil()) return;
        auto* gc = new GraphicsComponent();
        LuaRef luaTexture = luaGC["texture"];
        LuaRef luaTextureRect = luaGC["textureRect"];
        gc->m_z = luaGC["z"].cast<float>();
        sf::IntRect rect = {luaTextureRect[1].cast<int>(), luaTextureRect[2].cast<int>(),
            luaTextureRect[3].cast<int>(), luaTextureRect[4].cast<int>()};
        sf::Vector2i size = {luaSize[1].cast<int>(), luaSize[2].cast<int>()};
        gc->hasTexture = false;
        if(luaTexture.isString()) {
            sf::Texture& tex = m_game.getResourceHandler()->getTexture(luaTexture.cast<std::string>());
            tex.setSmooth(false);
            gc->m_sprite.setTexture(tex);
            gc->hasTexture = true;
        }
        gc->m_sprite.setTextureRect(rect);
        gc->m_sprite.setOrigin({0, float(rect.height)});
        gc->m_sprite.setScale({float(size.x * m_tileSize) / float(rect.width),
            float(size.y * m_tileSize) / float(rect.height)});
        list[std::type_index(typeid(GraphicsComponent))] = gc;
    }

    void GraphicsSystem::createComponent(rapidjson::Value& jsonE, std::map<std::type_index, IComponent*>& list,
        Entity* entity) {
        auto* component = list[std::type_index(typeid(GraphicsComponent))];
        if(!component) return;
        auto* gc = new GraphicsComponent(*dynamic_cast<GraphicsComponent*>(component));
        if(!gc->hasTexture) {
            gc->m_stageObjectTexture = m_game.getWorld().getStage(jsonE["stage"].GetString())
                ->getObjectTexture();
            sf::Texture& tex = m_game.getResourceHandler()->getTexture(gc->m_stageObjectTexture);
            tex.setSmooth(false);
            gc->m_sprite.setTexture(tex);
        }
        entity->addComponent(m_game, std::type_index(typeid(GraphicsComponent)), gc);
    }

    void GraphicsSystem::draw(const double dt) {
        auto& cList = *m_game.getWorld().getField()->getComponentList<GraphicsComponent>();
        for(auto& c : cList) {
            auto& gc = *dynamic_cast<GraphicsComponent*>(c.second);
            gc.m_sprite.setPosition(mapCoordsToPixel(m_game.getWorld().getField()->getEntity(c.first)
                ->getCoord()));
            m_game.getWindowHandler()->getWindow().draw(gc.m_sprite);
        }
    }

    void GraphicsSystem::setViewCoord(float x, float y) {
        m_viewCoord = {x, y};
        /*if(x * m_tileSize - m_view.getSize().x / 2 < 0)
            m_viewCoord.x = m_view.getSize().x / 2 / m_tileSize;
        if(x * m_tileSize + m_view.getSize().x / 2 > m_game.getWorld().getField()->getSize() * m_tileSize)
            m_viewCoord.x = m_game.getWorld().getField()->getSize() - m_view.getSize().x / 2 / m_tileSize;
        if(y < -1)
            m_viewCoord.y = -1;*/
        m_view.setCenter(m_viewCoord.x * m_tileSize, m_view.getSize().y / 2 - m_viewCoord.y * m_tileSize);
    }

    sf::Vector2f GraphicsSystem::getViewCoord() {
        return m_viewCoord;
    }

    sf::Vector2f GraphicsSystem::mapViewPixel(sf::Vector2f pixel, bool addView) {
        sf::RenderWindow& window = m_game.getWindowHandler()->getWindow();
        if(addView)
            return sf::Vector2f(window.mapCoordsToPixel(pixel, m_view));
        else
            return window.mapPixelToCoords(sf::Vector2i(pixel), m_view);
    }

    sf::Vector2f GraphicsSystem::mapPixelToCoords(sf::Vector2f pixel) {
        sf::RenderWindow& window = m_game.getWindowHandler()->getWindow();
        //sf::Vector2f pixelV = window.mapPixelToCoords(sf::Vector2i(pixel), m_view);
        sf::Vector2f coord;
        coord.x = pixel.x / m_tileSize;
        coord.y = (window.getSize().y - pixel.y) / m_tileSize;
        return coord;
    }

    sf::Vector2f GraphicsSystem::mapCoordsToPixel(sf::Vector2f coord) {
        sf::RenderWindow& window = m_game.getWindowHandler()->getWindow();
        float x = coord.x * m_tileSize;
        float y = window.getSize().y - coord.y * m_tileSize;
        //sf::Vector2f pixel = sf::Vector2f(window.mapCoordsToPixel({x, y}, m_view));
        return{x, y};
    }

    int GraphicsSystem::getTextureSize() {
        return m_textureSize;
    }

    float GraphicsSystem::getTileSize() {
        return m_tileSize;
    }
    sf::View& GraphicsSystem::getView() {
        return m_view;
    }
}