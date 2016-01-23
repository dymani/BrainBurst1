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
                onHitGround = function(this)
                    return false
                end,
                onCollide = function(this, other)
                    return false
                end
            },
            ControlComponent = {
                control = true,
                onInput = function(this)
                    if(this.csFacingLeft) then
                        if(this.csState == 0) then
                            this:gsSetAnimation("idleL")
                        elseif(this.csState == 1) then
                            this:gsSetAnimation("walkL")
                        elseif(this.csState == 2) then
                            if(this.psVelocityX == 0) then
                                this:gsSetAnimation("idleL")
                            else
                                this:gsSetAnimation("walkL")
                            end
                        elseif(this.csState == 3) then
                            this:gsSetAnimation("crouchL")
                        end
                    else
                        if(this.csState == 0) then
                            this:gsSetAnimation("idleR")
                        elseif(this.csState == 1) then
                            this:gsSetAnimation("walkR")
                        elseif(this.csState == 2) then
                            if(this.psVelocityX == 0) then
                                this:gsSetAnimation("idleR")
                            else
                                this:gsSetAnimation("walkR")
                            end    
                        elseif(this.csState == 3) then
                            this:gsSetAnimation("crouchR")
                        end
                    end
                    if(this.csState == 3) then
                        this:psSetHitbox(0.3, 0, 0.4, 0.66)
                    else
                        this:psSetHitbox(0.3, 0, 0.4, 0.94)
                    end
                    return false
                end
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
                    --this:hsSetDamage(1)
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
    }
}