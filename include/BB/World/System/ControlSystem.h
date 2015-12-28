#ifndef CONTROL_SYSTEM_H
#define CONTROL_SYSTEM_H

#include <SFML/Graphics.hpp>
#include "BB/World/System/ISystem.h"
#include "BB/World/Component/PlayerComponent.h"
#include "BB/World/Component/MovementComponent.h"

namespace bb {
    class GameStateGame;

    class ControlSystem : public ISystem {
    public:
        ControlSystem(GameStateGame& game);
        IComponent* createComponent(luabridge::LuaRef& luaE);
        IComponent* createComponent(IComponent* component, rapidjson::Value& jsonE);
        void handleInput();
        void handleInput(sf::Event& windowEvent);
    private:
        GameStateGame& m_game;
    };
}

#endif