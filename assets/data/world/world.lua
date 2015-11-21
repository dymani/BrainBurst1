player = {
    components = {
        GraphicsComponent = {
            size = {64, 64},
            z = 5,
            drawables = {
                {
                    type = 0,
                    name = "base",
                    texture = "player",
                    textureRect = {0, 0, 16, 16}
                }
            }
        },
        CollisionComponent = {
            type = 1,
            hitbox = {4, 0, 56, 64}
        },
        MovementComponent = {
            velocity = {0, 0}
        },
        PlayerComponent = {
        }
    }
}