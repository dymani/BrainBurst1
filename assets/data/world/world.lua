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
            PhysicsComponent = {
                isMovable = true,
                type = 2,
                velocities = {0, 0},
                hitbox = {0.3, 0, 0.4, 0.94},
                collide = function(e)
                    return false
                end
            },
            ControlComponent = {
                control = true
            }
        }
    },
    {
        name = "rock1",
        size = {1, 2},
        components = {
            GraphicsComponent = {
                z = 5,
                type = 1,
                animations = {
                    {
                        name = "full",
                        frameStrip = {0, 0, 32, 64},
                        frames = 1,
                        speed = -1
                    },
                    {
                        name = "slight",
                        frameStrip = {32, 0, 32, 64},
                        frames = 1,
                        speed = -1
                    },
                    {
                        name = "more",
                        frameStrip = {64, 0, 32, 64},
                        frames = 1,
                        speed = -1
                    },
                    {
                        name = "broken",
                        frameStrip = {96, 0, 32, 64},
                        frames = 1,
                        speed = -1
                    }
                }
            },
            PhysicsComponent = {
                isMovable = false,
                type = 2,
                hitbox = {0.22, 0, 0.56, 2},
                collide = function(e)
                    e:setDamage(1)
                    return false
                end
            },
            HealthComponent = {
                maxHealth = 7,
                health = 7,
                frames = {"full", "slight", "more", "broken"},
                death = function(e)
                    return true
                end
            }
        }
    },
    {
        name = "rock2",
        size = {1, 1.09},
        components = {
            GraphicsComponent = {
                z = 5,
                type = 0, 
                textureRect = {0, 93, 32, 35}
            },
            PhysicsComponent = {
                isMovable = false,
                type = 2,
                hitbox = {0.28, 0, 0.47, 1},
                collide = function(e)
                    return false
                end
            }
        }
    }
}