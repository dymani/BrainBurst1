#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>

namespace bb {
    class Object {
    public:
        Object(sf::IntRect textureRect, sf::Vector2i size, sf::Vector2f coord = sf::Vector2f());
        Object(Object& obj);
        void draw(sf::RenderTarget& target, sf::Texture& texture);
        void setCoord(sf::Vector2f coord);
    private:
        sf::Sprite m_sprite;
    };
}

#endif