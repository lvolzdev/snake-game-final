#include "snake.h"
#include "map.h"

WINDOW *gameWin;
WINDOW *scoreWin;
WINDOW *missionWin;
Map m;

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
  init_pair(5, COLOR_YELLOW, COLOR_BLACK);
  bkgd(COLOR_PAIR(1));
  attrset(A_BOLD);
  attron(COLOR_PAIR(1));
  border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  refresh();

  InitScreen();
  getch();
  //map 로드 - 1단8
  stage = 1;
  m.setMap(stage);
  MainScreen();
  setMission();
  nodelay(stdscr, true);

  getmaxyx(stdscr, maxheight, maxwidth);
  maxheight = 21;
  maxwidth = 42;
  //init a few variables
  partchar = 'O';
  angel.x = 0;
  angel.y = 0;
  devil.x = 0;
  devil.y = 0;


  angel_points = 0;
  devil_points = 0;
  total_points = 0;
  gate_points = 0;
  angel_timer = 0;
  devil_timer = 0;
  gate_timer = 0;
  total_timer = 0;
  del = 140000;
  get = false;
  direction = 'l';
  over = false;
  pass = false;
  // snakehead = snake[0];

  srand(time(0));
  // readwall();
  putangel();
  putdevil();
  // putgate();

  drawSnake();
  //write the points(angel)
  getlength();
  wmove(scoreWin,maxheight-1, 0);
  mvwprintw(scoreWin, 2, 8, "%d", nowlength);
  mvwprintw(scoreWin, 4, 8, "%d", angel_points);
  mvwprintw(scoreWin, 6, 8, "%d", devil_points);
  mvwprintw(scoreWin, 8, 8, "%d", gate_points);
  mvwprintw(scoreWin, 8, 21, "%d", total_points);
  mvwprintw(scoreWin, 2, 21, "%d", goal_size); // maxlength

  //draw the points(angel)
  wattroff(gameWin, A_BOLD);
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
void Snake::drawSnake(){
  if(snake.size()!= 3){
    for(int i=0; i < snake.size(); i++){
      snake.pop_back();
    }
  }
  for (int i=0; i<3; i++)
    snake.push_back(SnakePos(10+i, 5));
  for (int i=0; i<snake.size(); i++) {
    wmove(gameWin, snake[i].y, snake[i].x);
    waddch(gameWin, partchar);
  }
}
void Snake::InitScreen() {
  attron(COLOR_PAIR(1));
  attrset(A_BOLD);
  mvprintw(13, 33, "SNAKE GAME");
  mvprintw(18, 22, "Press Any Key To Start!");
  border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  //getch();
  curs_set(0);
  keypad(stdscr, TRUE);
  noecho();
  refresh();

}

void Snake::MainScreen() {
  attron(COLOR_PAIR(1));
  attrset(A_BOLD);
  mvprintw(2, 33, "--Snake Game--");
  mvprintw(2, 65, "Stage : ");
  mvprintw(2, 74, "%d", stage);


  GameWindow();
  ScoreWindow();
  MissionWindow();
}

void Snake::GameWindow() {
  gameWin = newwin(21, 42, 5, 4);
  wbkgd(gameWin, COLOR_PAIR(1));
  wattron(gameWin, COLOR_PAIR(1));
  wattrset(gameWin, A_BOLD);
  keypad(stdscr,true);
  getMapOnGameWindow();
  wrefresh(gameWin);
}

void Snake::getMapOnGameWindow() {
  for (int i=0; i<MAP_HEIGHT; i++) {
    for (int j=0; j<MAP_WIDTH; j++) {
      char c = m.mapArr[i][j];
      if (c == WALL || c == IMMUNEWALL)
        mvwprintw(gameWin, i, j*2, "ㅁ"); //"\u25A0"
      else if (c == SPACE)
        mvwprintw(gameWin, i, j*2, "\u0020");
    }
  }
  wrefresh(gameWin);
}

void Snake::ScoreWindow() {
  scoreWin = newwin(10, 28, 5, 48);
  wbkgd(scoreWin, COLOR_PAIR(1));
  wattron(scoreWin, COLOR_PAIR(1));
  wattrset(scoreWin, A_BOLD);
  wborder(scoreWin, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  mvwprintw(scoreWin, 1, 1, "- - - -Score Board- - - -");
  mvwprintw(scoreWin, 2, 2, "B : "); // snake size
  mvwprintw(scoreWin, 2, 15, "MAX : "); // maxlength
  mvwprintw(scoreWin, 4, 2, "+ : "); // angel
  mvwprintw(scoreWin, 6, 2, "- : "); // devil
  mvwprintw(scoreWin, 8, 2, "G : "); //gate
  mvwprintw(scoreWin, 8, 17, "T : "); //totalscore
  wrefresh(scoreWin);
}

void Snake::MissionWindow() {
  missionWin = newwin(10, 28, 16, 48);
  wbkgd(missionWin, COLOR_PAIR(1));
  wattron(missionWin, COLOR_PAIR(1));
  wattrset(missionWin, A_BOLD);
  wborder(missionWin, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  mvwprintw(missionWin, 1, 1, "- - - Mission Board - - -");
  mvwprintw(missionWin, 2, 2, "B : ");
  mvwprintw(missionWin, 4, 2, "+ : ");
  mvwprintw(missionWin, 6, 2, "- : ");
  mvwprintw(missionWin, 8, 2, "G : ");
  wrefresh(missionWin);
  //추가
}
void Snake::NextStageScreen() {
  clear();
  nodelay(stdscr, FALSE); // ?
  attron(COLOR_PAIR(1));
  attrset(A_BOLD);
  mvprintw(12, 31, "Stage Clear!");
  mvprintw(17, 25, "Press Any Key To Continue!");
  border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  getch();
  refresh();
}

void Snake::GameOverScreen() {
  clear();
  //system("cls");
  nodelay(stdscr, FALSE);
  attron(COLOR_PAIR(1));
  attrset(A_BOLD);
  mvprintw(12, 33, "Game Over!");
  mvprintw(17, 33, "Try agian!");
  border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  getch();
  endwin();
}

void Snake::GameClearScreen() {
    clear();
    //system("cls");
    nodelay(stdscr, FALSE);
    attron(COLOR_PAIR(1));
    mvprintw(12, 33, "Game Clear!!!");
    border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    getch();
    endwin();
}
void Snake::setMission(){
  //clear scoreboard
  switch (stage) {

    case 1:

      mvwprintw(missionWin, 2, 8, "4");
      mvwprintw(missionWin, 4, 8, "30");
      mvwprintw(missionWin, 6, 8, "20");
      mvwprintw(missionWin, 8, 8, "2");
      wrefresh(missionWin);

      goal_size = 4;
      goal_angel = 30;
      goal_devil = 20;
      goal_gate = 2;

      break;

    case 2:
      mvwprintw(missionWin, 2, 8, "6");
      mvwprintw(missionWin, 4, 8, "50");
      mvwprintw(missionWin, 6, 8, "40");
      mvwprintw(missionWin, 8, 8, "4");
      wrefresh(missionWin);

      goal_size = 6;
      goal_angel = 50;
      goal_devil = 40;
      goal_gate = 4;

      break;

    case 3:
      mvwprintw(missionWin, 2, 8, "8");
      mvwprintw(missionWin, 4, 8, "70");
      mvwprintw(missionWin, 6, 8, "60");
      mvwprintw(missionWin, 8, 8, "6");
      wrefresh(missionWin);

      goal_size = 8;
      goal_angel = 70;
      goal_devil = 60;
      goal_gate = 6;

      break;

    case 4:
      mvwprintw(missionWin, 2, 8, "10");
      mvwprintw(missionWin, 4, 8, "80");
      mvwprintw(missionWin, 6, 8, "70");
      mvwprintw(missionWin, 8, 8, "7");
      wrefresh(missionWin);

      goal_size = 10;
      goal_angel = 80;
      goal_devil = 70;
      goal_gate = 7;

      break;
  }
}
bool Snake::stageclear(){
  if(snake.size() >= goal_size && angel_points >= goal_angel && devil_points >= goal_devil && gate_points >= goal_gate){
    return true;
  }
  return false;
}

void Snake::changestage(){
  // for(int i=0; i<snake.size(); i++){
  //   snake.pop_back();
  // }
  if(stageclear()){
    if(stage == 4){
      GameClearScreen();
    }else{
      NextStageScreen();
      stage++;
      m.setMap(stage);
      MainScreen();
      setMission();
      nodelay(stdscr, true);

      getmaxyx(stdscr, maxheight, maxwidth);
      maxheight = 21;
      maxwidth = 42;
      //init a few variables
      partchar = 'O';
      angel.x = 0;
      angel.y = 0;
      devil.x = 0;
      devil.y = 0;


      angel_points = 0;
      devil_points = 0;
      total_points = 0;
      gate_points = 0;
      angel_timer = 0;
      devil_timer = 0;
      gate_timer = 0;
      total_timer = 0;
      del = 140000;
      get = false;
      direction = 'l';
      over = false;
      pass = false;
      // snakehead = snake[0];

      srand(time(0));
      // readwall();
      putangel();
      putdevil();
      // putgate();

      drawSnake();
      //write the points(angel)
      getlength();
      wmove(scoreWin,maxheight-1, 0);
      mvwprintw(scoreWin, 2, 8, "%d", nowlength);
      mvwprintw(scoreWin, 4, 8, "%d", angel_points);
      mvwprintw(scoreWin, 6, 8, "%d", devil_points);
      mvwprintw(scoreWin, 8, 8, "%d", gate_points);
      mvwprintw(scoreWin, 8, 21, "%d", total_points);
      mvwprintw(scoreWin, 2, 21, "%d", goal_size); // maxlength

      //draw the points(angel)
      wattroff(gameWin,A_BOLD);
      wattron(gameWin,COLOR_PAIR(3));
      wmove(gameWin, angel.y, angel.x);
      waddch(gameWin,'A');
      wattroff(gameWin,COLOR_PAIR(3));
      wrefresh(gameWin);
      wrefresh(scoreWin);
    }
  }
}


int Snake::getlength(){
  nowlength = snake.size();
  return nowlength;
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
    if (m.mapArr[tmpy][tmpx] == WALL || m.mapArr[tmpy][tmpx] == IMMUNEWALL) //벽과 겹치지 않게
        continue;
    if (tmpx >= maxwidth-2 || tmpy >= maxheight-3)
      continue;
    // add wall, gate part
    angel.x = tmpx;
    angel.y = tmpy;
    m.mapArr[tmpy][tmpx] = '6'; //angel item data value
    break;
  }
  wattroff(gameWin,A_BOLD);
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
    if (m.mapArr[tmpy][tmpx] == WALL || m.mapArr[tmpy][tmpx] == IMMUNEWALL) //벽과 겹치지 않게
      continue;
    if (tmpx >= maxwidth-2 || tmpy >= maxheight-3)
      continue;
    //add wall , gate part
    devil.x = tmpx;
    devil.y = tmpy;
    m.mapArr[tmpy][tmpx] = '7'; //devil item data value
    break;
  }
  wattroff(gameWin,A_BOLD);
  wattron(gameWin,COLOR_PAIR(4)); //devil 4번 색깔로
  wmove(gameWin, devil.y, devil.x);
  waddch(gameWin, 'D');
  wattroff(gameWin,COLOR_PAIR(4));
  wrefresh(gameWin);
}

void Snake::angeltime(){
  angel_timer++;
  if(angel_timer % 60 == 0 ){
    wmove(gameWin, angel.y, angel.x);
    wprintw(gameWin, " ");
    //add map change 0
    putangel();
    angel_timer = 0;
  }
}

void Snake::deviltime(){
  devil_timer++;
  if(devil_timer % 60 == 0){
    wmove(gameWin, devil.y, devil.x);
    wprintw(gameWin, " ");
    //add map change 0
    putdevil();
    devil_timer = 0;
  }
}

// void Snake::gatetime(){
//   gate_timer++;
//   if(gate_timer % 110){
//     wattron(gameWin, COLOR_PAIR(5));
//     wmove(gameWin, gate_one.y, gate_one.x);
//     wprintw(gameWin, "G");
//     wmove(gameWin, gate_two.y, gate_two.x);
//     wprintw(gameWin, "G");
//     wrefresh(gameWin);
//     wattroff(gameWin, COLOR_PAIR(5));
//     putgate();
//     gate_timer = 0;
//   }
// }


bool Snake::collision() {
  //벽에 닿을 때
  if(snake[0].x==0 || snake[0].x == maxwidth || snake[0].y==0 || snake[0].y==maxheight) //벽
    return true;


  // for(int i=0; i<snake.size()-1; i++) {
  //   if(m.mapArr[snake[i].y][snake[i].x] == WALL || m.mapArr[snake[i].y][snake[i].x] == IMMUNEWALL) {
  //     return true;
  //   }
  // }

  //자기 몸에 닿을 때
  for (int i=2; i<snake.size(); i++)
    if(snake[0].x == snake[i].x && snake[i].y == snake[0].y)
      return true;

  //angel과 닿을 때
  if(snake[0].x==angel.x && snake[0].y==angel.y) {
    get = true;
    putangel();
    angel_points += 10;
    total_points += 10;
    m.mapArr[angel.y][angel.x] = '0';
    getlength();
    wmove(gameWin, maxheight-1,0);
    mvwprintw(scoreWin, 4 , 8,"%d", angel_points);
    mvwprintw(scoreWin, 2, 8, "%d", nowlength);
    mvwprintw(scoreWin, 8, 21, "%d", total_points);
    wrefresh(scoreWin);
    if(angel_points >= goal_angel){
      mvwprintw(missionWin, 4, 15, "v");
    }
    if(nowlength >= goal_size){
      mvwprintw(missionWin, 2, 15, "v");
    }
    wrefresh(missionWin);
    if ((angel_points%100)==0)
      del -= 10000;
  } else
    get = false;

  //devil과 닿을 때
  if(snake[0].x==devil.x && snake[0].y==devil.y) {
    putdevil();
    wmove(gameWin, snake[snake.size()-1].y, snake[snake.size()-1].x);
    snake.pop_back();
    wprintw(gameWin, " ");
    wrefresh(gameWin);

    getlength();
    devil_points += 10;
    total_points -= 10;
    m.mapArr[devil.y][devil.x] = '0';
    wmove(gameWin, maxheight-1,0);
    mvwprintw(scoreWin, 6, 8,"%d", devil_points);
    mvwprintw(scoreWin, 2, 8,"%d", nowlength);
    mvwprintw(scoreWin, 8, 21, "%d", total_points);
    wrefresh(scoreWin);
    if(devil_points >= goal_devil){
      mvwprintw(missionWin, 6, 15, "v");
    }
    if(nowlength >= goal_size){
      mvwprintw(missionWin, 2, 15, "v");
    }
    wrefresh(missionWin);
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
  getlength();
  mvwprintw(scoreWin, 2, 8, "%d", nowlength);
  wrefresh(scoreWin);
  if(nowlength >= goal_size){
    mvwprintw(missionWin, 2, 15, "v");
  }
  if(gate_points >= goal_gate){
    mvwprintw(missionWin, 8, 15, "v");
  }
  wrefresh(missionWin);

}

void Snake::start() {
  while(1) {
    if (collision()|| snake.size() < 3 || over) {
      GameOverScreen();
      break;
    }
    movesnake();
    stageclear();
    changestage();
    if (direction=='q'){
      GameOverScreen();
      break;
    }
    usleep(del);
    angeltime();
    deviltime();
    // gatetime();
  }
}
