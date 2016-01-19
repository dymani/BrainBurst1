#ifndef LUA_ENTITY_H
#define LUA_ENTITY_H

namespace bb {
    class GameStateGame;

    class LuaEntity {
    public:
        LuaEntity(GameStateGame& game, int entity);
        void setDamage(int damage);
    private:
        GameStateGame& m_game;
        const int m_entity;
    };
}

#endif