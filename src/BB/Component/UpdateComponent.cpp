#include "BB/Component/UpdateComponent.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Handler/UpdateHandler.h"
#include "BB/World/Entity.h"
#include "BB/GameState/GameStateSplash.h"

namespace bb {
    UpdateComponent::UpdateComponent(UpdateHandler& updateHandler): m_updateHandler(updateHandler),
        m_updateFunc(nullptr) {
        m_remove = false;
    }

    UpdateComponent::~UpdateComponent() {
        delete m_updateFunc.get();
    }

    UpdateComponent * UpdateComponent::copy() {
        UpdateComponent* uc = new UpdateComponent(m_updateHandler);
        uc->m_updateFunc = m_updateFunc;
        return uc;
    }

    bool UpdateComponent::createFromLua(luabridge::lua_State * L, luabridge::LuaRef & luaUpdateComponent) {
        using namespace luabridge;
        LuaRef func = luaUpdateComponent["update"];
        if(!func.isFunction()) {
            LogHandler::log(LogHandler::ERR, "Incorrect format for update function", typeid(*this).name());
            return false;
        }
        m_updateFunc = std::make_shared<LuaRef>(luaUpdateComponent["update"]);
        return true;
    }

    bool UpdateComponent::initFromLua(luabridge::lua_State * L, luabridge::LuaRef & luaEntity) {
        return true;
    }

    bool UpdateComponent::update(Entity * entity) {
        if(m_remove)
            return true;
        if(m_updateFunc) {
            try {
                LogHandler::log(LogHandler::INF, "U", typeid(*this).name());
                /*(*m_updateFunc)(this, entity->get<GraphicsComponent>(), dynamic_cast<GameStateSplash*>(m_updateHandler.getGameState()));*/
            } catch(luabridge::LuaException const& e) {
                LogHandler::log(LogHandler::ERR, e.what(), typeid(*this).name());
            }
        }
        return false;
    }

    void UpdateComponent::remove() {
        m_remove = true;
    }
}