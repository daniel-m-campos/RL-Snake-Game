#pragma once

#include "game.h"

class Controller
{
  public:
    virtual ~Controller()                              = default;
    Controller()                                       = default;
    Controller(Controller const &)                     = default;
    auto operator=(Controller const &) -> Controller & = default;
    Controller(Controller &&)                          = default;
    auto operator=(Controller &&) -> Controller &      = default;
    virtual auto update(Game &game) -> bool            = 0;
};
