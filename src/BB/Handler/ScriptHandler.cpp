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
            std::cerr << "Error while loading " + file + ".\n";
            return;
        }
        LuaRef luaTemplates = getGlobal(L, "templates");
        if(!luaTemplates.isTable()) {
            std::cerr << "Error while getting \"templates\" in " + file + ".\n";
            return;
        }
        for(int i = 1; i <= luaTemplates.length(); i++) {
            Entity* entity = new Entity();

            LuaRef luaTemplate = luaTemplates[i];
            LuaRef luaName = luaTemplate["name"];
            LuaRef luaComponents = luaTemplate["components"];
            if(!luaName.isString() || !luaComponents.isTable()) {
                std::cerr << "Error while getting \"template[" + std::to_string(i) + "]\" in " + file
                    + ".\n";
                continue;
            }

            LuaRef luaGraphicsComponent = luaComponents["GraphicsComponent"];
            if(!luaGraphicsComponent.isNil()) {
                LuaRef luaType = luaGraphicsComponent["type"];
                if(!luaType.isNumber()) {
                    std::cerr << "Error while getting \"GraphicsComponent\" of \"define[" + std::to_string(i)
                        + "]\" in " + file + ".\n";
                    continue;
                }
                GraphicsComponent* gc = new GraphicsComponent();
                int type = luaType.cast<int>();
                if(type == gc->SPRITE) {
                    LuaRef luaTexture = luaGraphicsComponent["texture"];
                    LuaRef luaTextureRects = luaGraphicsComponent["textureRects"];
                    if(!luaGraphicsComponent.isTable()
                        || !luaTexture.isString()) {
                        std::cerr << "Error while getting \"GraphicsComponent\" of \"define["
                            + std::to_string(i) + "]\" in " + file + ".\n";
                        continue;
                    }
                    std::string texture = luaTexture.cast<std::string>();
                    gc->setTexture(m_resourceHandler.getTexture(texture));
                    if(luaTextureRects.isTable()) {
                        for(int i = 1; i <= luaTextureRects.length(); i++) {
                            LuaRef luaTextureRect = luaTextureRects[i];
                            if(!luaTextureRect.isTable()) {
                                std::cerr << "Error while getting \"GraphicsComponent\" of \"define["
                                    + std::to_string(i) + "]\" in " + file + ".\n";
                                continue;
                            }
                            gc->addTextureRect(luaTextureRect[1].cast<std::string>(), {
                                luaTextureRect[2].cast<int>(), luaTextureRect[3].cast<int>(),
                                luaTextureRect[4].cast<int>(), luaTextureRect[5].cast<int>()});
                        }
                        gc->setTextureRect("default");
                    }
                } else if(type == gc->TEXT) {
                    LuaRef luaFont = luaGraphicsComponent["font"];
                    LuaRef luaSize = luaGraphicsComponent["size"];
                    if(!luaGraphicsComponent.isTable()
                        || !luaFont.isString()
                        || !luaSize.isNumber()) {
                        std::cerr << "Error while getting \"GraphicsComponent\" of \"define["
                            + std::to_string(i) + "]\" in " + file + ".\n";
                        continue;
                    }
                    std::string font = luaFont.cast<std::string>();
                    unsigned int size = luaSize.cast<unsigned int>();
                    gc->setFont(m_resourceHandler.getFont(font), size);
                } else {
                    std::cerr << "Error while getting \"GraphicsComponent\" of \"define[" + std::to_string(i)
                        + "]\" in " + file + ".\n";
                    continue;
                }
                LuaRef luaZ = luaGraphicsComponent["z"];
                if(!luaZ.isNumber()) {
                    std::cerr << "Error while getting \"GraphicsComponent\" of \"define[" + std::to_string(i)
                        + "]\" in " + file + ".\n";
                    continue;
                }
                float z = luaZ.cast<float>();
                gc->setType(type)->setZ(z);
                entity->addComponent(std::type_index(typeid(GraphicsComponent)), gc);
            }

            LuaRef luaGuiComponent = luaComponents["GuiComponent"];
            if(!luaGuiComponent.isNil()) {
                LuaRef luaType = luaGuiComponent["type"];
                LuaRef luaWidth = luaGuiComponent["width"];
                LuaRef luaHeight = luaGuiComponent["height"];
                if(!luaType.isNumber()
                    || !luaWidth.isNumber()
                    || !luaHeight.isNumber()) {
                    std::cerr << "Error while getting \"GuiComponent\" of \"define[" + std::to_string(i)
                        + "]\" in " + file + ".\n";
                    continue;
                }
                GuiComponent* gc = new GuiComponent();
                int type = luaType.cast<int>();
                gc->setType(type);
                if(gc->getType() == gc->BUTTON) {
                    int width = luaWidth.cast<int>();
                    int height = luaHeight.cast<int>();
                    gc->setSize({width, height});
                }
                entity->addComponent(std::type_index(typeid(GuiComponent)), gc);
            }

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
                continue;
            }
            std::string name = luaName.cast<std::string>();
            float x = luaX.cast<float>();
            float y = luaY.cast<float>();
            if(m_entityList.find(name) == m_entityList.end()) {
                std::cerr << "Error while getting \"entities[" + std::to_string(i) + "]\" in " + file
                    + ".\n";
                continue;
            }
            Entity* entity = new Entity(*m_entityList[name]);
            if(entity->get<GraphicsComponent>()->getType() == 1) {
                LuaRef luaText = luaEntity["text"];
                std::string text = "";
                if(luaText.isString()) {
                    text = luaText.cast<std::string>();
                }
                entity->get<GraphicsComponent>()->setText(text);
            }
            if(entity->get<GuiComponent>()) {
                if(!luaEntity["id"].isNumber()) {
                    std::cerr << "Error while getting \"entities[" + std::to_string(i) + "]\" in " + file
                        + ".\n";
                    continue;
                }
                entity->get<GuiComponent>()->setId(luaEntity["id"].cast<int>());
            }
            entity->setCoord({x, y});
            entityList.push_back(entity);
        }
    }
}