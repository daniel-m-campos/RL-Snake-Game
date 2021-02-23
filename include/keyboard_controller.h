#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "controller.h"

class KeyboardController : public Controller {
 public:
  bool Update(Game& game) override;
};

#endif