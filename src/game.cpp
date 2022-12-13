#include <iostream>
#include "SDL.h"
#include "game.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(std::make_unique<Snake>(grid_width, grid_height)),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      random_f(1, 11)
{
  PlaceFood();
  enemy = std::make_unique<EnemySnake>(food);
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration)
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running)
  {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, *snake);
    Update();
    renderer.Render(*snake, *enemy, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000)
    {
      renderer.UpdateWindowTitle(score, high_score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration)
    {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood()
{
  std::lock_guard<std::mutex> lock(mtx);

  int f = random_f(engine);
  // food = f == 1 ? std::unique_ptr<Food>(new GoldenFruit) : std::unique_ptr<Food>(new Food);
  food = GoldenFruit();

  int x, y;
  while (true)
  {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake->SnakeCell(x, y))
    {
      food.x = x;
      food.y = y;
      return;
    }
    if (enemy != nullptr && !enemy->SnakeCell(x, y))
    {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update()
{
  if (!snake->alive || !enemy->alive)
    return;

  snake->Update();

  int32_t new_x = static_cast<int>(snake->head_x);
  int32_t new_y = static_cast<int>(snake->head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y)
  {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake->GrowBody();
    snake->speed += 0.02;
  }

  enemy->FindFood(food);
  enemy->Update();

  int e_new_x = (int)enemy->head_x;
  int e_new_y = (int)enemy->head_y;

  if (food.x == e_new_x && food.y == e_new_y) // Check if the enemy ate the food
  {
    PlaceFood();
    // Grow enemy->
    enemy->GrowBody();
  }

  // Check if the snake touched the enemy
  if (enemy->SnakeCell(new_x, new_y))
  {
    std::cout << "You ate the wrong food." << std::endl;
    snake->alive = false;
  }

  // Check if the enemy touched the snakes tail
  for (int i = 0; i < snake->body.size(); i++)
  {
    if (e_new_x == snake->body[i].x && e_new_y == snake->body[i].y)
    {
      std::cout << "You got smaller by " << i + 1 << " pieces." << std::endl;
      enemy->GrowBody(i + 1);

      snake->size -= i + 1;
      snake->speed -= (i + 1) * 0.02;

      for (int j = 0; j <= i; j++)
      {
        snake->body.erase(snake->body.begin());
      }
    }
  }

  score = snake->size - 1;

  if (high_score < score)
    high_score = score;
}

int Game::GetScore() const { return score; }

int Game::GetHighScore() const { return high_score; }