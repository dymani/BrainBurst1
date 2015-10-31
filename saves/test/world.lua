player = {
    name = "Silver Crow",
    field = "01",
    coord = {5, 0},
    level = 1,
    bp = 100,
    components = {
        GraphicsComponent = {
            size = {64, 64},
            z = 5,
            drawables = {
                {
                    type = 0,
                    name = "base",
                    texture = "NULL",
                    textureRect = {0, 32, 16, 16}
                --},
                --{
                --    type = 1,
                --    name = "text",
                --    text = "Silver Crow",
                --    font = "default",
                --    size = 10,
                --    color = {255, 255, 255, 255}
                }
            }
        },
        CollisionComponent = {
            hitbox = {0, 0, 16, 16}
        },
        MovementComponent = {
            velocity = {0, 0}
        },
        PlayerComponent = {
        }
    }
}