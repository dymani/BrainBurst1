#include "BB/Component/GraphicsComponent.h"

namespace bb {
    GraphicsComponent::GraphicsComponent() {
        m_color = sf::Color::Black;
    }

    GraphicsComponent* GraphicsComponent::setColor(sf::Color color) {
        m_color = color;
        return this;
    }

    sf::Color& GraphicsComponent::getColor() {
        return m_color;
    }
}