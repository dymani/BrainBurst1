#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "BB/World/System/ISystem.h"
#include "BB/World/Component/MovementComponent.h"

namespace bb {
    class GameStateGame;

    class PhysicsSystem : public ISystem {
    public:
        PhysicsSystem(GameStateGame& game);
        IComponent* createComponent(luabridge::LuaRef& luaE);
        IComponent* createComponent(IComponent* component, rapidjson::Value& jsonE);
        void update();
    private:
        GameStateGame& m_game;
    };
}

#endif