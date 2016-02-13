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
                resistance = true,
                hitbox = {0.3, 0, 0.4, 0.94},
                onHitGround = function(this)
                    return false
                end,
                onCollide = function(this, other)
                    if(this.psVelocityY <= -20) then
                       other:hsSetDamage(-20 - this.psVelocityY)
                    end
                    return false
                end
            },
            ControlComponent = {
                control = true,
                scriptName = "player"
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
                defaultAnimation = "full",
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
                onCollide = function(this, other)
                    return false
                end
            },
            HealthComponent = {
                maxHealth = 7,
                health = 7,
                onHit = function(this)
                    this:hsSetDamage(1)
                    return false
                end,
                onHealthChange = function(this)
                    if(this.hsHealth == 7) then
                        this:gsSetAnimation("full")
                    elseif(this.hsHealth >= 4) then
                        this:gsSetAnimation("slight")
                    elseif(this.hsHealth >= 2) then
                        this:gsSetAnimation("more")
                    else
                        this:gsSetAnimation("broken")
                    end
                    return false
                end,
                onDeath = function(this)
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
                onCollide = function(this, other)
                    return false
                end
            }
        }
    },
    {
        name = "lavaCarbuncle",
        size = {2, 1},
        components = {
            GraphicsComponent = {
                z = 6,
                type = 1,
                texture = "enemies",
                defaultAnimation = "left",
                animations = {
                    {
                        name = "left",
                        frameStrip = {0, 0, 64, 32},
                        frames = 1,
                        speed = -1
                    },
                    {
                        name = "right",
                        frameStrip = {64, 0, 64, 32},
                        frames = 1,
                        speed = -1
                    }
                }
            },
            PhysicsComponent = {
                isMovable = true,
                type = 2,
                velocities = {1, 0},
                resistance = true,
                hitbox = {0.19, 0, 1.38, 0.66},
                onHitGround = function(this)
                    return false
                end,
                onCollide = function(this, other)
                    if(this.y - other.y >-0.1) then
                        if(this.x + 0.19 + 0.69 - 0.5 < other.x) then
                            this.psVelocityX = -3
                        else
                            this.psVelocityX = 3
                        end
                    end
                    return false
                end
            },
            HealthComponent = {
                maxHealth = 10,
                health = 10,
                onHit = function(this)
                    this:hsSetDamage(1)
                    return false
                end,
                onHealthChange = function(this)
                    return false
                end,
                onDeath = function(this)
                    return true
                end
            },
            ControlComponent = {
                control = false,
                scriptName = "lavaCarbuncle"
            }
        }
    },
    {
        name = "FireL",
        size = {0.1, 0.1},
        components = {
            GraphicsComponent = {
                z = 10,
                type = 0, 
                texture = "NULL",
                textureRect = {0, 0, 32, 32}
            },
            PhysicsComponent = {
                isMovable = true,
                type = 2,
                hitbox = {0, 0, 0.1, 0.1},
                velocities = {-5, 0},
                resistance = false,
                onHitGround = function(this)
                    return true
                end,
                onCollide = function(this, other)
                    other:hsSetDamage(1)
                    return true
                end
            }
        }
    },
    {
        name = "FireR",
        size = {0.1, 0.1},
        components = {
            GraphicsComponent = {
                z = 10,
                type = 0, 
                texture = "NULL",
                textureRect = {0, 0, 32, 32}
            },
            PhysicsComponent = {
                isMovable = true,
                type = 2,
                hitbox = {0, 0, 0.1, 0.1},
                velocities = {5, 0},
                resistance = false,
                onHitGround = function(this)
                    return true
                end,
                onCollide = function(this, other)
                    other:hsSetDamage(1)
                    return true
                end
            }
        }
    }
}