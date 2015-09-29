#include "BB/Handler/ScriptHandler.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Entity.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Component/GuiComponent.h"

namespace bb {
    ScriptHandler::ScriptHandler(ResourceHandler& resourceHandler):m_resourceHandler(resourceHandler) {
    }

    void ScriptHandler::loadEntities(std::vector<Entity*>& entityList, luabridge::lua_State* L,
        std::string file) {
        for(auto& entity : m_entityList) {
            delete entity.second;
        }
        m_entityList.clear();
        for(auto& entity : entityList) {
            delete entity;
        }
        entityList.clear();

        using namespace luabridge;
        if(luaL_loadfile(L, file.c_str()) || lua_pcall(L, 0, 0, 0)) {
            writeError("loading and reading file");

            return;
        }

        LuaRef luaTemplates = getGlobal(L, "templates");
        if(!luaTemplates.isTable()) {
            writeError("loading templates");
            return;
        }
        if(!loadTemplates(L, luaTemplates)) {
            writeError("reading templates");
            return;
        }

        LuaRef luaEntities = getGlobal(L, "entities");
        if(!luaEntities.isTable()) {
            writeError("loading entities");
            return;
        }
        if(!loadEntities(entityList, L, luaEntities)) {
            writeError("reading entities");
            return;
        }
    }

    bool ScriptHandler::loadTemplates(luabridge::lua_State* L, luabridge::LuaRef& luaTemplates) {
        using namespace luabridge;
        bool error = false;
        Entity* entity;
        for(int i = 1; i <= luaTemplates.length(); i++) {
            entity = new Entity();
            LuaRef luaTemplate = luaTemplates[i];
            if(!luaTemplate.isTable()) {
                writeError("reading templates[" + std::to_string(i) + "]");
                error = true;
                continue;
            }
            LuaRef luaName = luaTemplate["name"];
            LuaRef luaComponents = luaTemplate["components"];
            if(!luaName.isString() || !luaComponents.isTable()) {
                writeError("reading templates[" + std::to_string(i) + "]");
                error = true;
                continue;
            }
            if(!loadComponents(entity, L, luaComponents)) {
                writeError("reading components of templates[" + std::to_string(i) + "]");
                error = true;
                continue;
            }
            m_entityList[luaName.cast<std::string>()] = entity;
        }
        return !error;
    }

    bool ScriptHandler::loadComponents(Entity* entity, luabridge::lua_State* L,
        luabridge::LuaRef& luaComponents) {
        using namespace luabridge;
        bool error = false;

        LuaRef luaGraphicsComponent = luaComponents["GraphicsComponent"];
        if(!luaGraphicsComponent.isNil()) {
            GraphicsComponent* gc = new GraphicsComponent();
            if(gc->createFromLua(L, luaGraphicsComponent, m_resourceHandler))
                entity->addComponent(std::type_index(typeid(GraphicsComponent)), gc);
        }

        LuaRef luaGuiComponent = luaComponents["GuiComponent"];
        if(!luaGuiComponent.isNil()) {
            GuiComponent* gc = new GuiComponent();
            if(gc->createFromLua(L, luaGuiComponent))
                entity->addComponent(std::type_index(typeid(GuiComponent)), gc);
        }
        return !error;
    }

    bool ScriptHandler::loadEntities(std::vector<Entity*>& entityList, luabridge::lua_State* L,
        luabridge::LuaRef& luaEntities) {
        using namespace luabridge;
        bool error = false;
        for(int i = 1; i <= luaEntities.length(); i++) {
            LuaRef luaEntity = luaEntities[i];
            Entity* entity = getEntity(L, luaEntity);
            if(!entity) {
                writeError("reading entities[" + std::to_string(i) + "]");
                error = true;
            } else {
                entityList.push_back(entity);
            }
        }
        return !error;
    }

    Entity* ScriptHandler::getEntity(luabridge::lua_State* L, luabridge::LuaRef& luaEntity) {
        using namespace luabridge;
        LuaRef luaName = luaEntity["name"];
        LuaRef luaX = luaEntity["x"];
        LuaRef luaY = luaEntity["y"];
        if(!luaName.isString()
            || !luaX.isNumber()
            || !luaY.isNumber()) {
            writeError("reading entity");
            return nullptr;
        }
        if(m_entityList.find(luaName.cast<std::string>()) == m_entityList.end()) {
            writeError("loading entity " + luaName.cast<std::string>());
            return nullptr;
        }

        float x = luaX.cast<float>();
        float y = luaY.cast<float>();
        Entity* entity = new Entity(*m_entityList[luaName.cast<std::string>()]);
        if(entity->get<GraphicsComponent>()) {
            if(!entity->get<GraphicsComponent>()->initFromLua(L, luaEntity)) {
                std::cerr << "Error while reading entity.\n";
            }
        }
        if(entity->get<GuiComponent>()) {
            if(!entity->get<GuiComponent>()->initFromLua(L, luaEntity)) {
                std::cerr << "Error while reading entity.\n";
            }
        }
        entity->setCoord({x, y});
        return entity;
    }

    void ScriptHandler::writeError(std::string message) {
        std::cerr << "Error while " + message + " in script handler.\n";
    }

}