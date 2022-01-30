#include <iostream>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <string>

#include "labDef.h"
#include "labReader.h"
#include "labSolver.h"

int main(void) {
  int res;

  do {
    std::cout << "input test txt filename (from '/tests' directory, 0 - to exit): ";
    std::string filename, filepath;

    std::cin >> filename;
    if (filename == "0")
      break;
    filepath = std::string("tests/" + filename);

    labirintMap map;
    res = map.labirintMapRead(filepath);
    //map.DrawLabirint();
    if (res == lab_true) {
      graphSolver GS;

      /***
       * TWO WAYS TO SOLVE: WAVE/DIJKSTRA or ONLYWAVE
       ***/

      GS.ParseGraph(map);
      //GS.ParseGraphOnlyWave(map);
    }
    else if (res == uncorrectMap_uncorrectArgsCnt)
      std::cout << "ERROR: uncorrect count of arguments (must be width and height)" << std::endl;
    else if (res == uncorrectMap_uncorrectSymbol)
      std::cout << "ERROR: uncorrect symbol in map" << std::endl;
    else if (res == uncorrectMap_wrongWidth)
      std::cout << "ERROR: wrong wight argument" << std::endl;
    else if (res == uncorrectMap_wrongHeight)
      std::cout << "ERROR: wrong height argument" << std::endl;
    else if (res == uncorrectMap_notPossitiveArg)
      std::cout << "ERROR: arguments must be more than zero" << std::endl;
    else if (res == uncorrectMap_noStart)
      std::cout << "ERROR: no start position" << std::endl;
    else if (res == uncorrectMap_noFinish)
      std::cout << "ERROR: no finish position" << std::endl;
    else if (res == uncorrectMap_badFilename)
      std::cout << "ERROR: this file doesn't exist" << std::endl;

    filename.clear();
  } while (1);

  _CrtDumpMemoryLeaks();
  return 0;
}