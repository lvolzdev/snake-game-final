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
  int del, maxwidth, maxheight;
  char direction, partchar;
  int angel_points, devil_points, total_points;
  int angel_timer, devil_timer, gate_timer, total_timer;
  bool get;
  SnakePos angel;
  SnakePos devil;
  std::vector<SnakePos> snake;
  bool over;
  int nowlength;

  //screen
  WINDOW *gameWin;
  WINDOW *scoreWin;
  WINDOW *missionWin;

  void putangel();
  void putdevil();
  void angeltime();
  void deviltime();
  int getlength();
  bool collision();
  void movesnake();

  //screen function
  void InitScreen(); //초기 화면
  void MainScreen();
  void GameWindow();
  void getMapOnGameWindow();
  void ScoreWindow();
  void MissionWindow();
  // void update();

  void NextStageScreen();
  void GameOverScreen();
  void GameClearScreen();
public:
  Snake();
  ~Snake();
  void start();
};

#endif
