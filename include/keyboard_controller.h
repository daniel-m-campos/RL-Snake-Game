#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "controller.h"

class KeyboardController : public Controller {
 public:
  bool Update(Game& game) override;

 private:
  static void ChangeDirection(snake::Snake &snake, snake::Direction input,
                              snake::Direction opposite);
};

#endif