#include "BB/World/Field.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/World/Component/GraphicsComponent.h"

namespace bb {
    Field::Field(GameStateGame& game, std::string worldName, std::string id) : m_game(game) {
        for(auto& system : m_game.getWorld().getSystems()) {
            system.second->createList(m_componentLists);
        }
        m_worldName = worldName;
        m_id = id;
    }

    void Field::load() {
        std::string file = "saves/" + m_worldName + "/fields/" + m_id + ".json";
        std::ifstream fin(file);
        if(fin.fail()) {
            LogHandler::log<Field>(ERR, "Field \"" + file + "\" not found");
            return;
        }
        std::stringstream strStream;
        strStream << fin.rdbuf();
        std::string fileString = strStream.str();
        fin.close();
        using namespace rapidjson;
        Document document;
        if(document.Parse<0>(fileString.c_str()).HasParseError()) {
            LogHandler::log<Field>(ERR, "Failed to parse \"" + file + "\"");
            return;
        }
        Value& jsonEntities = document["entities"];
        for(SizeType i = 0; i < jsonEntities.Size(); i++) {
            Value& jsonEntity = jsonEntities[i];
            Value& jsonName = jsonEntity["name"];
            Entity* entity = m_game.getWorld().getEntityTemplate(jsonName.GetString())->createEntity(m_game,
                jsonEntity);
            m_entities[entity->getId()] = std::unique_ptr<Entity>(entity);
        }
        m_stage = document["tileSet"].GetString();
        m_tileSet = m_game.getWorld().getStage(m_stage)->getTileSet();
        Value& jsonTiles = document["tiles"];
        for(SizeType i = 0; i < jsonTiles.Size(); i++) {
            m_tiles.push_back(jsonTiles[i].GetInt());
        }
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(int(m_tiles.size()) * 4);
        int textureSize = m_game.getWorld().getSystem<GraphicsSystem>().getTextureSize();
        float tileSize = m_game.getWorld().getSystem<GraphicsSystem>().getTileSize();
        int h = m_game.getWindowHandler()->getWindow().getSize().y;
        for(int i = 0; i < int(m_tiles.size()); i++) {
            sf::Vertex* quad = &m_vertices[i * 4];
            quad[0].position = {float(i * tileSize), float(h)};
            quad[1].position = {float((i + 1) * tileSize), float(h)};
            quad[2].position = {float((i + 1) * tileSize), float(h + tileSize)};
            quad[3].position = {float(i * tileSize), float(h + tileSize)};
            quad[0].texCoords = {float(m_tiles[i] * textureSize), float(0)};
            quad[1].texCoords = {float(m_tiles[i] + 1) * textureSize, float(0)};
            quad[2].texCoords = {float(m_tiles[i] + 1) * textureSize, float(textureSize)};
            quad[3].texCoords = {float(m_tiles[i] * textureSize), float(textureSize)};
        }
        m_background.setTexture(m_game.getResourceHandler()->getTexture(m_game.getWorld()
            .getStage(document["tileSet"].GetString())->getBackgroundTexture()));
        sf::Texture& tex = m_game.getResourceHandler()->getTexture(m_tileSet);
        tex.setSmooth(false);
        m_states.texture = &tex;
        sf::View view = m_game.getWindowHandler()->getWindow().getDefaultView();
        m_background.setScale(view.getSize().x / m_background.getTexture()->getSize().x,
            view.getSize().x / m_background.getTexture()->getSize().x);
        int player = m_game.getWorld().getSystem<ControlSystem>().getPlayerId();
        m_game.getWorld().getSystem<GraphicsSystem>().setViewCoord(m_entities[player]->getCoord().x, -1);
    }

    void Field::update() {
    }

    void Field::draw(const double dt) {
        auto& gs = m_game.getWorld().getSystem<GraphicsSystem>();
        int player = m_game.getWorld().getSystem<ControlSystem>().getPlayerId();
        sf::Vector2f playerCoord = m_entities[player]->getCoord();
        if(gs.getViewCoord().x > playerCoord.x + 3) {
            gs.setViewCoord(playerCoord.x + 3, gs.getViewCoord().y);
        } else if(gs.getViewCoord().x < playerCoord.x - 3) {
            gs.setViewCoord(playerCoord.x - 3, gs.getViewCoord().y);
        }
        if(gs.getViewCoord().y < playerCoord.y - 5) {
            gs.setViewCoord(gs.getViewCoord().x, playerCoord.y - 5);
        } else if(gs.getViewCoord().y > playerCoord.y - 5) {
            gs.setViewCoord(gs.getViewCoord().x, playerCoord.y - 5);
        }
        sf::View view = gs.getView();
        auto& window = m_game.getWindowHandler()->getWindow();
        window.setView(window.getDefaultView());
        window.draw(m_background);
        window.setView(view);
        window.draw(m_vertices, m_states);
        gs.draw(dt);
    }

    int Field::getSize() {
        return int(m_tiles.size());
    }

    Entity* Field::getEntity(int id) {
        return m_entities[id].get();
    }

    int Field::createEntity(std::string name, sf::Vector2f coord) {
        int id = rand() % 1000 + 1;
        auto* entity = m_game.getWorld().getEntityTemplate(name)->createEntity(m_game, id, coord);
        m_entities[entity->getId()] = std::unique_ptr<Entity>(entity);
        return id;
    }

    void Field::addDeleteEntity(int id) {
        m_deletingEntities.push_back(id);
    }

    void Field::deleteEntities() {
        for(int id : m_deletingEntities) {
            m_entities.erase(id);
        }
        m_deletingEntities.clear();
    }

    void Field::deleteComponents() {
        for(auto& c : m_deletingComponents) {
            m_entities[c.second]->removeComponent(c.first);
        }
        m_deletingComponents.clear();
    }

    CList* Field::getComponentList(std::type_index type){
        if(m_componentLists.empty()) return nullptr;
        if(m_componentLists.find(type) == m_componentLists.end()) return nullptr;
        return m_componentLists[type].get();
    }

    std::string Field::getStageName() {
        return m_stage;
    }
}