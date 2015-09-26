#include "BB/Handler/ScriptHandler.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Entity.h"
#include "BB/Component/GraphicsComponent.h"

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
            std::cerr << "Error while loading " + file + ".\n";
            return;
        }
        LuaRef luaTemplates = getGlobal(L, "templates");
        if(!luaTemplates.isTable()) {
            std::cerr << "Error while getting \"templates\" in " + file + ".\n";
            return;
        }
        for(int i = 1; i <= luaTemplates.length(); i++) {
            LuaRef luaTemplate = luaTemplates[i];
            LuaRef luaName = luaTemplate["name"];
            LuaRef luaComponents = luaTemplate["components"];
            if(!luaName.isString() || !luaComponents.isTable()) {
                std::cerr << "Error while getting \"template[" + std::to_string(i) + "]\" in " + file
                    + ".\n";
                return;
            }
            LuaRef luaGraphicsComponent = luaComponents["GraphicsComponent"];
            LuaRef luaTexture = luaGraphicsComponent["texture"];
            LuaRef luaZ = luaGraphicsComponent["z"];
            if(!luaGraphicsComponent.isTable()
                || !luaTexture.isString()
                || !luaZ.isNumber()) {
                std::cerr << "Error while getting \"GraphicsComponent\" of \"define[" + std::to_string(i)
                    + "]\" in " + file + ".\n";
                return;
            }
            Entity* entity = new Entity();
            GraphicsComponent* gc = new GraphicsComponent();
            std::string texture = luaTexture.cast<std::string>();
            float z = luaZ.cast<float>();
            gc->setTexture(m_resourceHandler.getTexture(texture))->setZ(z);
            entity->addComponent(std::type_index(typeid(GraphicsComponent)), gc);
            m_entityList[luaName.cast<std::string>()] = entity;
        }
        LuaRef luaEntities = getGlobal(L, "entities");
        if(!luaEntities.isTable()) {
            std::cerr << "Error while getting \"entities\" in " + file + ".\n";
            return;
        }
        for(int i = 1; i <= luaEntities.length(); i++) {
            LuaRef luaEntity = luaEntities[i];
            LuaRef luaName = luaEntity["name"];
            LuaRef luaX = luaEntity["x"];
            LuaRef luaY = luaEntity["y"];
            if(!luaName.isString()
                || !luaX.isNumber()
                || !luaY.isNumber()) {
                std::cerr << "Error while getting \"entities[" + std::to_string(i) + "]\" in " + file
                    + ".\n";
                return;
            }
            std::string name = luaName.cast<std::string>();
            float x = luaX.cast<float>();
            float y = luaY.cast<float>();
            Entity* entity = new Entity(*m_entityList[name]);
            entity->setCoord({x, y});
            entityList.push_back(entity);
        }
    }
}