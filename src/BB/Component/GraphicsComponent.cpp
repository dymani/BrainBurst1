#include "BB/Component/GraphicsComponent.h"

namespace bb {
    GraphicsComponent::GraphicsComponent() {
    }

    GraphicsComponent* GraphicsComponent::setTexture(sf::Texture& texture) {
        m_sprite.setTexture(texture);
        return this;
    }

    sf::Sprite& GraphicsComponent::getSprite() {
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