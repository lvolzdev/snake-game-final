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
  // nodelay(stdscr, true);
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

  InitScreen();
  getch();
  MainScreen();
  nodelay(stdscr, true);

  getmaxyx(stdscr, maxheight, maxwidth);
  maxheight = 21;
  maxwidth = 42;
  //int maxheight = 30;
  //int maxwidth = 80;
  //init a few variables
  partchar = 'O';
  // oldalchar = (char)219;
  oldalchar = '@';
  angel.x = 0;
  angel.y = 0;
  devil.x = 0;
  devil.y = 0;
  for (int i=0; i<3; i++)
    snake.push_back(SnakePos(10+i,10));
  points = 0;
  del = 150000;
  get = false;
  direction = 'l';
  over = false;
  srand(time(0));
  putangel();
  putdevil();
  //put the edges
  for (int i=0; i<maxwidth-1; i++) {
    wmove(gameWin,0,i);
    waddch(gameWin,oldalchar);
  }
  for (int i=0; i<maxwidth-1; i++) {
    wmove(gameWin,maxheight-2, i);
    waddch(gameWin,oldalchar);
  }
  for (int i=0; i<maxheight-1; i++) {
    wmove(gameWin,i,0);
    waddch(gameWin,oldalchar);
  }
  for (int i=0; i<maxheight-1; i++) {
    wmove(gameWin, i, maxwidth-2);
    waddch(gameWin, oldalchar);
  }
  //draw the snake
  for (int i=0; i<snake.size(); i++) {
    wmove(gameWin, snake[i].y, snake[i].x);
    waddch(gameWin, partchar);
  }
  //write the points(angel)
  wmove(scoreWin,maxheight-1, 0);
  mvwprintw(scoreWin, 3, 5, "%d", points);
  //draw the points(angel)
  wattron(gameWin,COLOR_PAIR(3));
  wmove(gameWin, angel.y, angel.x);
  waddch(gameWin,'A');
  wattroff(gameWin,COLOR_PAIR(3));
  wrefresh(gameWin);
  wrefresh(scoreWin);
}

Snake::~Snake() {
  nodelay(stdscr, false);
  getch();
  delwin(gameWin);
  delwin(scoreWin);
  delwin(missionWin);
  endwin();
}

void Snake::InitScreen() {
  attron(COLOR_PAIR(1));
  mvprintw(13, 33, "SNAKE GAME");
  mvprintw(15, 31, "Press Any Key..");
  border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  //getch();
  curs_set(0);
  keypad(stdscr, TRUE);
  noecho();
  refresh();
}

void Snake::MainScreen() {
  attron(COLOR_PAIR(1));
  mvprintw(2, 33, "--Snake Game--");

  GameWindow();
  ScoreWindow();
  MissionWindow();
}

void Snake::GameWindow() {
  gameWin = newwin(21, 42, 5, 4);
  wbkgd(gameWin, COLOR_PAIR(1));
  wattron(gameWin, COLOR_PAIR(1));
  keypad(stdscr,true);
  wrefresh(gameWin);
}

void Snake::ScoreWindow() {
  scoreWin = newwin(10, 28, 5, 48);
  wbkgd(scoreWin, COLOR_PAIR(1));
  wattron(scoreWin, COLOR_PAIR(1));
  wborder(scoreWin, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  mvwprintw(scoreWin, 1, 1, "- - - -Score Board- - - -");
  mvwprintw(scoreWin, 3, 2, "A : ");
  mvwprintw(scoreWin, 5, 2, "D : ");
  mvwprintw(scoreWin, 7, 2, "T : ");
  wrefresh(scoreWin);
}

void Snake::MissionWindow() {
  missionWin = newwin(10, 28, 16, 48);
  wbkgd(missionWin, COLOR_PAIR(1));
  wattron(missionWin, COLOR_PAIR(1));
  wborder(missionWin, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  mvwprintw(missionWin, 1, 1, "- - - Mission Board - - -");
  mvwprintw(missionWin, 3, 2, "A : ");
  mvwprintw(missionWin, 5, 2, "D : ");
  mvwprintw(missionWin, 7, 2, "T : ");
  wrefresh(missionWin);
  //추가
}
void Snake::NextStageScreen() {
  clear();
  nodelay(stdscr, FALSE); // ?
  attron(COLOR_PAIR(1));
  mvprintw(12, 31, "Stage Clear!");
  mvprintw(17, 26, "Press Any Key...");
  border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  getch();
  refresh();
}

void Snake::GameOverScreen() {
  clear();
  //system("cls");
  nodelay(stdscr, FALSE);
  attron(COLOR_PAIR(1));
  mvprintw(12, 33, "Game Over!");
  mvprintw(17, 33, "Try agian!");
  border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  getch();
  clear();
}

void Snake::GameClearScreen() {
    clear();
    //system("cls");
    nodelay(stdscr, FALSE);
    attron(COLOR_PAIR(1));
    mvprintw(12, 35, "Game Clear!!!");
    border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    getch();
    refresh();
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
    // add wall part
    angel.x = tmpx;
    angel.y = tmpy;
    break;
  }
  wattron(gameWin,COLOR_PAIR(3)); //angel 3번 색깔로
  wmove(gameWin,angel.y, angel.x);
  waddch(gameWin,'A');
  wattroff(gameWin,COLOR_PAIR(3));
  wrefresh(gameWin);
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
    //add wall part
    devil.x = tmpx;
    devil.y = tmpy;
    break;
  }
  wattron(gameWin,COLOR_PAIR(4)); //devil 4번 색깔로
  wmove(gameWin, devil.y, devil.x);
  waddch(gameWin, 'D');
  wattroff(gameWin,COLOR_PAIR(4));
  wrefresh(gameWin);
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
    wmove(gameWin, maxheight-1,0);
    mvwprintw(scoreWin, 3 , 5,"%d", points);
    wrefresh(scoreWin);
    if ((points%100)==0)
      del -= 10000;
  } else
    get = false;

  if(snake[0].x==devil.x && snake[0].y==devil.y) { //devil과 닿을 때
    get = true;
    putdevil();
    // 줄어드는거 어떻게?
    points -= 10;
    wmove(gameWin, maxheight-1,0);
    mvwprintw(scoreWin, 3, 5,"%d", points);
    wrefresh(scoreWin);
  }
  return false;
}

void Snake::movesnake() {
  int tmp = getch();
  switch(tmp) {
    case KEY_RIGHT:
      if(direction != 'l')
        direction = 'r';
      else
        over = true;
      break;
    case KEY_LEFT:
      if(direction != 'r')
        direction = 'l';
      else
        over = true;
      break;
    case KEY_UP:
      if(direction != 'd')
        direction = 'u';
      else
        over = true;
      break;
    case KEY_DOWN:
      if(direction != 'u')
        direction = 'd';
      else
        over = true;
      break;
    case KEY_BACKSPACE:
      direction = 'q';
      break;
  }
  if(!get) {
    wmove(gameWin, snake[snake.size()-1].y, snake[snake.size()-1].x);
    waddch(gameWin, ' ');
    wrefresh(gameWin);
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
  wmove(gameWin, snake[0].y, snake[0].x);
  waddch(gameWin, partchar);
  wrefresh(gameWin);
}

void Snake::start() {
  while(1) {
    if(collision()|| snake.size() < 3 || over) {
      GameOverScreen();
      break;
    }
    movesnake();
    if(direction=='q'){
      GameOverScreen();
      break;
    }
    usleep(del);
  }
}
