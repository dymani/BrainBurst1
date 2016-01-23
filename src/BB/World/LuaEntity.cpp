#include "BB/World/LuaEntity.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    LuaEntity::LuaEntity(GameStateGame& game, int entity) : m_game(game), m_entity(entity) {
    }

    void LuaEntity::setDamage(int damage) {
        auto* e = m_game.getWorld().getField()->getEntity(m_entity);
        auto* hc = m_game.getWorld().getField()->getComponent<HealthComponent>(m_entity);
        if(!hc) return;
        m_game.getWorld().getSystem<HealthSystem>().addDamage(hc, damage);
    }
}