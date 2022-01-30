#ifndef __labDef_h__
#define __labDef_h__

enum graphTypes {
  terrariumGraph = 1,   // a lot of spaces -> use wave algorithm
  labirintGraph = 2     // no empty spaces, only corridors -> use Dijkstra’s algorithm
};

enum cellType {
  empty = -1,
  wall = -2,
  start = -3,
  finish = -4,

  path = -5,
};

enum returnType {
  lab_true = -1,

  uncorrectMap_uncorrectArgsCnt = -2,
  uncorrectMap_uncorrectSymbol = -3,
  uncorrectMap_wrongWidth = -4,
  uncorrectMap_wrongHeight = -5,
  uncorrectMap_notPossitiveArg = -6,
  uncorrectMap_noStart = -7,
  uncorrectMap_noFinish = -8,
  uncorrectMap_noWayToFinish = -9,
  uncorrectMap_badFilename = -10
};

#endif // __labDef_h__
