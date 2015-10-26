type = "HL06"

entities = {
    {
        name = "rock",
        coord = {3, 8},
        components = {
            GraphicsComponent = {
                size = {64, 64},
                z = 5,
                drawables = {
                    --{
                    --    type = 0,
                    --    name = "base",
                    --    textureRect = {0, 32, 16, 16}
                    --},
                    {
                        type = 1,
                        name = "text",
                        text = "Test",
                        font = "default",
                        size = 64,
                        color = {255, 255, 255, 255}
                    --},
                    --{
                    --    type = 2,
                    --    name = "sparks",
                    --    size = {16, 16},
                    --    textureRect = {0, 16, 16, 16},
                    --    speed = 1
                    }
                }
            --},
            --CollisionComponent = {
            --    hitbox = {0, 0, 16, 16}
            --},
            --MovementComponent = {
            --    velocity = {0, 0}
            --},
            --PlayerComponent = {
--
            }
        }
    }
}