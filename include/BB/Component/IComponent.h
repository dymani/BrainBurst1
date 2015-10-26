#ifndef I_COMPONENT_H
#define I_COMPONENT_H

namespace bb {
    class GameStateGame;

    class IComponent {
    public:
        virtual ~IComponent() {
        };
        virtual IComponent* copy(int entity) = 0;
    protected:
        int const m_entity;
        GameStateGame& m_game;
        IComponent(GameStateGame& game, int entity):m_game(game), m_entity(entity) {
        }
    };
}

#endif