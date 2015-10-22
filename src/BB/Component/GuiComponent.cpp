#include "BB/Component/GuiComponent.h"
#include "BB/Component/GraphicsComponent.h"

namespace bb {
    GuiComponent::GuiComponent() {
    }

    bool GuiComponent::createFromLua(luabridge::lua_State* L, luabridge::LuaRef& luaGuiComponent) {
        using namespace luabridge;
        LuaRef luaType = luaGuiComponent["type"];
        LuaRef luaWidth = luaGuiComponent["width"];
        LuaRef luaHeight = luaGuiComponent["height"];
        if(!luaType.isNumber()
            || !luaWidth.isNumber()
            || !luaHeight.isNumber()) {
            LogHandler::log(LogHandler::ERR, "Incorrect data foramt", typeid(*this).name());
            return false;
        }
        setType(luaType.cast<int>());
        if(getType() == BUTTON) {
            setSize({luaWidth.cast<int>(), luaHeight.cast<int>()});
        } else if(getType() == TEXTBOX) {
            setSize({luaWidth.cast<int>(), luaHeight.cast<int>()});
        } else {
            LogHandler::log(LogHandler::ERR, "Type id " + std::to_string(getType()) + " not found",
                typeid(*this).name());
            return false;
        }
        return true;
    }

    bool GuiComponent::initFromLua(luabridge::lua_State* L, luabridge::LuaRef& luaEntity) {
        using namespace luabridge;
        bool error = false;
        if(!luaEntity["id"].isNumber()) {
            LogHandler::log(LogHandler::ERR, "Incorrect data foramt for id", typeid(*this).name());
            return false;
        }
        setId(luaEntity["id"].cast<int>());
        if(m_type == TEXTBOX) {
            if(!luaEntity["characters"].isTable()) {
                LogHandler::log(LogHandler::ERR, "Incorrect data foramt for characters",
                    typeid(*this).name());
                return false;
            }
            m_characters = 0;
            LuaRef luaCharacters = luaEntity["characters"];
            for(int i = 1; i <= luaCharacters.length(); i++) {
                if(!luaCharacters[i].isString()) {
                    LogHandler::log(LogHandler::ERR, "Incorrect data foramt for character",
                        typeid(*this).name());
                    error = true;
                    continue;
                }
                std::string characters = luaCharacters[i].cast<std::string>();
                if(characters == "alphabet")
                    m_characters += Alphabet;
                else if(characters == "number")
                    m_characters += Number;
                else if(characters == "space")
                    m_characters += Space;
            }
        }
        return !error;
    }

    IComponent* GuiComponent::copy() {
        GuiComponent* gc = new GuiComponent();
        gc->m_buttonState = m_buttonState;
        gc->m_id = m_id;
        gc->m_size = m_size;
        gc->m_type = m_type;
        gc->m_text = m_text;
        gc->m_textboxState = m_textboxState;
        return gc;
    }

    bool GuiComponent::update(GraphicsComponent* gc) {
        switch(m_type) {
            case BUTTON:
                if(m_buttonState == RELEASED_B) {
                    m_buttonState = HOVER_B;
                    //gc->getDrawable<sf::Sprite>("sprite")->setTextureRect(gc->getTextureRect("hover"));
                    return true;
                }
                break;
            case TEXTBOX:
                if(m_textboxState == RETURN_T) {
                    return true;
                }
                break;
        }
        return false;
    }

    bool GuiComponent::handleInput(sf::Event& windowEvent, sf::RenderWindow& window, sf::Vector2f coord,
        GraphicsComponent* gc) {
        if(m_type == BUTTON) {
            int mouseX = sf::Mouse::getPosition(window).x;
            int mouseY = sf::Mouse::getPosition(window).y;
            bool isLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
            bool isHover = mouseX > coord.x
                && mouseX < coord.x + m_size.x
                && mouseY > coord.y
                && mouseY < coord.y + m_size.y;
            if(isHover) {
                if(isLeftPressed) {
                    if(m_buttonState == HOVER_B || m_buttonState == LEFT_B) {
                        m_buttonState = PRESSED_B;
                        //gc->getDrawable<sf::Sprite>("sprite")->setTextureRect(gc->getTextureRect("pressed"));
                    }
                } else {
                    if(m_buttonState == IDLE_B || m_buttonState == LEFT_B) {
                        m_buttonState = HOVER_B;
                        //gc->getDrawable<sf::Sprite>("sprite")->setTextureRect(gc->getTextureRect("hover"));
                    } else if(m_buttonState == PRESSED_B) {
                        m_buttonState = RELEASED_B;
                        //gc->getDrawable<sf::Sprite>("sprite")->setTextureRect(gc->getTextureRect("default"));
                    }
                }
                return true;
            } else {
                if(isLeftPressed) {
                    if(m_buttonState == HOVER_B) {
                        m_buttonState = IDLE_B;
                        //gc->getDrawable<sf::Sprite>("sprite")->setTextureRect(gc->getTextureRect("default"));
                    } else if(m_buttonState == PRESSED_B) {
                        m_buttonState = LEFT_B;
                        //gc->getDrawable<sf::Sprite>("sprite")->setTextureRect(gc->getTextureRect("default"));
                    }
                } else {
                    if(m_buttonState == HOVER_B || m_buttonState == LEFT_B) {
                        m_buttonState = IDLE_B;
                        //gc->getDrawable<sf::Sprite>("sprite")->setTextureRect(gc->getTextureRect("default"));
                    }
                }
                return false;
            }
        } else if(m_type == TEXTBOX) {
            int mouseX = sf::Mouse::getPosition(window).x;
            int mouseY = sf::Mouse::getPosition(window).y;
            bool isLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
            bool isHover = mouseX > coord.x
                && mouseX < coord.x + m_size.x
                && mouseY > coord.y
                && mouseY < coord.y + m_size.y;
            if(isHover) {
                if(isLeftPressed) {
                    if(m_textboxState == HOVER_T || m_textboxState == LEFT_T) {
                        m_textboxState = PRESSED_T;
                        //gc->getDrawable<sf::Sprite>("sprite")->setTextureRect(gc->getTextureRect("hover"));
                    }
                } else {
                    if(m_textboxState == IDLE_T || m_textboxState == LEFT_T) {
                        m_textboxState = HOVER_T;
                        //gc->getDrawable<sf::Sprite>("sprite")->setTextureRect(gc->getTextureRect("hover"));
                    } else if(m_textboxState == PRESSED_T) {
                        m_textboxState = FOCUSED_T;
                        //gc->getDrawable<sf::Sprite>("sprite")->setTextureRect(gc->getTextureRect("focused"));
                    }
                }
            } else {
                if(isLeftPressed) {
                    if(m_textboxState == FOCUSED_T || m_textboxState == HOVER_T) {
                        m_textboxState = IDLE_T;
                        //gc->getDrawable<sf::Sprite>("sprite")->setTextureRect(gc->getTextureRect("default"));
                    } else if(m_textboxState == PRESSED_T) {
                        m_textboxState = LEFT_T;
                        //gc->getDrawable<sf::Sprite>("sprite")->setTextureRect(gc->getTextureRect("default"));
                    }
                } else {
                    if(m_textboxState == HOVER_T || m_textboxState == LEFT_T) {
                        m_textboxState = IDLE_T;
                        //gc->getDrawable<sf::Sprite>("sprite")->setTextureRect(gc->getTextureRect("default"));
                    }
                }
            }
            if(m_textboxState == FOCUSED_T) {
                if(windowEvent.type == sf::Event::KeyPressed) {
                    if(windowEvent.key.code == sf::Keyboard::BackSpace && m_text.size() != 0) {
                        m_text.pop_back();
                        //gc->getDrawable<sf::Text>("text")->setString(m_text);
                    } else if(windowEvent.key.code == sf::Keyboard::Return && m_text.size() != 0) {
                        m_textboxState = RETURN_T;
                    }
                    return true;
                } else if(windowEvent.type == sf::Event::TextEntered) {
                    bool canInput = false;
                    auto& unicode = windowEvent.text.unicode;
                    if(unicode >= (char)'0' && unicode <= (char)'9') {
                        if(m_characters & Number)
                            canInput = true;
                    } else if((unicode >= (char)'a' && unicode <= (char)'z')
                        || (unicode >= (char)'A' && unicode <= (char)'Z')) {
                        if(m_characters & Alphabet)
                            canInput = true;
                    } else if(unicode == (char)' ') {
                        if(m_characters & Space)
                            canInput = true;
                    }
                    if(canInput) {
                        m_text += (char)unicode;
                        //gc->getDrawable<sf::Text>("text")->setString(m_text);
                    }
                    return true;
                }
            }
            return isHover;
        }
        return false;
    }

    GuiComponent* GuiComponent::setType(int type) {
        m_type = Type(type);
        if(m_type == BUTTON)
            m_buttonState = IDLE_B;
        else if(m_type == TEXTBOX)
            m_textboxState = IDLE_T;
        return this;
    }

    GuiComponent* GuiComponent::setSize(sf::Vector2i size) {
        m_size = size;
        return this;
    }

    GuiComponent* GuiComponent::setId(int id) {
        m_id = id;
        return this;
    }

    GuiComponent* GuiComponent::setCharacters(int characters) {
        if(characters < Max)
            m_characters = Characters(characters);
        return this;
    }

    int GuiComponent::getType() {
        return m_type;
    }

    int GuiComponent::getId() {
        return m_id;
    }

    std::string GuiComponent::getText() {
        if(m_type == TEXTBOX)
            return m_text;
        return "";
    }
}