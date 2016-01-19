#include "BB/World/LuaEntity.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    LuaEntity::LuaEntity(GameStateGame& game, int entity) : m_game(game), m_entity(entity) {
    }

    void LuaEntity::setDamage(int damage) {
        auto* e = m_game.getWorld().getField()->getEntity(m_entity);
        auto* hc = m_game.getWorld().getField()->getComponent<HealthComponent>(m_entity);
        auto* dc = m_game.getWorld().getField()->getComponent<DamageComponent>(m_entity);
        if(!hc) return;
        if(!dc)
            e->addComponent(std::type_index(typeid(DamageComponent)), new DamageComponent(damage));
        else
            m_game.getWorld().getSystem<HealthSystem>().addDamage(dc, damage);
    }
}