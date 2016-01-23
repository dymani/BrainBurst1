#include "BB/World/System/GraphicsSystem.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    GraphicsSystem::GraphicsSystem(GameStateGame& game) : ISystem(), m_game(game) {
        m_view = m_game.getWindowHandler()->getWindow().getDefaultView();
        m_tileSize = m_view.getSize().x / 16;
        m_textureSize = 32;
    }

    void GraphicsSystem::createList(std::map<std::type_index, std::unique_ptr<CList>>& lists) {
        lists[std::type_index(typeid(GraphicsComponent))] = std::unique_ptr<CList>(new CList());
    }

    void GraphicsSystem::createComponent(luabridge::LuaRef& luaE, std::map < std::type_index,
        std::unique_ptr < IComponent >> &list) {
        using namespace luabridge;
        LuaRef luaSize = luaE["size"];
        LuaRef luaComponents = luaE["components"];
        LuaRef luaGC = luaComponents["GraphicsComponent"];
        if(luaGC.isNil()) return;
        LuaRef luaType = luaGC["type"];
        LuaRef luaTexture = luaGC["texture"];
        auto* gc = new GraphicsComponent();
        gc->m_z = luaGC["z"].cast<float>();
        gc->m_type = luaType.cast<int>();
        if(gc->m_type == 0) {
            LuaRef luaTextureRect = luaGC["textureRect"];
            sf::IntRect rect = {luaTextureRect[1].cast<int>(), luaTextureRect[2].cast<int>(),
                luaTextureRect[3].cast<int>(), luaTextureRect[4].cast<int>()};
            sf::Vector2f size = {luaSize[1].cast<float>(), luaSize[2].cast<float>()};
            gc->m_hasTexture = false;
            if(luaTexture.isString()) {
                sf::Texture& tex = m_game.getResourceHandler()->getTexture(luaTexture.cast<std::string>());
                tex.setSmooth(false);
                gc->m_sprite.setTexture(tex);
                gc->m_hasTexture = true;
            }
            gc->m_sprite.setTextureRect(rect);
            gc->m_sprite.setOrigin({0, float(rect.height)});
            gc->m_sprite.setScale({float(size.x * m_tileSize) / float(rect.width),
                float(size.y * m_tileSize) / float(rect.height)});
        } else if(gc->m_type == 1) {
            sf::Vector2i size = {luaSize[1].cast<int>(), luaSize[2].cast<int>()};
            gc->m_hasTexture = false;
            if(luaTexture.isString()) {
                sf::Texture& tex = m_game.getResourceHandler()->getTexture(luaTexture.cast<std::string>());
                tex.setSmooth(false);
                gc->m_sprite.setTexture(tex);
                gc->m_hasTexture = true;
            }
            LuaRef luaDefaultAnimation = luaGC["defaultAnimation"];
            LuaRef luaAnimations = luaGC["animations"];
            for(int i = 1; i <= luaAnimations.length(); i++) {
                LuaRef luaAnimation = luaAnimations[i];
                LuaRef luaFrameStrip = luaAnimation["frameStrip"];
                Animation animation;
                animation.frameStrip = {luaFrameStrip[1].cast<int>(), luaFrameStrip[2].cast<int>(),
                    luaFrameStrip[3].cast<int>(), luaFrameStrip[4].cast<int>()};
                animation.frames = luaAnimation["frames"].cast<int>();
                animation.speed = luaAnimation["speed"].cast<float>();
                gc->m_animations[luaAnimation["name"].cast<std::string>()] = animation;
            }
            if(luaDefaultAnimation.isString())
                gc->m_currentAnimation = luaDefaultAnimation.cast<std::string>();
            else
                gc->m_currentAnimation = gc->m_animations.begin()->first;
            setAnimation(gc, "");
            Animation animation = gc->m_animations.begin()->second;
            gc->m_currentFrame = 0;
            gc->m_frameInterval = 0;
            gc->m_sprite.setOrigin({0, float(animation.frameStrip.height)});
            gc->m_sprite.setScale({float(size.x * m_tileSize) / float(animation.frameStrip.width /
                animation.frames), float(size.y * m_tileSize) / float(animation.frameStrip.height)});
        }
        list[std::type_index(typeid(GraphicsComponent))] = std::unique_ptr<GraphicsComponent>(gc);
    }

    void GraphicsSystem::createComponent(rapidjson::Value& jsonE,
        std::map<std::type_index, std::unique_ptr<IComponent>>& list, Entity* entity) {
        auto* component = list[std::type_index(typeid(GraphicsComponent))].get();
        if(!component) return;
        auto* gc = new GraphicsComponent(*dynamic_cast<GraphicsComponent*>(component));
        if(!gc->m_hasTexture) {
            gc->m_stageObjectTexture = m_game.getWorld().getStage(jsonE["stage"].GetString())
                ->getObjectTexture();
            sf::Texture& tex = m_game.getResourceHandler()->getTexture(gc->m_stageObjectTexture);
            tex.setSmooth(false);
            gc->m_sprite.setTexture(tex);
        }
        if(gc->m_type == 0) {

        } else if(gc->m_type == 1) {
            if(jsonE.HasMember("animation")) {
                if(jsonE.HasMember("frame"))
                    setAnimation(gc, jsonE["animation"].GetString(), jsonE["frame"].GetInt());
                else
                    setAnimation(gc, jsonE["animation"].GetString());
            }
        }
        entity->addComponent(std::type_index(typeid(GraphicsComponent)), gc);
    }

    void GraphicsSystem::update() {
        auto& cList = *m_game.getWorld().getField()->getComponentList<GraphicsComponent>();
        for(auto& c : cList.m_list) {
            auto& gc = *dynamic_cast<GraphicsComponent*>(c.second.get());
            if(gc.m_type == 1) {
                if(gc.m_animations[gc.m_currentAnimation].speed == -1) continue;
                gc.m_frameInterval++;
                if(gc.m_frameInterval >= gc.m_animations[gc.m_currentAnimation].speed) {
                    gc.m_frameInterval = 0;
                    setAnimation(&gc, "", gc.m_currentFrame + 1);
                }
            }
        }
    }

    void GraphicsSystem::draw(const double dt) {
        auto& cList = m_game.getWorld().getField()->getComponentList<GraphicsComponent>()->m_list;
        for(auto& c : cList) {
            auto& gc = *dynamic_cast<GraphicsComponent*>(c.second.get());
            gc.m_sprite.setPosition(mapCoordsToPixel(m_game.getWorld().getField()->getEntity(c.first)
                ->getCoord()));
            m_game.getWindowHandler()->getWindow().draw(gc.m_sprite);
        }
    }

    void GraphicsSystem::setViewCoord(float x, float y) {
        m_viewCoord = {x, y};
        if(x * m_tileSize - m_view.getSize().x / 2 < 0)
            m_viewCoord.x = m_view.getSize().x / 2 / m_tileSize;
        if(x * m_tileSize + m_view.getSize().x / 2 > m_game.getWorld().getField()->getSize() * m_tileSize)
            m_viewCoord.x = m_game.getWorld().getField()->getSize() - m_view.getSize().x / 2 / m_tileSize;
        if(y < -1)
            m_viewCoord.y = -1;
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

    void GraphicsSystem::setAnimation(GraphicsComponent* gc, std::string name, int frame) {
        gc->m_currentAnimation = name == "" ? gc->m_currentAnimation : name;
        auto& animation = gc->m_animations[gc->m_currentAnimation];
        gc->m_currentFrame = frame >= animation.frames ? 0 : frame;
        sf::IntRect rect = {animation.frameStrip.left + animation.frameStrip.width / animation.frames
            * gc->m_currentFrame, animation.frameStrip.top, animation.frameStrip.width / animation.frames,
            animation.frameStrip.height};
        gc->m_sprite.setTextureRect(rect);
    }
}