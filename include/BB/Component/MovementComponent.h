#ifndef MOVEMENT_COMPONENT_H
#define MOVEMENT_COMPONENT_H

#include "BB/Component/IComponent.h"
#include <SFML/Graphics.hpp>
#include "BB/Handler/LogHandler.h"

namespace bb {
    class MovementComponent: public IComponent {
    public:
        static MovementComponent* create(GameStateGame& game, luabridge::lua_State* L,
            luabridge::LuaRef& luaMC);
        MovementComponent(GameStateGame& game, int entity);
        IComponent* copy(rapidjson::Value& value);
        IComponent* copy(int entity);
        void move();
        void update();
        void setVelocity(sf::Vector2f velocity);
        void setVelocityX(float x);
        void setVelocityY(float y);
        void addVelocity(float x, float y);
        void setNewCoord(sf::Vector2f newCoord);
        void setNewCoordX(float x);
        void setNewCoordY(float y);
        sf::Vector2f getVelocity();
        sf::Vector2f getNewCoord();
        void isOnGround(bool isOnGround);
        bool isOnGround();
        bool getUpdate();
    private:
        bool m_init = false;
        bool m_isOnGround;
        sf::Vector2f m_newCoord;
        sf::Vector2f m_velocity;
    };
}

#endif