#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include <SFML/Graphics.hpp>
#include "BB/Component/IComponent.h"

namespace bb {
    class GraphicsComponent;

    class GuiComponent: public IComponent {
    public:
        GuiComponent();
        IComponent* copy();
        bool update(GraphicsComponent* gc);
        bool handleInput(sf::Event& windowEvent, sf::RenderWindow& window, sf::Vector2f coord,
            GraphicsComponent* gc);
        GuiComponent* setType(int type);
        GuiComponent* setSize(sf::Vector2i size);
        GuiComponent* setText(sf::Font& font, int size, std::string text);
        GuiComponent* setId(int id);
        int getType();
        int getId();
        enum Type {
            BUTTON
        };
    private:
        Type m_type;
        sf::Vector2i m_size;
        sf::Text m_text;
        int m_id;
        enum ButtonState {
            IDLE, HOVER, PRESSED, LEFT, RELEASED
        } m_buttonState;
    };
}

#endif