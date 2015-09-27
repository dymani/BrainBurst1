#include "BB/Component/GuiComponent.h"
#include "BB/Component/GraphicsComponent.h"

namespace bb {
    GuiComponent::GuiComponent() {
    }

    IComponent* GuiComponent::copy() {
        GuiComponent* gc = new GuiComponent();
        gc->m_buttonState = m_buttonState;
        gc->m_id = m_id;
        gc->m_size = m_size;
        gc->m_text = m_text;
        gc->m_type = m_type;
        return gc;
    }


    bool GuiComponent::update(GraphicsComponent* gc) {
        if(m_buttonState == RELEASED) {
            m_buttonState = HOVER;
            gc->setTextureRect("hover");
            return true;
        }
        return false;
    }

    bool GuiComponent::handleInput(sf::Event& windowEvent, sf::RenderWindow& window, sf::Vector2f coord,
        GraphicsComponent* gc) {
        int mouseX = sf::Mouse::getPosition(window).x;
        int mouseY = sf::Mouse::getPosition(window).y;
        bool isLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        bool isHover = mouseX > coord.x
            && mouseX < coord.x + m_size.x
            && mouseY > coord.y
            && mouseY < coord.y + m_size.y;
        if(isHover) {
            if(isLeftPressed) {
                if(m_buttonState == HOVER || m_buttonState == LEFT) {
                    m_buttonState = PRESSED;
                    gc->setTextureRect("pressed");
                }
            } else {
                if(m_buttonState == IDLE || m_buttonState == LEFT) {
                    m_buttonState = HOVER;
                    gc->setTextureRect("hover");
                } else if(m_buttonState == PRESSED) {
                    m_buttonState = RELEASED;
                    gc->setTextureRect("default");
                }
            }
            return true;
        } else {
            if(isLeftPressed) {
                if(m_buttonState == HOVER) {
                    m_buttonState = IDLE;
                    gc->setTextureRect("default");
                } else if(m_buttonState == PRESSED) {
                    m_buttonState = LEFT;
                    gc->setTextureRect("default");
                }
            } else {
                if(m_buttonState == HOVER || m_buttonState == LEFT) {
                    m_buttonState = IDLE;
                    gc->setTextureRect("default");
                }
            }
            return false;
        }
    }

    GuiComponent* GuiComponent::setType(int type) {
        m_type = Type(type);
        if(m_type == BUTTON)
            m_buttonState = IDLE;
        return this;
    }

    GuiComponent* GuiComponent::setSize(sf::Vector2i size) {
        m_size = size;
        return this;
    }


    GuiComponent* GuiComponent::setText(sf::Font& font, int size, std::string text) {
        m_text.setFont(font);
        m_text.setCharacterSize(size);
        m_text.setString(text);
        m_text.setPosition(float((m_size.x - m_text.getLocalBounds().width) / 2),
            float((m_size.y - m_text.getLocalBounds().height) / 2));
        return this;
    }

    GuiComponent* GuiComponent::setId(int id) {
        m_id = id;
        return this;
    }

    int GuiComponent::getType() {
        return m_type;
    }

    int GuiComponent::getId() {
        return m_id;
    }
}