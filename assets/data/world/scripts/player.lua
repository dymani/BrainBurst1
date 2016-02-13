local facingLeft = {}
local movingLeft = {}
local state = {}
local coolDown = {}
local c = 50

onInput = function(this, input)
    if(facingLeft[this.id] == nil) then
        facingLeft[this.id] = true
    end
    if(movingLeft[this.id] == nil) then
        movingLeft[this.id] = true
    end
    if(state[this.id] == nil) then
        state[this.id] = 0
    end
    if(coolDown[this.id] == nil) then
        coolDown[this.id] = c
    end
    local update = false
    if(input.mouseX > (this.x + 0.5) and facingLeft[this.id]) then
        update = true
        facingLeft[this.id] = false
    elseif(input.mouseX < (this.x + 0.5) and not(facingLeft[this.id])) then
        update = true
        facingLeft[this.id] = true
    end
    if(coolDown[this.id] >= c) then
        if(input.mouseLeft) then
            coolDown[this.id] = 0
            if(input.mouseX < this.x) then
                this:create("FireL", this.x, this.y + 0.5)
            else
                this:create("FireR", this.x + 1, this.y + 0.5)
            end
        end
    else
        coolDown[this.id] = coolDown[this.id] + 1
    end
    if(state[this.id] == 0) then
        if(input.keyA == not input.keyD) then
            update = true
            if(input.keyA) then
                movingLeft[this.id] = true
                state[this.id] = 1
                this.psVelocityX = -3
            elseif(input.keyD) then
                movingLeft[this.id] = false
                state[this.id] = 1
                this.psVelocityX = 3
            end
        end
        if(input.keyShift or input.keyS) then
            update = true
            state[this.id] = 3
            this.psVelocityX = 0
        elseif(input.keyW or input.keySpace) then
            update = true
            state[this.id] = 2
            this.psVelocityY = 15
            this.psOnGround = false
        end
    elseif(state[this.id] == 1) then
        if(input.keyA == not input.keyD) then
            if(input.keyA) then
                movingLeft[this.id] = true
                this.psVelocityX = -3
            elseif(input.keyD) then
                movingLeft[this.id] = false
                this.psVelocityX = 3
            end
        else
            update = true
            state[this.id] = 0
            this.psVelocityX = 0
        end
        if(input.keyShift or input.keyS) then
            update = true
            state[this.id] = 3
            this.psVelocityX = 0
        elseif(input.keyW or input.keySpace) then
            update = true
            state[this.id] = 2
            this.psVelocityY = 15
            this.psOnGround = false
        end
    elseif(state[this.id] == 2) then
        if(input.keyA == not input.keyD) then
            if(this.psVelocityX == 0) then
                update = true
            end
            if(input.keyA) then
                movingLeft[this.id] = true
                this.psVelocityX = -3
            elseif(input.keyD) then
                movingLeft[this.id] = false
                this.psVelocityX = 3
            end
        else
            if(this.psVelocityX ~= 0) then
                update = true
            end
            this.psVelocityX = 0
        end
        if(this.psOnGround) then
            update = true
            if(this.psVelocityX == 0) then
                state[this.id] = 0
            else
                state[this.id] = 1
            end
        end
    elseif(state[this.id] == 3) then
        if(input.keyA == not input.keyD) then
            if(input.keyA) then
                movingLeft[this.id] = true
                this.psVelocityX = -1.5
            elseif(input.keyD) then
                movingLeft[this.id] = false
                this.psVelocityX = 1.5
            end
        else
            this.psVelocityX = 0
        end
        if(not(input.keyShift or input.keyS)) then
            update = true
            if(this.psVelocityX == 0) then
                state[this.id] = 0
            else
                state[this.id] = 1
            end
        end
    end
    if(update == true) then
        if(facingLeft[this.id]) then
            if(state[this.id] == 0) then
                this:gsSetAnimation("idleL")
            elseif(state[this.id] == 1) then
                this:gsSetAnimation("walkL")
            elseif(state[this.id] == 2) then
                if(this.psVelocityX == 0) then
                    this:gsSetAnimation("idleL")
                else
                    this:gsSetAnimation("walkL")
                end
            elseif(state[this.id] == 3) then
                this:gsSetAnimation("crouchL")
            end
        else
            if(state[this.id] == 0) then
                this:gsSetAnimation("idleR")
            elseif(state[this.id] == 1) then
                this:gsSetAnimation("walkR")
            elseif(state[this.id] == 2) then
                if(this.psVelocityX == 0) then
                    this:gsSetAnimation("idleR")
                else
                    this:gsSetAnimation("walkR")
                end    
            elseif(state[this.id] == 3) then
                this:gsSetAnimation("crouchR")
            end
        end
        if(state[this.id] == 3) then
            this:psSetHitbox(0.3, 0, 0.4, 0.66)
        else
            this:psSetHitbox(0.3, 0, 0.4, 0.94)
        end
    end
    return false
end