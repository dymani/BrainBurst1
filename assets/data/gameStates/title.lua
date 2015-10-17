  templates = {
    {
      name = "Background",
      components = {
        GraphicsComponent = {
          {
            name - "default",
            type = 0,
            texture = "background",
            textureRects = {
              {"default", 0, 0, 1024, 576}
            },
            offsetX = 0,
            offsetY = 0
          }
        }
      }
    },
    {
      name = "Logo",
      components = {
        GraphicsComponent = {
          {
            name = "default"
            type = 0,
            texture = "logo",
            textureRects = {
              {"default", 0, 0, 250, 250}
            },
            offsetX = 0,
            offsetY = 0
          }
        }
      }
    },
    {
      name = "Text",
      components = {
        GraphicsComponent = {
          {
            name = "default",
            type = 1,
            font = "default",
            size = 40,
            align = 0,
            offsetX = 0,
            offsetY = 0
          }
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
              {"focused", 0, 50, 200, 50}
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
            offsetX = 0,
            offsetY = 0
          }
        },
        GuiComponent = {
          type = 1,
          width = 200,
          height = 50
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
            size = "40",
            align = 1,
            offsetX = 0,
            offsetY = 0
          }
        },
        GuiComponent = {
          type = 0,
          width = 200,
          height = 50
        }
      }
    }
  }

  entities = {
    {
      name = "Background",
      x = 0,
      y = 0,
      z = 0
    },
    {
      name = "Logo",
      x = 387,
      y = 250,
      z = 1
    },
    {
      name = "Text",
      x = 100,
      y = 415,
      z = 1,
      text = "Name:"
    },
    {
      name = "Textbox",
      x = 200,
      y = 400,
      z = 1,
      id = 0,
      characters = {
        "alphabet", "number", "space"
      }
    },
    {
      name = "Button",
      x = 262,
      y = 500,
      z = 1,
      id = 1,
      text = "Start"
    },
    {
      name = "Button",
      x = 762,
      y = 500,
      z = 1,
      id = 2,
      text = "Quit"
    }
  }
