#include <iostream>
#include <ncurses.h>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include "map.h"
#ifndef SNAKE_H
#define SNAKE_H

#define SPACE '0'
#define WALL '1'
#define IMMUNEWALL '2'
#define HEAD '3'
#define BODY '4'
#define GATE '5'
#define ANGEL '6'
#define DEVIL '7'

struct SnakePos{ // position
  int x, y;
  SnakePos(int col, int row);
  SnakePos();
};

class Snake{
  int del, maxwidth, maxheight;
  char direction, partchar;
  int angel_points, devil_points, total_points, gate_points;
  int angel_timer, devil_timer, gate_timer, total_timer;
  bool get, pass;
  SnakePos angel;
  SnakePos devil;

  std::vector<SnakePos> snake;

  bool over;
  int nowlength;
  int stage;
  SnakePos snakehead;
  int goal_size, goal_angel, goal_devil, goal_gate;

  //screen
  WINDOW *gameWin;
  WINDOW *scoreWin;
  WINDOW *missionWin;

  void putangel();
  void putdevil();

  void angeltime();
  void deviltime();
  void gatetime();
  void setMission();
  bool stageclear();
  void changestage();


  int getlength();
  bool collision();
  void movesnake();
  void drawSnake();

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
