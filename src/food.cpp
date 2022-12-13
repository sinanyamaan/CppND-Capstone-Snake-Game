#include <memory>
#include "food.h"

GoldenFruit::GoldenFruit()
{
  setGrowPower(2);
}

void Food::setGrowPower(int gp)
{
  grow_power = gp;
}