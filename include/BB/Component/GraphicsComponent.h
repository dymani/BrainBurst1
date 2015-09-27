#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

#include <SFML/Graphics.hpp>
#include "BB/Component/IComponent.h"
#include <iostream>

namespace bb {
    class GraphicsComponent: public IComponent {
    public:
        GraphicsComponent();
        IComponent* copy();
        GraphicsComponent* setType(int type);
        GraphicsComponent* setText(std::string text);
        GraphicsComponent* setFont(sf::Font& font, unsigned int size);
        GraphicsComponent* setTexture(sf::Texture& texture);
        GraphicsComponent* addTextureRect(std::string name, sf::IntRect rect);
        GraphicsComponent* setTextureRect(std::string name);
        GraphicsComponent* setZ(float z);
        unsigned int getType();
        sf::Drawable& getDrawable();
        float getZ();
        enum Type {
            SPRITE, TEXT
        };
    private:
        Type m_type;
        sf::Text m_text;
        sf::Sprite m_sprite;
        std::map<std::string, sf::IntRect> m_textureRects;
        float m_z;
    };
}

#endif