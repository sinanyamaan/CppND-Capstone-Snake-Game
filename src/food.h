#ifndef FOOD_H
#define FOOD_H

#include "SDL.h"
#include "snake.h"

class Food
{
public:
  int x;
  int y;

  void setGrowPower(int gp);

  Food operator=(Food const &other)
  {
    Food ret;
    ret.x = other.x;
    ret.y = other.y;
    ret.grow_power = other.grow_power;
    return ret;
  }

private:
  int grow_power{1};
};

class GoldenFruit : public Food
{
public:
  GoldenFruit(){};
};

#endif