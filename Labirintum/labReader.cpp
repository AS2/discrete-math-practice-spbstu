#include <iostream>
#include <fstream>
#include <string>

#include "labDef.h"
#include "labReader.h"

labirintMap::labirintMap() {
  w = h = 0;
  map = nullptr;
  mapType = labirintGraph;
}

int labirintMap::getArgs(std::string fileString) {
  int pos = 0, argsCnt = 0, newArg = 0, wasArgReaded = 0;

  while (fileString[pos] != 0) {
    if (!((fileString[pos] >= '0' && fileString[pos] <= '9') ||
      fileString[pos] == '-' || fileString[pos] == ' ' || fileString[pos] == '\t'))
      return uncorrectMap_uncorrectSymbol;

    if ((fileString[pos] == ' ' || fileString[pos] == '\t') && wasArgReaded == 1) {
      if (newArg == 0)
        return uncorrectMap_notPossitiveArg;

      if (argsCnt == 0)
        w = newArg;
      else if (argsCnt == 1)
        h = newArg;
      else
        return uncorrectMap_uncorrectArgsCnt;

      wasArgReaded = 0;
      newArg = 0;
      argsCnt++;
    }
    else if (fileString[pos] == '-')
      return uncorrectMap_notPossitiveArg;
    else {
      wasArgReaded = 1;
      newArg = newArg * 10 + (int)(fileString[pos] - '0');
    }

    pos++;
  }

  if (argsCnt < 2 && wasArgReaded == 0)
    return uncorrectMap_uncorrectArgsCnt;
  else if (wasArgReaded == 1)
    h = newArg;

  if (w == 0 || h == 0)
    return uncorrectMap_notPossitiveArg;

  return lab_true;
}

void labirintMap::initMap() {
  map = new int[w * h];
}

int labirintMap::getMapRow(std::string fileString, int currentH) {
  int pos = 0;

  while (fileString[pos] != 0) {
    if (pos == w)
      return uncorrectMap_wrongWidth;
    if (fileString[pos] != ' ' && fileString[pos] != '#' && fileString[pos] != 'F' && fileString[pos] != 'S')
      return uncorrectMap_uncorrectSymbol;
    else if (fileString[pos] == ' ')
      map[pos + currentH * w] = empty;
    else if (fileString[pos] == '#')
      map[pos + currentH * w] = wall;
    else if (fileString[pos] == 'S') {
      startX = pos;
      startY = currentH;
      map[pos + currentH * w] = start;
    }
    else if (fileString[pos] == 'F') {
      finishX = pos;
      finishY = currentH;
      map[pos + currentH * w] = finish;
    }

    if (pos >= 1 && currentH >= 1)
      if (map[pos + currentH * w] != wall && map[(pos - 1) + currentH * w] != wall &&
        map[pos + (currentH - 1) * w] != wall && map[(pos - 1) + (currentH - 1) * w] != wall)
        mapType = terrariumGraph;

    pos++;
  }

  if (pos < w)
    return uncorrectMap_wrongWidth;

  return lab_true;
}

int labirintMap::labirintMapRead(std::string fileName) {
  std::ifstream labirinteFile(fileName);
  if (labirinteFile.is_open() == 0)
    return uncorrectMap_badFilename;

  // read map
  int stringsReaded = 0, res, argsWasReaden = 0;
  std::string oneStringFromFile;
  while (getline(labirinteFile, oneStringFromFile)) {
    if (argsWasReaden != 0 && stringsReaded == h)
      return uncorrectMap_wrongHeight;

    if (argsWasReaden == 0) {
      res = getArgs(oneStringFromFile);
      if (res != lab_true)
        return res;
      
      initMap();
      argsWasReaden = 1;
    }
    else {
      res = getMapRow(oneStringFromFile, stringsReaded);
      if (res != lab_true)
        return res;

      stringsReaded++;
    }
  }
  if (stringsReaded < h)
    return uncorrectMap_wrongHeight;

  // check map structure
  int wasStart = 0, wasFinish = 0;
  for (int i = 0; i < w; i++)
    for (int j = 0; j < h; j++)
      if (map[i + j * w] == start)
        wasStart = 1;
      else if (map[i + j * w] == finish)
        wasFinish = 1;
  if (!wasStart)
    return uncorrectMap_noStart;
  if (!wasFinish)
    return uncorrectMap_noFinish;

  return lab_true;
}

void labirintMap::DrawLabirint() {
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++)
      if (map[i + j * w] == wall)
        std::cout << "#";
      else if (map[i + j * w] == empty)
        std::cout << " ";
      else if (map[i + j * w] == start)
        std::cout << "S";
      else if (map[i + j * w] == finish)
        std::cout << "F";
    std::cout << std::endl;
  }
}

void labirintMap::ClearMap() {
  delete map;
}

labirintMap::~labirintMap() {
  delete[] map;
}

