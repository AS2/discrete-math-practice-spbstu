#ifndef __labSolver_h__
#define __labSolver_h__

#include <vector>

#include "labReader.h"

class point {
private:
  int x, y, dist;
public:
  point() { x = y = dist = 0; };
  point(int newDist, int newX, int newY) {
    dist = newDist, x = newX, y = newY;
  };

  int GetDist() { return dist; }
  int GetX() { return x; }
  int GetY() { return y; }
};

class vertex {
private:
  int index, mapX, mapY, smallestDistance, prevVertexIndex, wasHere;
public:
  vertex();
  vertex(int newIndex, int x, int y, int distance, int prevIndex);

  int GetIndex() { return index; }
  int GetX() { return mapX; }
  int GetY() { return mapY; }

  void SetSmallestDistance(int newDistance) { smallestDistance = newDistance; }
  int GetSmallestDistance() { return smallestDistance; }

  void SetPrevVertexIndex(int newPrevVertexIndex) { prevVertexIndex = newPrevVertexIndex; }
  int GetPrevVertexIndex() { return prevVertexIndex; }

  int wasInVertex() { return wasHere; };
  void CheckVertex() { wasHere = 1; };
};

class graphSolver {
private:
  int p, q;
  int graphType;

  int* map;
  int startX, startY, finishX, finishY, mapW, mapH;

  // init graph solver
  void InitGraphSolverParams(labirintMap& labirint);

  // for terrarium
  int SolveWaveMethod();
  void BuildWavePath();
  void PrintLab();

  // for labirint
  int startIndex, finalIndex;
  int *matrAdjacency;
  std::vector<vertex> vertexes;

  int CountWayDown(int x, int y, int& finalVertex);
  int CountWayUp(int x, int y, int& finalVertex);
  int CountWayRight(int x, int y, int& finalVertex);
  int CountWayLeft(int x, int y, int& finalVertex);

  void InitLabirint(labirintMap& labirint);
  int SolveDijkstraMehod(void);
  void PrintDijkstraSolve(void);
public:
  graphSolver();

  int ParseGraph(labirintMap& labirint);
  int ParseGraphOnlyWave(labirintMap& labirint);

  void ClearGraphSolver();
  ~graphSolver();
};

#endif // __labSolver_h__