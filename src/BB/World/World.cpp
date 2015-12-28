#include "BB/World/World.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    World::World(GameStateGame& game, std::string name) : m_game(game), m_graphicsSystem(game), m_physicsSystem(game) {
        m_name = name;
        m_field = new Field(m_game, m_name, "01");
        m_field->load();
    }

    void World::handleInput() {
        m_field->handleInput();
    }

    void World::handleInput(sf::Event& windowEvent) {
        m_field->handleInput(windowEvent);
        if(windowEvent.type == sf::Event::KeyPressed) {
            auto coord = m_graphicsSystem.getViewCoord();
            if(windowEvent.key.code == sf::Keyboard::Left)
                m_graphicsSystem.setViewCoord(coord.x - 0.1, coord.y);
            else if(windowEvent.key.code == sf::Keyboard::Right)
                m_graphicsSystem.setViewCoord(coord.x + 0.1, coord.y);
            else if(windowEvent.key.code == sf::Keyboard::Up)
                m_graphicsSystem.setViewCoord(coord.x, coord.y + 0.1);
            else if(windowEvent.key.code == sf::Keyboard::Down)
                m_graphicsSystem.setViewCoord(coord.x, coord.y - 0.1);
        }
    }

    void World::update() {
        m_field->update();
    }

    void World::draw(const double dt) {
        m_field->draw(dt);
    }

    Field * World::getField() {
        return m_field;
    }

    Stage* World::getStage(std::string name) {
        if(m_stages.find(name) != m_stages.end())
            return m_stages[name];
        Stage* stage = Stage::create(m_game.getLuaState(), name);
        if(stage)
            m_stages[name] = stage;
        return stage;
    }
    GraphicsSystem& World::getGraphicsSystem() {
        return m_graphicsSystem;
    }

    PhysicsSystem& World::getPhysicsSystem() {
        return m_physicsSystem;
    }
}