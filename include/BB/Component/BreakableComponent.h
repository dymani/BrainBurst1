#ifndef BREAKABLE_COMPONENT_H
#define BREAKABLE_COMPONENT_H

#include "BB/Component/IComponent.h"
#include "BB/Handler/LogHandler.h"

namespace bb {
    class BreakableComponent : public IComponent {
    public:
        static BreakableComponent* create(GameStateGame& game, luabridge::lua_State* L,
            luabridge::LuaRef& luaBC);
        BreakableComponent(GameStateGame& game, int entity);
        IComponent* copy(rapidjson::Value& value);
        IComponent* copy(int entity);
        void addDurability(int durability);
        void setDurability(int durability);
        int getDurability();
        void setFrame(int frame);
        int getFrame();
        bool getUpdate();
    private:
        int m_totalDurability, m_durability;
        int m_totalFrames, m_frame;
    };
}

#endif