#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include <SFML/Graphics.hpp>
#include "BB/Component/IComponent.h"
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

namespace bb {
    class GraphicsComponent;

    class GuiComponent: public IComponent {
    public:
        GuiComponent();
        IComponent* copy();
        bool createFromLua(luabridge::lua_State* L, luabridge::LuaRef& luaGuiComponent);
        bool initFromLua(luabridge::lua_State* L, luabridge::LuaRef& luaEntity);
        bool update(GraphicsComponent* gc);
        bool handleInput(sf::Event& windowEvent, sf::RenderWindow& window, sf::Vector2f coord,
            GraphicsComponent* gc);
        GuiComponent* setType(int type);
        GuiComponent* setSize(sf::Vector2i size);
        GuiComponent* setId(int id);
        GuiComponent* setCharacters(int characters);
        int getType();
        int getId();
        std::string getText();
        int getCharacters();
        enum Type {
            BUTTON, TEXTBOX
        };
        enum Characters {
            Alphabet = 1,
            Number = 2,
            Space = 4,
            Max = 8
        };
        friend Characters operator|(Characters a, Characters b) {
            return static_cast<Characters>(static_cast<int>(a) | static_cast<int>(b));
        }
    private:
        Type m_type;
        sf::Vector2i m_size;
        int m_id;
        std::string m_text;
        enum ButtonState {
            IDLE_B, HOVER_B, PRESSED_B, LEFT_B, RELEASED_B
        } m_buttonState;
        enum TextboxState {
            IDLE_T, HOVER_T, PRESSED_T, LEFT_T, FOCUSED_T, RETURN_T
        } m_textboxState;
        int m_characters;
    };
}

#endif