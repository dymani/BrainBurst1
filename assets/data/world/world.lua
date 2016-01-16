entities = {
    {
        name = "player",
        size = {1, 1},
        components = {
            GraphicsComponent = {
                z = 0,
                type = 1,
                texture = "player",
                animations = {
                    {
                        name = "idleL",
                        frameStrip = {32, 32, 32, 32},
                        frames = 1,
                        speed = 0
                    },
                    {
                        name = "idleR",
                        frameStrip = {32, 0, 32, 32},
                        frames = 1,
                        speed = 0
                    },
                    {
                        name = "walkL",
                        frameStrip = {0, 32, 128, 32},
                        frames = 4,
                        speed = 10
                    },
                    {
                        name = "walkR",
                        frameStrip = {0, 0, 128, 32},
                        frames = 4,
                        speed = 10
                    },
                    {
                        name = "crouchL",
                        frameStrip = {128, 32, 32, 32},
                        frames = 1,
                        speed = 0
                    },
                    {
                        name = "crouchR",
                        frameStrip = {128, 0, 128, 32},
                        frames = 1,
                        speed = 0
                    }
                }
            },
            MovementComponent = {
                velocities = {0, 0}
            },
            PlayerComponent = {
            },
            CollisionComponent = {
                hitbox = {0.3, 0.06, 0.4, 0.94},
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