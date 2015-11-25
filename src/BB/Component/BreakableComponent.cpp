#include "BB/Component/BreakableComponent.h"

namespace bb {
    BreakableComponent* BreakableComponent::create(GameStateGame& game, luabridge::lua_State* L, luabridge::LuaRef& luaBC) {
        BreakableComponent* bc = new BreakableComponent(game, -1);
        bc->m_totalDurability = luaBC["totalDurability"].cast<int>();
        bc->m_durability = luaBC["durability"].cast<int>();
        bc->m_totalFrames = luaBC["totalFrames"].cast<int>();
        bc->m_frame = bc->getFrame();
        return bc;
    }

    BreakableComponent::BreakableComponent(GameStateGame& game, int entity) : IComponent(game, entity) {
    }

    IComponent* BreakableComponent::copy(rapidjson::Value& value) {
        BreakableComponent* bc;
        if(value.HasMember("id"))
            bc = dynamic_cast<BreakableComponent*>(copy(value["id"].GetInt()));
        else
            bc = dynamic_cast<BreakableComponent*>(copy(-1));
        if(value.HasMember("durability"))
            bc->m_durability = value["durability"].GetInt();
        bc->m_frame = bc->getFrame();
        return bc;
    }

    IComponent* BreakableComponent::copy(int entity) {
        BreakableComponent* bc = new BreakableComponent(m_game, entity);
        bc->m_totalDurability = m_totalDurability;
        bc->m_durability = m_durability;
        bc->m_totalFrames = m_totalFrames;
        bc->m_frame = bc->getFrame();
        return bc;
    }

    void BreakableComponent::addDurability(int durability) {
        m_durability += durability;
        m_frame = getFrame();
    }

    void BreakableComponent::setDurability(int durability) {
        m_durability = durability;
        m_frame = getFrame();
    }

    int BreakableComponent::getDurability() {
        return m_durability;
    }

    void BreakableComponent::setFrame(int frame) {
        m_frame = frame;
    }

    int BreakableComponent::getFrame() {
        m_frame = int(m_totalFrames - float(m_durability) * float(m_totalFrames) / float(m_totalDurability));
        return m_frame;
    }

    bool BreakableComponent::getUpdate() {
        return m_durability <= 0;
    }
}