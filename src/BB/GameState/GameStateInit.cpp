#include "BB/GameState/GameStateInit.h"
#include "BB/Game.h"
#include "BB/GameState/GameStateSplash.h"

#if defined (SFML_SYSTEM_WINDOWS)
#include <windows.h>

bool setShape(HWND hWnd, const sf::Image& image) {
    const sf::Uint8* pixelData = image.getPixelsPtr();
    HRGN hRegion = CreateRectRgn(0, 0, image.getSize().x, image.getSize().y);
    for(unsigned int y = 0; y < image.getSize().y; y++) {
        for(unsigned int x = 0; x < image.getSize().x; x++) {
            if(pixelData[y * image.getSize().x * 4 + x * 4 + 3] == 0) {
                HRGN hRegionDest = CreateRectRgn(0, 0, 1, 1);
                HRGN hRegionPixel = CreateRectRgn(x, y, x + 1, y + 1);
                CombineRgn(hRegionDest, hRegion, hRegionPixel, RGN_XOR);
                DeleteObject(hRegion);
                DeleteObject(hRegionPixel);
                hRegion = hRegionDest;
            }
        }
    }
    SetWindowRgn(hWnd, hRegion, true);
    DeleteObject(hRegion);
    return true;
}

bool setTransparency(HWND hWnd, unsigned char alpha) {
    SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
    return true;
}
#endif

namespace bb {
    GameStateInit::GameStateInit(Game& game):IGameState(game) {
        m_windowHandler = new WindowHandler();
        m_resourceHandler = new ResourceHandler(m_game);
        m_state = RUNNING;
        using namespace luabridge;
        L = luaL_newstate();
        luaL_openlibs(L);
        m_resourceHandler->load(L);
        using namespace luabridge;
        if(luaL_loadfile(L, "assets/data/gameStates/init.lua") || lua_pcall(L, 0, 0, 0)) {
            LogHandler::log(LogHandler::ERR, "File \"assets/data/gameStates/init.lua\" not found",
                typeid(*this).name());
            return;
        }
        LuaRef luaTexture = getGlobal(L, "texture");
        if(!luaTexture.isString()) {
            LogHandler::log(LogHandler::ERR, "\"texture\" not a string in init.lua.",
                typeid(*this).name());
            return;
        }
        m_sprite.setTexture(m_resourceHandler->getTexture(luaTexture.cast<std::string>()));
        sf::Image init = m_resourceHandler->getTexture(luaTexture.cast<std::string>()).copyToImage();
        m_windowHandler->createWindow(sf::VideoMode(init.getSize().x, init.getSize().y, 32),
            "Brain Burst 2039", sf::Style::None, sf::ContextSettings());
        m_windowHandler->getWindow().setPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width - init.getSize().x) / 2, (sf::VideoMode::getDesktopMode().height - init.getSize().y) / 2));
        setShape(m_windowHandler->getWindow().getSystemHandle(), init);
        setTransparency(m_windowHandler->getWindow().getSystemHandle(), 200);
        m_updateCount = 0;
    }

    GameStateInit::~GameStateInit() {
    }

    void GameStateInit::handleInput() {
        sf::Event windowEvent;
        while(m_windowHandler->getWindow().pollEvent(windowEvent)) {
            if(windowEvent.type == sf::Event::KeyPressed) {
                switch(windowEvent.key.code) {
                    case sf::Keyboard::Escape:
                        m_state = QUIT;
                        return;
                }
            }
        }
    }

    bool GameStateInit::update() {
        m_updateCount++;
        switch(m_state) {
            case RUNNING:
                if(m_resourceHandler->load()) {
                    LogHandler::log(LogHandler::INF, "Finished loading resources ", typeid(*this).name());
                    m_state = NEXT;
                }
                break;
            case QUIT:
                m_windowHandler->getWindow().close();
                return false;
            case NEXT:
                if(m_updateCount >= 50) {
                    m_windowHandler->getWindow().close();
                    m_game.changeState(new GameStateSplash(m_game, m_resourceHandler, L));
                }
                break;
        }
        return true;
    }

    void GameStateInit::draw(const double dt) {
        m_windowHandler->getWindow().clear(sf::Color::Transparent);
        m_windowHandler->getWindow().draw(m_sprite);
        m_windowHandler->getWindow().display();
    }
}