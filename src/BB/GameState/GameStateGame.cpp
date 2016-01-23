#include "BB/GameState/GameStateGame.h"
#include "BB/Game.h"
#include "BB/GameState/GameStateTitle.h"
#include "BB/World/LuaEntity.h"

namespace bb {
    GameStateGame::GameStateGame(Game& game, ResourceHandler* resourceHandler, WindowHandler* windowHandler)
        : IGameState(game),
        m_resourceHandler(std::unique_ptr<ResourceHandler>(resourceHandler)),
        m_windowHandler(std::unique_ptr<WindowHandler>(windowHandler)),
        L(luabridge::luaL_newstate()), m_world(*this, "test") {
        m_audioHandler = std::unique_ptr<AudioHandler>(new AudioHandler(*m_resourceHandler.get()));
        m_state = RUNNING;
        m_windowHandler->getWindow().setKeyRepeatEnabled(true);
        luabridge::getGlobalNamespace(L)
            .beginClass<LuaEntity>("LuaEntity")
            .addProperty("csState", &LuaEntity::csGetState)
            .addProperty("csFacingLeft", &LuaEntity::csIsFacingLeft)
            .addProperty("csMovingLeft", &LuaEntity::csIsMovingLeft)
            .addProperty("x", &LuaEntity::getCoordX, &LuaEntity::setCoordX)
            .addProperty("y", &LuaEntity::getCoordY, &LuaEntity::setCoordY)
            .addFunction("gsSetAnimation", &LuaEntity::gsSetAnimation)
            .addFunction("gsSetAnimationFrame", &LuaEntity::gsSetAnimationFrame)
            .addProperty("hsHealth", &LuaEntity::hsGetHealth)
            .addFunction("hsSetDamage", &LuaEntity::hsSetDamage)
            .addFunction("print", &LuaEntity::print)
            .addProperty("psVelocityX", &LuaEntity::psGetVelocityX, &LuaEntity::psSetVelocityX)
            .addProperty("psVlocityY", &LuaEntity::psGetVelocityY, &LuaEntity::psSetVelocityY)
            .addFunction("psSetHitbox", &LuaEntity::psSetHitbox)
            .endClass();
        m_audioHandler->play("test1");
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
                m_game.changeState(new GameStateTitle(m_game, m_resourceHandler.release(),
                    m_windowHandler.release()));
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
        return m_resourceHandler.get();
    }

    WindowHandler* GameStateGame::getWindowHandler() {
        return m_windowHandler.get();
    }

    luabridge::lua_State* GameStateGame::getLuaState() {
        return L;
    }

    World& GameStateGame::getWorld() {
        return m_world;
    }
}