#include "BB/Component/GraphicsComponent.h"

namespace bb {
    GraphicsComponent::GraphicsComponent() {
    }

    IComponent* GraphicsComponent::copy() {
        GraphicsComponent* gc = new GraphicsComponent();
        gc->m_type = m_type;
        gc->m_sprite = m_sprite;
        gc->m_textureRects = m_textureRects;
        gc->m_text = m_text;
        gc->m_z = m_z;
        return gc;
    }

    GraphicsComponent* GraphicsComponent::setType(int type) {
        m_type = Type(type);
        return this;
    }

    GraphicsComponent* GraphicsComponent::setText(std::string text) {
        m_text.setString(text);
        m_text.setOrigin({m_text.getGlobalBounds().width / 2, m_text.getGlobalBounds().height / 2});
        return this;
    }

    GraphicsComponent* GraphicsComponent::setFont(sf::Font& font, unsigned int size) {
        m_text.setFont(font);
        m_text.setCharacterSize(size);
        m_text.setOrigin({m_text.getGlobalBounds().width / 2, m_text.getGlobalBounds().height / 2});
        return this;
    }

    GraphicsComponent* GraphicsComponent::setTexture(sf::Texture& texture) {
        m_sprite.setTexture(texture);
        return this;
    }

    GraphicsComponent* GraphicsComponent::addTextureRect(std::string name, sf::IntRect rect) {
        m_textureRects[name] = rect;
        return this;
    }

    GraphicsComponent* GraphicsComponent::setTextureRect(std::string name) {
        if(m_textureRects.find(name) == m_textureRects.end())
            std::cerr << "Error while setting texture rect " + name + ".\n";
        else
            m_sprite.setTextureRect(m_textureRects[name]);
        return this;
    }

    unsigned int GraphicsComponent::getType() {
        return m_type;
    }

    sf::Drawable& GraphicsComponent::getDrawable() {
        switch(m_type) {
            case 0:
                return m_sprite;
            case 1:
                return m_text;
        }
        std::cerr << "Error while getting drawable " + std::to_string(m_type) + " of entity.\n";
        return m_sprite;
    }

    GraphicsComponent* GraphicsComponent::setZ(float z) {
        m_z = z;
        return this;
    }

    float GraphicsComponent::getZ() {
        return m_z;
    }
}