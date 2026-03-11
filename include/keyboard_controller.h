#pragma once
#include "controller.h"

class KeyboardController : public Controller
{
  public:
    auto update(Game &game) -> bool override;
};
