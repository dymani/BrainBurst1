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
        void createList(std::map<std::type_index, std::unique_ptr<CList>>& lists);
        void createComponent(luabridge::LuaRef& luaE,
            std::map<std::type_index, std::unique_ptr<IComponent>>& list);
        void createComponent(rapidjson::Value& jsonE,
            std::map<std::type_index, std::unique_ptr<IComponent>>& list, Entity* entity);
        void handleInput();
        void handleInput(sf::Event& windowEvent);
    private:
        GameStateGame& m_game;
    };
}

#endif