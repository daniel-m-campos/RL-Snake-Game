#include "game.h"

#include <iostream>

#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height,
           std::unique_ptr<snake::Snake> snake)
    : _snake(std::move(snake)),
      _engine(_dev()),
      _random_w(0, static_cast<int>(grid_width)),
      _random_h(0, static_cast<int>(grid_height)) {
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, *_snake);
    Update();
    renderer.Render(*_snake, _food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(_score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = _random_w(_engine);
    y = _random_h(_engine);
    // Check that the location is not occupied by a snake item before placing
    // _food.
    if (!_snake->SnakeCell(x, y)) {
      _food.x = x;
      _food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!_snake->IsAlive()) return;

  _snake->Update();

  int new_x = static_cast<int>(_snake->GetHeadX());
  int new_y = static_cast<int>(_snake->GetHeadY());

  // Check if there's _food over here
  if (_food.x == new_x && _food.y == new_y) {
    _score++;
    PlaceFood();
    // Grow snake and increase _speed.
    _snake->GrowBody();
    _snake->SetSpeed(0.02f + _snake->GetSpeed());
  }
}

int Game::GetScore() const { return _score; }

int Game::GetSize() const { return _snake->Size(); }