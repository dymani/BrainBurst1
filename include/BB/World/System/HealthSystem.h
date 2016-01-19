#ifndef HEALTH_SYSTEM_H
#define HEALTH_SYSTEM_H

#include "BB/World/System/ISystem.h"
#include "BB/World/Component/HealthComponent.h"
#include "BB/World/Component/DamageComponent.h"

namespace bb {
    class GameStateGame;

    class HealthSystem : public ISystem {
    public:
        HealthSystem(GameStateGame& game);
        void createList(std::map<std::type_index, std::unique_ptr<CList>>& lists);
        void createComponent(luabridge::LuaRef& luaE,
            std::map<std::type_index, std::unique_ptr<IComponent>>& list);
        void createComponent(rapidjson::Value& jsonE,
            std::map<std::type_index, std::unique_ptr<IComponent>>& list, Entity* entity);
        void update();
        void addDamage(DamageComponent* dc, int damage);
    private:
        GameStateGame& m_game;
    };
}

#endif