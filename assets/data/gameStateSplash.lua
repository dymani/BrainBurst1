templates = {
  {
    name = "Splash0",
    components = {
      GraphicsComponent = {
        {
          name = "default",
          type = 0,
          texture = "splash0",
          scaleX = 0.5333333,
          scaleY = 0.5333333
        }
      },
      UpdateComponent = {
        update = function(this, gs)
          if(gs.state == 0) then
            this.alpha = this.alpha + 3
            if(this.alpha >= 255) then
              this.alpha = 255
              gs.wait = 0
              gs.state = 1
            end
          elseif (gs.state == 1) then
            gs.wait = gs.wait + 1
            if (gs.wait >= 851) then
              gs.state = 2
            end
          else
            this.alpha = this.alpha - 3
            if(this.alpha <= 0) then
              this.alpha = 0
              this:delete()
            end
          end
        end
      }
    }
  },
  {
    name = "Splash1",
    components = {
      GraphicsComponent = {
        {
          name = "default",
          type = 0,
          texture = "splash1",
          scaleX = 0.5333333,
          scaleY = 0.5333333
        }
      }
    }
  }
}
entities = {
  {
    name = "Splash0",
    x = 0,
    y = 0,
    z = 0
  }
}
