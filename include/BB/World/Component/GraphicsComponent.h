#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

#include <SFML/Graphics.hpp>
#include "BB/World/Component/IComponent.h"

namespace bb {
    struct Animation {
        sf::IntRect frameStrip;
        int frames;
        float speed;
    };

    class GraphicsComponent : public IComponent {
        friend class GraphicsSystem;
    public:
        GraphicsComponent() : IComponent() {
        }
    private:
        int m_type;
        float m_z;
        sf::Sprite m_sprite;
        std::map<std::string, Animation> m_animations;
        std::string m_currentAnimation;
        int m_currentFrame;
        int m_frameInterval;
        bool m_hasTexture;
        std::string m_stageObjectTexture;
    };
}

#endif