#ifndef RLSNAKEGAME_CONTROLLER_H
#define RLSNAKEGAME_CONTROLLER_H

#include "game.h"

class Controller {
 public:
  virtual ~Controller() = default;
  virtual bool Update(Game& game) = 0;
};

#endif  // RLSNAKEGAME_CONTROLLER_H