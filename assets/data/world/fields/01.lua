object = {
    {
        name = "columnA",
        components = {
          GraphicsComponent = {
             size = {64, 128},
             z = 0,
             drawables = {
                 {
                     type = 0,
                     name = "default",
                     textureRect = {0, 0, 16, 32}
                 }
             }
          }
      }
    },
    {
        name = "columnB",
        components = {
          GraphicsComponent = {
              size = {64, 64},
              z = 1,
              drawables = {
                  {
                      type = 0,
                      name = "defalut",
                      textureRect = {0, 32, 16, 16}
                  }
              }
          }
        }
    }
}

entities = {
  {
        name = "rockA",
        components = {
            GraphicsComponent = {
                size = {64, 64},
                z = 5,
                drawables = {
                    {
                        type = 0,
                        name = "base",
                        textureRect = {0, 32, 16, 16}
                    }
                }
            },
            CollisionComponent = {
                type = 0,
                hitbox = {16, 8, 32, 56}
            },
            BreakableComponent = {
                totalDurability = 4,
                durability = 4,
                totalFrames = 4
            }
        }
    },
    {
        name = "rockB",
        components = {
            GraphicsComponent = {
                size = {64, 128},
                z = 5,
                drawables = {
                    {
                        type = 0,
                        name = "base",
                        textureRect = {0, 0, 16, 32}
                    }
                }
            },
            CollisionComponent = {
                type = 0,
                hitbox = {16, 8, 32, 120}
            },
            BreakableComponent = {
                totalDurability = 4,
                durability = 4,
                totalFrames = 4
            }
        }
    }
}