#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

#include <SFML/Graphics.hpp>
#include "BB/Component/IComponent.h"

namespace bb {
    class GraphicsComponent: public IComponent {
    public:
        GraphicsComponent();
        GraphicsComponent* setTexture(sf::Texture& texture);
        sf::Sprite& getSprite();
        GraphicsComponent* setZ(float z);
        float getZ();
    private:
        sf::Sprite m_sprite;
        float m_z;
    };
}

#endif