#include "BB/World/Object.h"

namespace bb {
    Object::Object(sf::IntRect textureRect, sf::Vector2i size, sf::Vector2f coord) {
        m_sprite.setTextureRect(textureRect);
        m_sprite.setScale({float(size.x) / float(textureRect.width),
            float(size.y) / float(textureRect.height)});
        m_sprite.setPosition(float(int(coord.x * 64)), float(int(coord.y - size.y)));
    }

    Object::Object(Object & obj) {
        m_sprite = obj.m_sprite;
    }

    void Object::draw(sf::RenderTarget& target, sf::Texture & texture) {
        m_sprite.setTexture(texture);
        target.draw(m_sprite);
    }

    void Object::setCoord(sf::Vector2f coord) {
        m_sprite.setPosition(float(int(coord.x * 64)),
            float(int(coord.y - m_sprite.getGlobalBounds().height)));
    }
}