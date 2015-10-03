#ifndef I_GAME_STATE_H
#define I_GAME_STATE_H

#include "BB/Game.h"
#include <SFML/Graphics.hpp>

namespace bb {
    class Entity;

    class IGameState {
    public:
        IGameState(Game& game):m_game(game) {
        }
        virtual bool update() = 0;
        virtual void draw(const double dt) = 0;
        virtual void handleInput() = 0;
    protected:
        Game& m_game;
        std::vector<Entity*> m_entities;
    };
}

#endif