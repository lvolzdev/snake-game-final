#include "snake.h"

WINDOW *gameWin;
WINDOW *scoreWin;
WINDOW *missionWin;

SnakePos::SnakePos(int col, int row){
  x = col;
  y = row;
}

SnakePos::SnakePos(){
  x = 0;
  y = 0;
}

Snake::Snake() {
  setlocale(LC_ALL, "");
  initscr();
  resize_term(30, 80);
  nodelay(stdscr, true);
  keypad(stdscr, true);
  noecho();
  curs_set(0);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK); //main color
  init_pair(2, COLOR_CYAN, COLOR_BLACK); //snake color
  init_pair(3, COLOR_MAGENTA, COLOR_BLACK); //engel item color
  init_pair(4, COLOR_GREEN, COLOR_BLACK); //devil item color
  bkgd(COLOR_PAIR(1));
  attron(COLOR_PAIR(1));
  border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  refresh();

  getmaxyx(stdscr, maxheight, maxwidth);
  //int maxheight = 30;
  //int maxwidth = 80;
  //init a few variables
  partchar = 'O';
  oldalchar = (char)219;
  angel.x = 0;
  angel.y = 0;
  devil.x = 0;
  devil.y = 0;
  for (int i=0; i<3; i++)
    snake.push_back(SnakePos(10+i,10));
  points = 0;
  del = 110000;
  get = false;
  direction = 'l';
  srand(time(0));
  putangel();
  putdevil();
  //put the edges
  for (int i=0; i<maxwidth-1; i++) {
    move(0,i);
    addch(oldalchar);
  }
  for (int i=0; i<maxwidth-1; i++) {
    move(maxheight-2, i);
    addch(oldalchar);
  }
  for (int i=0; i<maxheight-1; i++) {
    move(i,0);
    addch(oldalchar);
  }
  for (int i=0; i<maxheight-1; i++) {
    move(i, maxwidth-2);
    addch(oldalchar);
  }
  //draw the snake
  for (int i=0; i<snake.size(); i++) {
    move(snake[i].y, snake[i].x);
    addch(partchar);
  }
  //write the points(angel)
  move(maxheight-1, 0);
  printw("%d", points);
  //draw the points(angel)
  move(angel.y, angel.x);
  addch('#');
  refresh();
}

Snake::~Snake() {
  nodelay(stdscr, false);
  getch();
  endwin();
}


void Snake::putangel() {
  while(1) {
    int tmpx = rand()%maxwidth+1;
    int tmpy = rand()%maxheight+1;
    for (int i=0; i<snake.size(); i++)
      if(snake[i].x == tmpx && snake[i].y == tmpy) //snake와 겹치지 않게
        continue;
    if(devil.x == tmpx && devil.y == tmpy) //devil item과 겹치기 않게
        continue;
    if (tmpx >= maxwidth-2 || tmpy >= maxheight-3)
      continue;
    angel.x = tmpx;
    angel.y = tmpy;
    break;
  }
  attron(COLOR_PAIR(3)); //angel 3번 색깔로
  move(angel.y, angel.x);
  addch('#');
  attroff(COLOR_PAIR(3));
  refresh();
}

void Snake::putdevil() {
  while(1) {
    int tmpx = rand()%maxwidth+1;
    int tmpy = rand()%maxheight+1;
    for (int i=0; i<snake.size(); i++)
      if(snake[i].x == tmpx && snake[i].y == tmpy) //snake와 겹치지 않게
        continue;
    if(angel.x == tmpx && angel.y == tmpy) //angel item과 겹치기 않게
      continue;
    if (tmpx >= maxwidth-2 || tmpy >= maxheight-3)
      continue;
    devil.x = tmpx;
    devil.y = tmpy;
    break;
  }
  attron(COLOR_PAIR(4)); //devil 4번 색깔로
  move(devil.y, devil.x);
  addch('#');
  attroff(COLOR_PAIR(4));
  refresh();
}

bool Snake::collision() {
  if(snake[0].x==0 || snake[0].x == maxwidth-1 || snake[0].y==0 || snake[0].y==maxheight-2) //벽
    return true;

  for (int i=2; i<snake.size(); i++) //자기 몸에 닿을 때
    if(snake[0].x == snake[i].x && snake[i].y == snake[0].y)
      return true;

  if(snake[0].x==angel.x && snake[0].y==angel.y) { //angel과 닿을 때
    get = true;
    putangel();
    points += 10;
    move(maxheight-1,0);
    printw("%d", points);
    if ((points%100)==0)
      del -= 10000;
  } else
    get = false;

  if(snake[0].x==devil.x && snake[0].y==devil.y) { //devil과 닿을 때
    get = true;
    putdevil();
    // 줄어드는거 어떻게?
    points -= 10;
    move(maxheight-1,0);
    printw("%d", points);
  }
  return false;
}

void Snake::movesnake() {
  int tmp = getch();
  switch(tmp) {
    case KEY_RIGHT:
      if(direction != 'l')
        direction = 'r';
      break;
    case KEY_LEFT:
      if(direction != 'r')
        direction = 'l';
      break;
    case KEY_UP:
      if(direction != 'd')
        direction = 'u';
      break;
    case KEY_DOWN:
      if(direction != 'u')
        direction = 'd';
      break;
    case KEY_BACKSPACE:
      direction = 'q';
      break;
  }
  if(!get) {
    move(snake[snake.size()-1].y, snake[snake.size()-1].x);
    addch(' ');
    refresh();
    snake.pop_back();
  }
  if(direction == 'l')
    snake.insert(snake.begin(), SnakePos(snake[0].x-1, snake[0].y));
  else if(direction == 'r')
    snake.insert(snake.begin(), SnakePos(snake[0].x+1, snake[0].y));
  else if(direction == 'u')
    snake.insert(snake.begin(), SnakePos(snake[0].x, snake[0].y-1));
  else if(direction == 'd')
    snake.insert(snake.begin(), SnakePos(snake[0].x, snake[0].y+1));
  move(snake[0].y, snake[0].x);
  addch(partchar);
  refresh();
}

void Snake::start() {
  while(1) {
    if(collision()) {
      move(12, 36);
      printw("game over");
      break;
    }
    movesnake();
    if(direction=='q')
      break;
    usleep(del);
  }
}
