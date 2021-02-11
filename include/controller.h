#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

class Controller {
 public:
  void HandleInput(bool &running, snake::Snake &snake) const;

 private:
  void ChangeDirection(snake::Snake &snake, snake::Direction input,
                       snake::Direction opposite) const;
};

#endif