#include "map.h"

Map::Map(){}

void Map::setMap(int stage) {
  mapPath = "../map/map" + std::to_string(stage) + ".txt";
  std::ifstream in(mapPath);
  try {
    if(!in) throw;
  }
  catch(...) {
    std::cout << "failed file read" << std::endl;
  }

  int row = 0;
  int col = 0;
  int c;
  while ((c = in.get()) != EOF) {
    if (c == '\n') { //txt 파일에서 하나씩 읽어오는데 엔터면
      row += 1;
      col = 0;
      continue;
    }
    else if (c != '0' && c != '1' && c != '2') {
      continue;
    }

    mapArr[row][col] = (char)c; //char 배열이니까 char로 형변환해서 넣어주기
    col += 1;
  }
  in.close();
}
