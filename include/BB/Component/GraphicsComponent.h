#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

#include <SFML/Graphics.hpp>
#include "BB/Component/IComponent.h"

namespace bb {
    class GraphicsComponent: public IComponent {
    public:
        GraphicsComponent();
        GraphicsComponent* setColor(sf::Color color);
        sf::Color& getColor();
    private:
        sf::Color m_color;
    };
}

#endif