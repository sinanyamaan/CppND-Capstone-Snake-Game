#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"

class Snake
{
public:
  enum class Direction
  {
    kUp,
    kDown,
    kLeft,
    kRight
  };

  Snake(int grid_width = 32, int grid_height = 32)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  void Update();

  void GrowBody(int count = 1);
  bool SnakeCell(int x, int y);

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

protected:
  void UpdateHead();
  virtual void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grow_count{1};
  int grid_width;
  int grid_height;
};

class EnemySnake : public Snake
{
public:
  EnemySnake(SDL_Point const &food);
  void FindFood(SDL_Point const &food);

private:
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell) override;
};

#endif