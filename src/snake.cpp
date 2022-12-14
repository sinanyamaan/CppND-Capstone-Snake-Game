#include "snake.h"
#include <cmath>
#include <iostream>
#include <algorithm>

void Snake::Update()
{
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)}; // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)}; // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y)
  {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead()
{
  switch (direction)
  {
  case Direction::kUp:
    head_y -= speed;
    break;

  case Direction::kDown:
    head_y += speed;
    break;

  case Direction::kLeft:
    head_x -= speed;
    break;

  case Direction::kRight:
    head_x += speed;
    break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell)
{
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing)
  {
    // Remove the tail from the vector.
    body.erase(body.begin());
  }
  else
  {
    grow_count--;
    growing = grow_count == 0 ? false : true;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body)
  {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y)
    {
      std::cout << "You tried to take a bite of yourself." << std::endl;
      alive = false;
    }
  }
}

void Snake::GrowBody(int count)
{
  growing = true;
  grow_count = count;
}

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y)
{
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y))
  {
    return true;
  }
  for (auto const &item : body)
  {
    if (x == item.x && y == item.y)
    {
      return true;
    }
  }
  return false;
}

EnemySnake::EnemySnake(SDL_Point const &food)
{
  int x_diff = grid_width - food.x;
  int y_diff = grid_height - food.y;

  if (x_diff > y_diff)
  {
    head_y = food.y;
    head_x = x_diff < grid_width / 2 ? 0 : grid_width - 1;
    direction = x_diff < grid_width / 2 ? Direction::kRight : Direction::kLeft;
  }
  else
  {
    head_x = food.x;
    head_y = y_diff < grid_height / 2 ? 0 : grid_height - 1;
    direction = y_diff < grid_height / 2 ? Direction::kDown : Direction::kUp;
  }
}

void EnemySnake::FindFood(SDL_Point const &food)
{
  if ((int)head_x == food.x && (direction == Direction::kLeft || direction == Direction::kRight))
  {
    direction = food.y - head_y < 0 ^ abs(food.y - head_y) < grid_height / 2 ? Direction::kDown : Direction::kUp;
  }
  if ((int)head_y == food.y && (direction == Direction::kUp || direction == Direction::kDown))
  {
    direction = food.x - head_x < 0 ^ abs(food.x - head_x) < grid_width / 2 ? Direction::kRight : Direction::kLeft;
  }
}

void EnemySnake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell)
{
  body.push_back(prev_head_cell);

  if (!growing)
  {
    body.erase(body.begin());
  }
  else
  {
    grow_count--;
    growing = grow_count == 0 ? false : true;
    size++;
  }

  // Check if the enemy has died.
  for (auto const &item : body)
  {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y)
    {
      std::cout << "Enemy gone crazy. You won!" << std::endl;
      alive = false;
    }
  }
}