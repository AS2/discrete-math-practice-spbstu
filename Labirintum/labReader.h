#ifndef __labReader_h__
#define __labReader_h__

#include <string>

class labirintMap {
private:
  int w, h;
  int* map;
  int startX, startY, finishX, finishY;
  int mapType;

  int getArgs(std::string fileString);
  void initMap();

  int getMapRow(std::string fileString, int currentH);
public:
  labirintMap();

  int labirintMapRead(std::string fileName);
  void DrawLabirint();

  int GetW() { return w; };
  int GetH() { return h; };
  int* GetMap() { return map; };

  int GetStartX() { return startX; };
  int GetStartY() { return startY; };

  int GetFinishX() { return finishX; };
  int GetFinishY() { return finishY; };

  int GetLabType() { return mapType; };

  void ClearMap();
  ~labirintMap();
};

#endif // __labReader_h__