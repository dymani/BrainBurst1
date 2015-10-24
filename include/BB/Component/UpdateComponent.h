#ifndef UPDATE_COMPONENT_H
#define UPDATE_COMPONENT_H

#include <memory>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include "BB/Component/IComponent.h"
#include "BB/Handler/LogHandler.h"

namespace bb {
    class UpdateHandler;
    class Entity;

    class UpdateComponent: public IComponent {
    public:
        UpdateComponent(UpdateHandler& updateHandler);
        ~UpdateComponent();
        UpdateComponent* copy(Entity& entity);
        bool createFromLua(luabridge::lua_State* L, luabridge::LuaRef& luaUpdateComponent);
        bool initFromLua(luabridge::lua_State* L, luabridge::LuaRef& luaEntity);
        bool update(Entity* entity);
        void remove();
    private:
        UpdateHandler& m_updateHandler;
        std::shared_ptr<luabridge::LuaRef> m_updateFunc;
        bool m_remove = false;
    };
}

#endif