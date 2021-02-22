#ifndef RLSNAKEGAME_AGENT_CONTROLLER_H
#define RLSNAKEGAME_AGENT_CONTROLLER_H

#include <memory>

#include "agent.h"
#include "controller.h"
#include "game.h"
#include "game_environment.h"

class AgentController : public Controller {
 public:
  AgentController();
  bool Update(Game& game) override;

 private:
  static const std::string _filename;
  std::unique_ptr<Agent<GameState, snake::Direction>> _agent;
};

#endif  // RLSNAKEGAME_AGENT_CONTROLLER_H
