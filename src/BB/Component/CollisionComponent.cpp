#include "BB/Component/CollisionComponent.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/Component/MovementComponent.h"
#include "BB/Component/GraphicsComponent.h"

namespace bb {
    CollisionComponent* CollisionComponent::create(GameStateGame& game, luabridge::lua_State* L,
        luabridge::LuaRef& luaCC) {
        auto* cc = new CollisionComponent(game, -1);
        using namespace luabridge;
        LuaRef luaType = luaCC["type"];
        cc->m_type = Type(luaType.cast<int>());
        LuaRef luaHitbox = luaCC["hitbox"];
        cc->m_hitboxI = {luaHitbox[1].cast<int>(), luaHitbox[2].cast<int>(), luaHitbox[3].cast<int>(),
            luaHitbox[4].cast<int>()};
        return cc;
    }

    CollisionComponent::CollisionComponent(GameStateGame& game, int entity) : IComponent(game, entity) {
    }

    IComponent* CollisionComponent::copy(rapidjson::Value& value) {
        CollisionComponent* cc;
        if(value.HasMember("id"))
            cc = dynamic_cast<CollisionComponent*>(copy(value["id"].GetInt()));
        else
            cc = dynamic_cast<CollisionComponent*>(copy(-1));
        return cc;
    }

    IComponent* CollisionComponent::copy(int entity) {
        auto* cc = new CollisionComponent(m_game, entity);
        cc->m_type = m_type;
        cc->m_size = m_size;
        cc->m_hitboxI = m_hitboxI;
        cc->m_hitboxF = m_hitboxF;
        return cc;
    }

    bool CollisionComponent::collide(int entity) {
        m_size = m_game.getWorld()->getEntity(m_entity)->getComponent<GraphicsComponent>()->getSize();
        if(m_type == MOVABLE) {
            auto* mc = m_game.getWorld()->getEntity(m_entity)->getComponent<MovementComponent>();
            sf::Vector2f coord = mc->getNewCoord();
            getHitbox();
            sf::FloatRect hitbox = m_game.getWorld()->getEntity(entity)->
                getComponent<CollisionComponent>()->getHitbox();
            float colT = m_hitboxF.top + m_hitboxF.height - hitbox.top;
            float colL = m_hitboxF.left + m_hitboxF.width - hitbox.left;
            float colB = hitbox.top + hitbox.height - m_hitboxF.top;
            float colR = hitbox.left + hitbox.width - m_hitboxF.left;
            if(colB < colT && colB < colL && colB < colR) { //top
                coord.y = -hitbox.top - hitbox.height + float(m_hitboxI.top - m_size.y) / 64.0F;
                mc->setVelocityY(0);
            }
            if(colT < colB && colT < colL && colT < colR) { //bottom
                coord.y = -hitbox.top + float(m_hitboxI.top + m_hitboxI.height - m_size.y) / 64.0F;
                mc->setVelocityY(0);
                mc->isOnGround(true);
            }
            if(colR < colL && colR < colT && colR < colB) { //left
                coord.x = hitbox.left + hitbox.width - float(m_hitboxI.left) / 64.0F;
                mc->setVelocityX(0);
            }
            if(colL < colR && colL < colT && colL < colB) { //right
                coord.x = hitbox.left - float(m_hitboxI.width + m_hitboxI.left) / 64.0F;
                mc->setVelocityX(0);
            }
            mc->setNewCoord(coord);
        }
        return (m_type == BREAKABLE);
    }

    sf::FloatRect CollisionComponent::getHitbox() {
        m_size = m_game.getWorld()->getEntity(m_entity)->getComponent<GraphicsComponent>()->getSize();
        sf::Vector2f coord;
        if(m_game.getWorld()->getEntity(m_entity)->getComponent<MovementComponent>())
            coord = m_game.getWorld()->getEntity(m_entity)->getComponent<MovementComponent>()->getNewCoord();
        else
            coord = m_game.getWorld()->getEntity(m_entity)->getCoord();
        m_hitboxF = sf::FloatRect(coord.x + float(m_hitboxI.left) / 64.0F,
            -(coord.y + float(m_size.y - m_hitboxI.top) / 64.0F),
            float(m_hitboxI.width) / 64.0F, float(m_hitboxI.height) / 64.0F);
        return m_hitboxF;
    }
}