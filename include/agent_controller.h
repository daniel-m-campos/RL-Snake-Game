#pragma once

#include <memory>

#include "agent.h"
#include "controller.h"
#include "game.h"
#include "game_environment.h"

class AgentController : public Controller
{
  public:
    explicit AgentController(std::string const &filename);
    auto update(Game &game) -> bool override;

  private:
    std::unique_ptr<Agent<GameState, snake::Direction>> _agent;
};
