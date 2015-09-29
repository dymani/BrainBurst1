templates = {
  {
    name = "Sprite",
    components = {
      GraphicsComponent = {
        {
          name = "default",
          type = 0,
          texture = "test1",
          textureRects = {
            {"default", 0, 0, 100, 100}
          },
          offsetX = 0,
          offsetY = 0
        }
      }
    }
  },
  {
    name = "Button",
    components = {
      GraphicsComponent = {
        {
          name = "sprite",
          type = 0,
          texture = "gui",
          textureRects = {
            {"default", 0, 0, 200, 50},
            {"hover", 0, 50, 200, 50},
            {"pressed", 0, 100, 200, 50}
          },
          offsetX = 0,
          offsetY = 0
        },
        {
          name = "text",
          type = 1,
          font = "default",
          size = 40,
          align = 1,
          offsetX = 100,
          offsetY = 10
        }
      },
      GuiComponent = {
        type = 0,
        width = 200,
        height = 50
      }
    }
  },
  {
    name = "Textbox",
    components = {
      GraphicsComponent = {
        {
          name = "sprite",
          type = 0,
          texture = "gui",
          textureRects = {
            {"default", 0, 0, 200, 50},
            {"hover", 0, 50, 200, 50},
            {"focused", 0, 100, 200, 50}
          },
          offsetX = 0,
          offsetY = 0
        },
        {
          name = "text",
          type = 1,
          font = "default",
          size = 40,
          align = 0,
          offsetX = 10,
          offsetY = 0
        }
      },
      GuiComponent = {
        type = 1,
        width = 200,
        height = 50
      }
    }
  }
}

entities = {
  {
    name = "Sprite",
    x = 206,
    y = 238,
    z = 0
  },
  {
    name = "Sprite",
    x = 462,
    y = 238,
    z = 0
  },
  {
    name = "Sprite",
    x = 718,
    y = 238,
    z = 0
  },

  {
    name = "Button",
    x = 412,
    y = 300,
    z = 0,
    id = 0,
    text = "Start"
  },
  {
    name = "Button",
    x = 412,
    y = 400,
    z = 0,
    id = 1,
    text = "Options"
  },
  {
    name = "Button",
    x = 412,
    y = 500,
    z = 0,
    id = 2,
    text = "Quit"
  },
  {
    name = "Textbox",
    x = 200,
    y = 200,
    z = 0,
    id = 3,
    text = "",
    characters = {
      "alphabet", "number", "space"
    }
  }
}
