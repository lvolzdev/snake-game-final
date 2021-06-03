#ifndef __MAP__
#define __MAP__

#include <ncurses.h>
#include <string>
#include <fstream>
#include <iostream>
#include <clocale>
#include <deque>

/*
#include <utility> //pair
#include <cstdlib> //random
#include <ctime> //time
#include <unistd.h> //time sleep
*/

#define MAP_HEIGHT 21
#define MAP_WIDTH 21

class Map {
public:
  //char initMapArr[21][21];
  char mapArr[MAP_HEIGHT][MAP_WIDTH];
  std::string mapPath;
  Map();
  void setMap(int stage);
  //void getMapData(int row, int col);
};
#endif
