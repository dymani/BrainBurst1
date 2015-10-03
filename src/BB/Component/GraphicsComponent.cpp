#include "BB/Component/GraphicsComponent.h"
#include "BB/Handler/ResourceHandler.h"

namespace bb {
    GraphicsComponent::GraphicsComponent() {
    }

    bool GraphicsComponent::createFromLua(luabridge::lua_State* L, luabridge::LuaRef& luaGraphicsComponent,
        ResourceHandler& m_resourceHandler) {
        using namespace luabridge;
        bool error = false;
        if(!luaGraphicsComponent.isTable()) {
            LogHandler::log(LogHandler::ERR, "Incorrect data format for GraphicsComponent",
                typeid(*this).name());
            return false;
        }
        for(int i = 1; i <= luaGraphicsComponent.length(); i++) {
            LuaRef luaDrawable = luaGraphicsComponent[i];
            LuaRef luaDName = luaDrawable["name"];
            LuaRef luaType = luaDrawable["type"];
            LuaRef luaOffsetX = luaDrawable["offsetX"];
            LuaRef luaOffsetY = luaDrawable["offsetY"];
            if(!luaDrawable.isTable()
                || !luaDName.isString()
                || !luaType.isNumber()) {
                LogHandler::log(LogHandler::WRN, "Incorrect data format for drawable["
                    + std::to_string(i) + "]", typeid(*this).name());
                error = true;
                continue;
            }
            float offsetX = 0;
            float offsetY = 0;
            if(luaOffsetX.isNumber()
                && luaOffsetY.isNumber()) {
                offsetX = luaOffsetX.cast<float>();
                offsetY = luaOffsetY.cast<float>();
            }
            if(luaType.cast<int>() == SPRITE) {
                LuaRef luaTexture = luaDrawable["texture"];
                LuaRef luaTextureRects = luaDrawable["textureRects"];
                if(!luaTexture.isString()) {
                    LogHandler::log(LogHandler::WRN, "Incorrect data format for sprite["
                        + std::to_string(i) + "]", typeid(*this).name());
                    error = true;
                    continue;
                }
                addDrawable(SPRITE, luaDName.cast<std::string>());
                sf::Sprite* sprite = getDrawable<sf::Sprite>(luaDName.cast<std::string>());
                sprite->setTexture(m_resourceHandler.getTexture(luaTexture.cast<std::string>()));
                if(luaTextureRects.isTable()) {
                    for(int j = 1; j <= luaTextureRects.length(); j++) {
                        LuaRef luaTextureRect = luaTextureRects[j];
                        if(!luaTextureRect.isTable()) {
                            LogHandler::log(LogHandler::WRN,
                                "Incorrect data format for texture rects[" + std::to_string(j)
                                + "] of sprite[" + std::to_string(i) + "]", typeid(*this).name());
                            error = true;
                            continue;
                        }
                        addTextureRect(luaTextureRect[1].cast<std::string>(), {
                            luaTextureRect[2].cast<int>(), luaTextureRect[3].cast<int>(),
                            luaTextureRect[4].cast<int>(), luaTextureRect[5].cast<int>()});
                    }
                    sprite->setTextureRect(getTextureRect("default"));
                }
                sprite->setPosition(offsetX, offsetY);
            } else if(luaType.cast<int>() == TEXT) {
                LuaRef luaFont = luaDrawable["font"];
                LuaRef luaSize = luaDrawable["size"];
                LuaRef luaAlign = luaDrawable["align"];
                if(!luaFont.isString()
                    || !luaSize.isNumber()
                    || !luaAlign.isNumber()) {
                    LogHandler::log(LogHandler::WRN, "Incorrect data format for text[" + std::to_string(i)
                        + "]", typeid(*this).name());
                    error = true;
                    continue;
                }
                addDrawable(TEXT, luaDName.cast<std::string>());
                sf::Text* text = getDrawable<sf::Text>(luaDName.cast<std::string>());
                text->setFont(m_resourceHandler.getFont(luaFont.cast<std::string>()));
                text->setCharacterSize(luaSize.cast<unsigned int>());
                setAlign(luaAlign.cast<int>());
                text->setPosition(offsetX, offsetY);
            } else {
                LogHandler::log(LogHandler::WRN, "Incorrect type id "
                    + std::to_string(luaType.cast<int>()) + " for drawable[" + std::to_string(i)
                    + "]", typeid(*this).name());
                error = true;
                continue;
            }
        }
        return !error;
    }

    bool GraphicsComponent::initFromLua(luabridge::lua_State* L, luabridge::LuaRef& luaEntity) {
        using namespace luabridge;
        bool error = false;
        LuaRef luaZ = luaEntity["z"];
        if(!luaZ.isNumber()) {
            LogHandler::log(LogHandler::WRN, "Incorrect data format for z", typeid(*this).name());
            setZ(0.0F);
            error = true;
        } else {
            setZ(luaZ.cast<float>());
        }
        if(getDrawable<sf::Text>("text")) {
            LuaRef luaText = luaEntity["text"];
            std::string string = "";
            if(luaText.isString()) {
                string = luaText.cast<std::string>();
            } else {
                LogHandler::log(LogHandler::WRN, "Incorrect data format for text", typeid(*this).name());
                error = true;
            }
            sf::Text* text = getDrawable<sf::Text>("text");
            text->setString(string);
            switch(getAlign()) {
                case 1:
                    text->setOrigin(text->getLocalBounds().left
                        + text->getLocalBounds().width / 2, text->getLocalBounds().top);
                    break;
                case 2:
                    text->setOrigin(text->getLocalBounds().left + text->getLocalBounds().width,
                        text->getLocalBounds().top);
                    break;
                case 0:
                default:
                    text->setOrigin(text->getLocalBounds().left, text->getLocalBounds().top);
                    break;
            }
        }
        return !error;
    }

    IComponent* GraphicsComponent::copy() {
        GraphicsComponent* gc = new GraphicsComponent();
        for(auto& drawable : m_drawables) {
            gc->m_drawables.push_back({drawable.first, drawable.second->copy()});
        }
        gc->m_textureRects = m_textureRects;
        gc->m_z = m_z;
        gc->m_align = m_align;
        return gc;
    }

    GraphicsComponent* GraphicsComponent::addDrawable(Type type, std::string name) {
        Drawable* drawable;
        switch(type) {
            case SPRITE:
                drawable = new Drawable(Drawable::SPRITE, new sf::Sprite());
                break;
            case TEXT:
                drawable = new Drawable(Drawable::TEXT, new sf::Text());
                break;
            default:
                drawable = new Drawable(Drawable::SPRITE, new sf::Sprite());
                LogHandler::log(LogHandler::ERR, "Type id " + std::to_string(int(type)) + " not found",
                    typeid(*this).name());
                break;
        }
        m_drawables.push_back({name, drawable});
        return this;
    }

    GraphicsComponent* GraphicsComponent::addTextureRect(std::string name, sf::IntRect rect) {
        m_textureRects[name] = rect;
        return this;
    }

    GraphicsComponent* GraphicsComponent::setZ(float z) {
        m_z = z;
        return this;
    }

    GraphicsComponent* GraphicsComponent::setAlign(int align) {
        m_align = align;
        return this;
    }

    sf::IntRect GraphicsComponent::getTextureRect(std::string name) {
        if(m_textureRects.find(name) == m_textureRects.end())
            LogHandler::log(LogHandler::ERR, "Texture rect \"" + name + "\" not found",
                typeid(*this).name());
        return m_textureRects[name];
    }

    std::vector<std::pair<std::string, Drawable*>>& GraphicsComponent::getDrawables() {
        return m_drawables;
    }

    float GraphicsComponent::getZ() {
        return m_z;
    }

    int GraphicsComponent::getAlign() {
        return m_align;
    }

    Drawable::Drawable(Type type, sf::Drawable* drawable) {
        m_type = type;
        m_drawable = drawable;
    }

    Drawable* Drawable::copy() {
        sf::Drawable* drawable;
        switch(m_type) {
            case SPRITE:
                drawable = new sf::Sprite(*dynamic_cast<sf::Sprite*>(m_drawable));
                break;
            case TEXT:
                drawable = new sf::Text(*dynamic_cast<sf::Text*>(m_drawable));
                break;
            default:
                drawable = new sf::Sprite(*dynamic_cast<sf::Sprite*>(m_drawable));
                LogHandler::log(LogHandler::ERR, "Type id " + std::to_string(int(m_type))
                    + " not found", typeid(*this).name());
                break;
        }
        return new Drawable(m_type, drawable);
    }
}