#include <iostream>
#include <ncurses.h>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#ifndef SNAKE_H
#define SNAKE_H

struct SnakePos{ // position
  int x, y;
  SnakePos(int col, int row);
  SnakePos();
};

class Snake{
  int points, del, maxwidth, maxheight;
  char direction, partchar, oldalchar, foo;
  bool get;
  SnakePos angel;
  SnakePos devil;
  std::vector<SnakePos> snake;

  void putangel();
  void putdevil();
  bool collision();
  void movesnake();
public:
  Snake();
  ~Snake();
  void start();
};

#endif
