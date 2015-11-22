#include "BB/Component/GraphicsComponent.h"
#include "BB/World/Entity.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/World/Stage.h"
#include "BB/Component/BreakableComponent.h"

namespace bb {
    GraphicsComponent* GraphicsComponent::create(GameStateGame& game, luabridge::lua_State* L, luabridge::LuaRef& luaGC) {
        GraphicsComponent* gc = new GraphicsComponent(game, -1);
        gc->L = L;
        using namespace luabridge;
        LuaRef luaSize = luaGC["size"];
        gc->setSize({luaSize[1].cast<int>(), luaSize[2].cast<int>()});
        LuaRef luaZ = luaGC["z"];
        gc->setZ(luaZ.cast<float>());
        LuaRef luaDrawables = luaGC["drawables"];
        for(int i = 1; i <= luaDrawables.length(); i++) {
            LuaRef luaDrawable = luaDrawables[i];
            LuaRef luaType = luaDrawable["type"];
            LuaRef luaName = luaDrawable["name"];
            if(luaType.cast<int>() == 0) {
                LuaRef luaTexture = luaDrawable["texture"];
                LuaRef luaTextureRect = luaDrawable["textureRect"];
                sf::IntRect rect = {luaTextureRect[1].cast<int>(), luaTextureRect[2].cast<int>(),
                    luaTextureRect[3].cast<int>(), luaTextureRect[4].cast<int>()};
                sf::Sprite* sprite = new sf::Sprite();
                sf::Vector2i size = gc->getSize();
                if(luaTexture.isString()) {
                    sf::Texture& tex = game.getResourceHandler()->getTexture(luaTexture.cast<std::string>());
                    tex.setSmooth(false);
                    sprite->setTexture(tex);
                }
                sprite->setTextureRect(rect);
                sprite->setOrigin({0, float(rect.height)});
                sprite->setScale({float(size.x) / float(rect.width), float(size.y) / float(rect.height)});
                gc->addDrawable(luaName.cast<std::string>(), sprite, i);
            } else if(luaType.cast<int>() == 1) {
                LuaRef luaText = luaDrawable["text"];
                LuaRef luaFont = luaDrawable["font"];
                LuaRef luaSize = luaDrawable["size"];
                LuaRef luaColor = luaDrawable["color"];
                sf::Text* text = new sf::Text();
                text->setString(luaText.cast<std::string>());
                text->setFont(game.getResourceHandler()->getFont(luaFont.cast<std::string>()));
                text->setCharacterSize(luaSize.cast<int>());
                text->setColor({luaColor[1].cast<sf::Uint8>(), luaColor[2].cast<sf::Uint8>(),
                    luaColor[3].cast<sf::Uint8>(), luaColor[4].cast<sf::Uint8>()});
                gc->addDrawable(luaName.cast<std::string>(), text, i);
            }
        }
        return gc;
    }

    GraphicsComponent::GraphicsComponent(GameStateGame& game, int entity) : IComponent(game, entity) {
    }

    IComponent* GraphicsComponent::copy(rapidjson::Value& value) {
        GraphicsComponent* gc;
        if(value.HasMember("id")) {
            gc = dynamic_cast<GraphicsComponent*>(copy(value["id"].GetInt()));
        } else {
            gc = dynamic_cast<GraphicsComponent*>(copy(-1));
        }
        if(value.HasMember("stage")) {
            sf::Texture& texture = m_game.getResourceHandler()->getTexture(m_game.getWorld()->getStage(std::string(value["stage"].GetString()))->getObjectTexture());
            texture.setSmooth(false);
            for(auto& sprite : gc->getSprites()) {
                sprite.second->setTexture(texture);
            }
        }
        return gc;
    }

    IComponent* GraphicsComponent::copy(int entity) {
        GraphicsComponent* gc = new GraphicsComponent(m_game, entity);
        for(auto& it : m_sprites) {
            sf::Sprite* sprite = new sf::Sprite(*it.second);
            int z = 0;
            for(auto& itd : m_drawables) {
                if(itd.second == it.second)
                    z = itd.first;
            }
            gc->addDrawable(it.first, sprite, z);
        }
        for(auto& it : m_texts) {
            sf::Text* text = new sf::Text(*it.second);
            int z = 0;
            for(auto& itd : m_drawables) {
                if(itd.second == it.second)
                    z = itd.first;
            }
            gc->addDrawable(it.first, text, z);
        }
        gc->setSize(getSize());
        gc->setZ(getZ());
        return gc;
    }

    void GraphicsComponent::draw(sf::RenderWindow& window, sf::Vector2f offset) {
        auto* entity = m_game.getWorld()->getEntity(m_entity);
        auto* bc = entity->getComponent<BreakableComponent>();
        for(auto& sprite : m_sprites) {
            if(bc) {
                auto rect = sprite.second->getTextureRect();
                rect.left = rect.width * bc->getFrame();
                sprite.second->setTextureRect(rect);
            }
            sprite.second->setPosition({entity->getCoord().x * 64,
                window.getSize().y - entity->getCoord().y * 64 - 64});
        }
        for(auto& text : m_texts) {
            text.second->setPosition({entity->getCoord().x * 64,
                window.getSize().y - entity->getCoord().y * 64 - 64});
        }
        m_transform = sf::Transform().translate(offset);
        for(auto& it : m_drawables) {
            window.draw(*it.second, m_transform);
        }
    }

    void GraphicsComponent::addDrawable(std::string name, sf::Sprite* sprite, int z) {
        m_sprites[name] = sprite;
        m_drawables.push_back({z, sprite});
        std::sort(m_drawables.begin(), m_drawables.end());
    }

    void GraphicsComponent::addDrawable(std::string name, sf::Text* text, int z) {
        m_texts[name] = text;
        m_drawables.push_back({z, text});
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

    void GraphicsComponent::getDrawable(std::string name, sf::Text*& text) {
        auto& it = m_texts.find(name);
        if(it != m_texts.end()) {
            text = it->second;
            return;
        }
        LogHandler::log(LogHandler::ERR, "Text" + name + "not found in graphics component",
            typeid(this).name());
        text = nullptr;
    }

    std::map<std::string, sf::Sprite*>& GraphicsComponent::getSprites() {
        return m_sprites;
    }

    std::map<std::string, sf::Text*>& GraphicsComponent::getTexts() {
        return m_texts;
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

    bool GraphicsComponent::getUpdate() {
        return false;
    }
}