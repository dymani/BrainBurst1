#ifndef PLAYER_COMPONENT_H
#define PLAYER_COMPONENT_H

#include "BB/Component/IComponent.h"
#include "BB/Handler/LogHandler.h"
#include "BB/Handler/ResourceHandler.h"

namespace bb {
    class PlayerComponent: public IComponent {
    public:
        static PlayerComponent* create(GameStateGame& game, luabridge::lua_State* L, luabridge::LuaRef& luaPC);
        PlayerComponent(GameStateGame& game, int entity);
        IComponent* copy(rapidjson::Value& value);
        IComponent* copy(int entity);
        void handleInput();
    private:
        enum State {
            IDLE, JUMPING, CROUCHING
        } m_state;
        bool m_facingLeft;
        float m_speed;
        enum Walk {
            STAND, L_WALKING, R_WALKING, L_PRE, R_PRE, L_SPRINTING, R_SPRINTING
        } m_walk;
        int m_sprintCount;
    };
}

#endif