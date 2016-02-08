local state = {}
local count = {}

onInput = function(this)
    if(state[this.id] == nil) then
        state[this.id] = math.random(0, 2)
        if(state[this.id] == 1) then
            this:gsSetAnimation("left")
        elseif(state[this.id] == 2) then
            this:gsSetAnimation("right")
        end
    end
    if(count[this.id] == nil) then
        count[this.id] = math.random(50, 1000)
    end
    count[this.id] = count[this.id] - 1
    if(state[this.id] == 0) then
        this.psVelocityX = 0
    elseif(state[this.id] == 1) then
        this.psVelocityX = -1.5
    elseif(state[this.id] == 2) then
        this.psVelocityX = 1.5
    end
    if(count[this.id] == 0) then
        if(state[this.id] == 0) then
            state[this.id] = math.random(0, 2)
        elseif(state[this.id] == 1) then
            state[this.id] = 0
        elseif(state[this.id] == 2) then
            state[this.id] = 0
        end
        count[this.id] = math.random(50, 1000)
        if(state[this.id] == 1) then
            this:gsSetAnimation("left")
        elseif(state[this.id] == 2) then
            this:gsSetAnimation("right")
        end
    end
end