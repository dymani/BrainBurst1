#include "BB/Component/GraphicsComponent.h"
#include "BB/World/Entity.h"

namespace bb {
    GraphicsComponent* GraphicsComponent::create(Entity& entity, luabridge::lua_State* L,
        luabridge::LuaRef& luaGC) {
        GraphicsComponent* gc = new GraphicsComponent(entity);
        using namespace luabridge;
        LuaRef luaSize = luaGC["size"];
        LuaRef luaZ = luaGC["z"];
        LuaRef luaDrawables = luaGC["drawables"];
        gc->setSize({luaSize[1].cast<int>(), luaSize[2].cast<int>()});
        gc->setZ(luaZ.cast<int>());
        for(int i = 1; i <= luaDrawables.length(); i++) {
            LuaRef luaDrawable = luaDrawables[i];
            LuaRef luaType = luaDrawable["type"];
            LuaRef luaName = luaDrawable["name"];
            if(luaType.cast<int>() == 0) {
                LuaRef luaTextureRect = luaDrawable["textureRect"];
                sf::IntRect rect = {luaTextureRect[1].cast<int>(), luaTextureRect[2].cast<int>(),
                    luaTextureRect[3].cast<int>(), luaTextureRect[4].cast<int>()};
                sf::Sprite* sprite = new sf::Sprite();
                sf::Vector2i size = gc->getSize();
                sprite->setTextureRect(rect);
                sprite->setScale({float(size.x) / float(rect.width), float(size.y) / float(rect.height)});
                gc->addDrawable(luaName.cast<std::string>(), sprite, i);
            }
        }
        return gc;
    }

    GraphicsComponent::GraphicsComponent(Entity& entity): m_entity(entity) {
    }

    IComponent* GraphicsComponent::copy() {
        GraphicsComponent* gc = new GraphicsComponent(m_entity);
        for(auto& it : m_sprites) {
            sf::Sprite* sprite = new sf::Sprite(*it.second);
            int z = 0;
            for(auto& itd : m_drawables) {
                if(itd.second == it.second)
                    z = itd.first;
            }
            gc->addDrawable(it.first, sprite, z);
        }
        gc->setSize(getSize());
        gc->setZ(getZ());
        return gc;
    }

    void GraphicsComponent::addDrawable(std::string name, sf::Sprite* sprite, int z) {
        m_sprites[name] = sprite;
        m_drawables.push_back({z, sprite});
        std::sort(m_drawables.begin(), m_drawables.end());
    }

    void GraphicsComponent::getDrawable(std::string name, sf::Sprite*& sprite) {
        auto& it = m_sprites.find(name);
        if(it != m_sprites.end()) {
            sprite = it->second;
            return;
        }
        LogHandler::log(LogHandler::ERR, "Sprite " + name + "not found in graphics component",
            typeid(this).name());
        sprite = nullptr;
    }

    std::map<std::string, sf::Sprite*>& GraphicsComponent::getSprites() {
        return m_sprites;
    }

    void GraphicsComponent::setZ(float z) {
        m_z = z;
    }

    void GraphicsComponent::setSize(sf::Vector2i size) {
        m_size = size;
    }

    float GraphicsComponent::getZ() {
        return m_z;
    }

    sf::Vector2i GraphicsComponent::getSize() {
        return m_size;
    }

    void GraphicsComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        for(auto& it : m_drawables) {
            target.draw(*it.second, states);
        }
    }
}