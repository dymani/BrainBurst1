entities = {
    {
        name = "player",
        size = {1, 1},
        components = {
            GraphicsComponent = {
                z = 0,
                type = 0,
                texture = "player",
                textureRect = {0, 0, 32, 32}
            },
            MovementComponent = {
                velocities = {0, 0}
            },
            PlayerComponent = {
            },
            CollisionComponent = {
                hitbox = {0.25, 0.25, 0.5, 0.5},
                collide = function()
                    return false
                end
            }
        }
    },
    {
        name = "rock",
        size = {1, 1},
        components = {
            GraphicsComponent = {
                z = 5,
                type = 0, 
                textureRect = {0, 0, 32, 32}
            },
            CollisionComponent = {
                hitbox = {0, 0, 1, 1},
                collide = function()
                    return false
                end
            }
        }
    },
    {
        name = "rock1",
        size = {1, 1},
        components = {
            GraphicsComponent = {
                z = 5,
                type = 0, 
                textureRect = {0, 0, 32, 32}
            },
            CollisionComponent = {
                hitbox = {0, 0, 1, 1},
                collide = function()
                    return false
                end
            }
        }
    }
}