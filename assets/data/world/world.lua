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
            MovementComponent = {
                velocities = {0, 0}
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
            }
        }
    }
}