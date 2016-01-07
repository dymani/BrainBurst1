#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "BB/World/System/ISystem.h"
#include "BB/World/Component/MovementComponent.h"
#include "BB/World/Component/CollisionComponent.h"

namespace bb {
    class GameStateGame;

    class PhysicsSystem : public ISystem {
    public:
        PhysicsSystem(GameStateGame& game);
        void createList(std::map<std::type_index, std::map<int, IComponent*>*>& lists);
        void createComponent(luabridge::LuaRef& luaE, std::map<std::type_index, IComponent*>& list);
        void createComponent(rapidjson::Value& jsonE, std::map<std::type_index, IComponent*>& list,
            Entity* entity);
        void update();
    private:
        GameStateGame& m_game;
    };
}

#endif