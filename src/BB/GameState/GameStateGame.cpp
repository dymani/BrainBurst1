#include "BB/GameState/GameStateGame.h"
#include "BB/Game.h"
#include "BB/GameState/GameStateTitle.h"
#include "BB/World/LuaEntity.h"

namespace bb {
    GameStateGame::GameStateGame(Game& game, ResourceHandler* resourceHandler, WindowHandler* windowHandler,
        luabridge::lua_State* L) : IGameState(game), m_windowHandler(windowHandler),
        m_resourceHandler(resourceHandler), L(L), m_world(*this, "test") {
        m_state = RUNNING;
        m_windowHandler->getWindow().setKeyRepeatEnabled(true);
        luabridge::getGlobalNamespace(L)
            .beginClass<LuaEntity>("LuaEntity")
            .addFunction("setDamage", &LuaEntity::setDamage)
            .endClass();
    }

    void GameStateGame::handleInput() {
        sf::Event windowEvent;
        m_world.handleInput();
        while(m_windowHandler->getWindow().pollEvent(windowEvent)) {
            m_world.handleInput(windowEvent);
            if(windowEvent.type == sf::Event::Closed) {
                m_state = QUIT;
                return;
            } else if(windowEvent.type == sf::Event::KeyPressed) {
                switch(windowEvent.key.code) {
                    case sf::Keyboard::Escape:
                        m_state = TITLE;
                        break;
                }
            }
        }
    }

    bool GameStateGame::update() {
        m_world.update();
        switch(m_state) {
            case RUNNING:
                break;
            case TITLE:
                m_game.changeState(new GameStateTitle(m_game, m_resourceHandler, m_windowHandler, L));
                break;
            case QUIT:
                m_windowHandler->getWindow().close();
                return false;
        }
        return true;
    }

    void GameStateGame::draw(const double dt) {
        m_windowHandler->getWindow().clear();
        m_world.draw(dt);
        m_windowHandler->getWindow().display();
    }

    ResourceHandler* GameStateGame::getResourceHandler() {
        return m_resourceHandler;
    }

    WindowHandler* GameStateGame::getWindowHandler() {
        return m_windowHandler;
    }

    luabridge::lua_State* GameStateGame::getLuaState() {
        return L;
    }

    World& GameStateGame::getWorld() {
        return m_world;
    }
}