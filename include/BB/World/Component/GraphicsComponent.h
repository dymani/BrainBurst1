#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

#include <SFML/Graphics.hpp>
#include "BB/World/Component/IComponent.h"

namespace bb {
    class GraphicsComponent : public IComponent {
        friend class GraphicsSystem;
    public:
        GraphicsComponent() : IComponent() {
        }
    private:
        float m_z;
        sf::Sprite m_sprite;
        bool hasTexture;
        std::string m_stageObjectTexture;
    };
}

#endif