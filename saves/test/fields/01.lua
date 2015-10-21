type = "HL06"

entities = {
    {
        name = "rock",
        coord = 10,
        components = {
            GraphicsComponent = {
                z = 1,
                drawables = {
                    {
                        type = 0,
                        name = "base",
                        textureRect = {0, 0, 16, 16}
                    },
                    {
                        type = 1,
                        name = "text",
                        text = "Test",
                        font = "default",
                        size = 14,
                        color = {255, 255, 255, 255}
                    },
                    {
                        type = 2,
                        name = "sparks",
                        size = {16, 16},
                        textureRect = {0, 0, 64, 64},
                        speed = 1
                    }
                }
            },
            CollisionComponent = {
                hitbox = {0, 0, 16, 16}
            }
        }
    }
}