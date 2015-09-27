templates = {
  {
    name = "Test1",
    components = {
      GraphicsComponent = {
        type = 0,
        texture = "test1",
        textureRects = {
          {"default", 0, 0, 100, 100}
        },
        z = 0
      }
    }
  },
  {
    name = "Test2",
    components = {
      GraphicsComponent = {
        type = 0,
        texture = "test2",
        textureRects = {
          {"default", 0, 0, 100, 100}
        },
        z = 1
      }
    }
  },
  {
    name = "Test3",
    components = {
      GraphicsComponent = {
        type = 0,
        texture = "test3",
        textureRects = {
          {"default", 0, 0, 100, 100}
        },
        z = 2
      }
    }
  },
  {
    name = "Button1",
    components = {
      GraphicsComponent = {
        type = 0,
        texture = "gui",
        textureRects = {
          {"default", 0, 0, 200, 50},
          {"hover", 0, 50, 200, 50},
          {"pressed", 0, 100, 200, 50}
        },
        z = 10
      },
      GuiComponent = {
        type = 0,
        width = 200,
        height = 50
      }
    }
  },
  {
    name = "Button2",
    components = {
      GraphicsComponent = {
        type = 0,
        texture = "gui",
        textureRects = {
          {"default", 0, 0, 50, 50},
          {"hover", 0, 50, 50, 50},
          {"pressed", 0, 100, 50, 50}
        },
        z = 10
      },
      GuiComponent = {
        type = 0,
        width = 50,
        height = 50
      }
    }
  },
  {
    name = "Text",
    components = {
      GraphicsComponent = {
        type = 1,
        font = "default",
        size = 40,
        z = 11
      }
    }
  }
}

entities = {
  {
    name = "Test1",
    x = 206,
    y = 238
  },
  {
    name = "Test2",
    x = 462,
    y = 238
  },
  {
    name = "Test3",
    x = 718,
    y = 238
  },
  {
    name = "Button1",
    x = 412,
    y = 300,
    id = 0
  },
  {
    name = "Text",
    x = 512,
    y = 315,
    text = "Start"
  },
  {
    name = "Button1",
    x = 412,
    y = 400,
    id = 1
  },
  {
    name = "Text",
    x = 512,
    y = 415,
    text = "Options"
  },
  {
    name = "Button1",
    x = 412,
    y = 500,
    id = 2
  },
  {
    name = "Text",
    x = 512,
    y = 515,
    text = "Quit"
  }
}
