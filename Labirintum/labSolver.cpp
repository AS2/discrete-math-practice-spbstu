#include <iostream>
#include <queue>
#include <stack>

#include "labSolver.h"
#include "labDef.h"

#define infinite 1000000000

/******
 * class 'vertex'
 ******/
vertex::vertex() {
  index = mapX = mapY = smallestDistance = prevVertexIndex = wasHere = 0;
}

vertex::vertex(int newIndex, int x, int y, int distance, int prevIndex) {
  index = newIndex;
  mapX = x;
  mapY = y;
  smallestDistance = distance;
  prevVertexIndex = prevIndex;
  wasHere = 0;
}

/******
 * Graph solver
 ******/

graphSolver::graphSolver() {
  graphType = 0;
  p = q = 0;
  map = nullptr;
  matrAdjacency = nullptr;
}

int graphSolver::SolveWaveMethod() {
  // init
  std::queue<point> pointToSpreadWave;
  int dist = 0;
  map[startX + startY * mapW] = dist;
  pointToSpreadWave.push(point(dist, startX, startY));

  // wave spread
  point currentpoint;
  int x, y;
  do {
    currentpoint = pointToSpreadWave.front();
    pointToSpreadWave.pop();

    x = currentpoint.GetX(), y = currentpoint.GetY();
    dist = currentpoint.GetDist();

    if (x > 0 && (map[(x - 1) + y * mapW] == empty || map[(x - 1) + y * mapW] == finish))
      map[(x - 1) + y * mapW] = dist + 1, pointToSpreadWave.push(point(dist + 1, x - 1, y));

    if (x < mapW - 1 && (map[(x + 1) + y * mapW] == empty || map[(x + 1) + y * mapW] == finish))
      map[(x + 1) + y * mapW] = dist + 1, pointToSpreadWave.push(point(dist + 1, x + 1, y));

    if (y > 0 && (map[x + (y - 1) * mapW] == empty || map[x + (y - 1) * mapW] == finish))
      map[x + (y - 1) * mapW] = dist + 1, pointToSpreadWave.push(point(dist + 1, x, y - 1));

    if (y < mapH - 1 && (map[x + (y + 1) * mapW] == empty || map[x + (y + 1) * mapW] == finish))
      map[x + (y + 1) * mapW] = dist + 1, pointToSpreadWave.push(point(dist + 1, x, y + 1));

  } while (map[finishX + finishY * mapW] == finish && !pointToSpreadWave.empty());

  if (map[finishX + finishY * mapW] == finish)
    return uncorrectMap_noWayToFinish;

  return lab_true;
}

void graphSolver::BuildWavePath() {
  int currentDist = map[finishX + finishY * mapW], currentX = finishX, currentY = finishY;

  while (currentDist != 0) {
    if (currentX > 0 && map[(currentX - 1) + currentY * mapW] == currentDist - 1) {
      map[(currentX - 1) + currentY * mapW] = path;
      currentDist--;
      currentX--;
    }
    else if (currentX < mapW - 1 && map[(currentX + 1) + currentY * mapW] == currentDist - 1) {
      map[(currentX + 1) + currentY * mapW] = path;
      currentDist--;
      currentX++;
    }
    else if (currentY > 0 && map[currentX + (currentY - 1) * mapW] == currentDist - 1) {
      map[currentX + (currentY - 1) * mapW] = path;
      currentDist--;
      currentY--;
    }
    else if (currentY < mapH - 1 && map[currentX + (currentY + 1) * mapW] == currentDist - 1) {
      map[currentX + (currentY + 1) * mapW] = path;
      currentDist--;
      currentY++;
    }
  }
}

void graphSolver::PrintLab() {
  for (int y = 0; y < mapH; y++) {
    for (int x = 0; x < mapW; x++) {
      if (x == startX && y == startY)
        std::cout << "S";
      else if (x == finishX && y == finishY)
        std::cout << "F";
      else if (map[x + y * mapW] == path)
        std::cout << ".";
      else if (map[x + y * mapW] == wall)
        std::cout << "#";
      else
        std::cout << " ";
    }
    std::cout << std::endl;
  }
}

static int CountSidesCells(int* map, int w, int h, int x, int y) {
  int sideCells = 0;

  if (x > 0 && map[x + y * w - 1] != wall)
    sideCells++;
  if (x < w - 1 && map[x + y * w + 1] != wall)
    sideCells++;
  if (y > 0 && map[x + (y - 1) * w] != wall)
    sideCells++;
  if (y < h - 1 && map[x + (y + 1) * w] != wall)
    sideCells++;

  return sideCells;
}

int graphSolver::CountWayDown(int x, int y, int& finalVertex) {
  int res = 0;

  while (y < mapH - 1 && map[x + (y + 1) * mapW] == empty)
    y++, res++;

  if (y == mapH - 1 || (y < mapH - 1 && map[x + (y + 1) * mapW] == wall)) {
    if (x > 0 && map[(x - 1) + y * mapW] != wall)
      res += CountWayLeft(x, y, finalVertex);
    else
      res += CountWayRight(x, y, finalVertex);
  }
  else
    finalVertex = map[x + (y + 1) * mapW];

  return res;
}

int graphSolver::CountWayUp(int x, int y, int &finalVertex) {
  int res = 0;

  while (y > 0 && map[x + (y - 1) * mapW] == empty)
    y--, res++;

  if (y == 0 || (y > 0 && map[x + (y - 1) * mapW] == wall)) {
    if (x > 0 && map[(x - 1) + y * mapW] != wall)
      res += CountWayLeft(x, y, finalVertex);
    else
      res += CountWayRight(x, y, finalVertex);
  }
  else
    finalVertex = map[x + (y - 1) * mapW];

  return res;
}

int graphSolver::CountWayRight(int x, int y, int& finalVertex) {
  int res = 0;

  while (x < mapW - 1 && map[(x + 1) + y * mapW] == empty)
    x++, res++;

  if (x == mapW - 1 || (x < mapW - 1 && map[(x + 1) + y * mapW] == wall)) {
    if (y > 0 && map[x + (y - 1) * mapW] != wall)
      res += CountWayUp(x, y, finalVertex);
    else
      res += CountWayDown(x, y, finalVertex);
  }
  else
    finalVertex = map[(x + 1) + y * mapW];

  return res;
}

int graphSolver::CountWayLeft(int x, int y, int& finalVertex) {
  int res = 0;

  while (x > 0 && map[(x - 1) + y * mapW] == empty)
    x--, res++;

  if (x == 0 || (x > 0 && map[(x - 1) + y * mapW] == wall)) {
    if (y > 0 && map[x + (y - 1) * mapW] != wall)
      res += CountWayUp(x, y, finalVertex);
    else
      res += CountWayDown(x, y, finalVertex);
  }
  else
    finalVertex = map[(x - 1) + y * mapW];

  return res;
}

void graphSolver::InitLabirint(labirintMap& labirint) {
  int lastGraphIndex = 0, sideCells;

  // init labirint structer and count vertex
  for (int j = 0; j < mapH; j++)
    for (int i = 0; i < mapW; i++)
      if (map[i + j * mapW] != wall) {
        sideCells = CountSidesCells(map, mapW, mapH, i, j);
        if (map[i + mapW * j] == start)
          map[i + j * mapW] = startIndex = lastGraphIndex++;
        else if (map[i + mapW * j] == finish)
          map[i + j * mapW] = finalIndex = lastGraphIndex++;
        else if (sideCells != 2)
          map[i + j * mapW] = lastGraphIndex++;
      }
  p = lastGraphIndex;
  vertexes = std::vector<vertex>(p);
  matrAdjacency = new int[p * p];
  for (int i = 0; i < p * p; i++)
    matrAdjacency[i] = 0;

  for (int j = 0; j < mapH; j++)
    for (int i = 0; i < mapW; i++)
      if (map[i + j * mapW] >= 0) {
        if (i == startX && j == startY)
          vertexes[map[i + j * mapW]] = vertex(map[i + j * mapW], i, j, 0, -1);
        else
          vertexes[map[i + j * mapW]] = vertex(map[i + j * mapW], i, j, infinite, -1);
      }

  // make adjacency matrix with edges
  for (int i = 0; i < p; i++) {
    int verX = vertexes[i].GetX(), verY = vertexes[i].GetY(), 
      nearVertexIndex, distance;
  
    // look for paths to next vertex
    if (verX < mapW - 1 && map[(verX + 1) + verY * mapW] != wall) {
      distance = CountWayRight(verX, verY, nearVertexIndex);
      matrAdjacency[i + p * nearVertexIndex] = matrAdjacency[nearVertexIndex + p * i] = distance + 1;
    }
    if (verX > 0 && map[(verX - 1) + verY * mapW] != wall) {
      distance = CountWayLeft(verX, verY, nearVertexIndex);
      matrAdjacency[i + p * nearVertexIndex] = matrAdjacency[nearVertexIndex + p * i] = distance + 1;
    }
    if (verY < mapH - 1 && map[verX + (verY + 1) * mapW] != wall) {
      distance = CountWayDown(verX, verY, nearVertexIndex);
      matrAdjacency[i + p * nearVertexIndex] = matrAdjacency[nearVertexIndex + p * i] = distance + 1;
    }
    if (verY > 0 && map[verX + (verY - 1) * mapW] != wall) {
      distance = CountWayUp(verX, verY, nearVertexIndex);
      matrAdjacency[i + p * nearVertexIndex] = matrAdjacency[nearVertexIndex + p * i] = distance + 1;
    }
  }
}

int graphSolver::SolveDijkstraMehod(void) {
  std::queue<vertex> vertexesToCheck;
  int currentIndex = startIndex, smallestDistance, prevIndex;
  vertexesToCheck.push(vertexes[startIndex]);

  vertex currentVertex;
  do {
    currentVertex = vertexesToCheck.front();
    currentIndex = currentVertex.GetIndex();
    smallestDistance = currentVertex.GetSmallestDistance();
    prevIndex = currentVertex.GetPrevVertexIndex();
    vertexesToCheck.pop();

    for (int i = 0; i < p; i++)
      if (vertexes[i].wasInVertex() == 0 && matrAdjacency[currentIndex * p + i] != 0 &&
        matrAdjacency[currentIndex * p + i] + smallestDistance < vertexes[i].GetSmallestDistance()) {
        vertexes[i].SetSmallestDistance(matrAdjacency[currentIndex * p + i] + smallestDistance);
        vertexes[i].SetPrevVertexIndex(currentIndex);
        vertexesToCheck.push(vertexes[i]);
      }

    currentVertex.CheckVertex();
  } while (!vertexesToCheck.empty());

  if (vertexes[finalIndex].GetSmallestDistance() == infinite)
    return uncorrectMap_noWayToFinish;

  return lab_true;
}

void graphSolver::PrintDijkstraSolve(void) {
  // draw map for help
  for (int y = 0; y < mapH; y++) {
    for (int x = 0; x < mapW; x++) {
      if (x == startX && y == startY)
        std::cout << "S";
      else if (x == finishX && y == finishY)
        std::cout << "F";
      else if (map[x + y * mapW] == wall)
        std::cout << "#";
      else if (map[x + y * mapW] >= 0)
        std::cout << (char)(map[x + y * mapW] + '0');
        //std::cout << " ";
      else if (map[x + y * mapW] == empty)
        std::cout << " ";
    }
    std::cout << std::endl;
  }

  // get path and reverse it with stack
  std::stack<int> vertexIndexes;
  vertexIndexes.push(finalIndex);

  vertex currentVertex = vertexes[finalIndex], prevVertex;
  int currentIndex = finalIndex, prevIndex = currentVertex.GetPrevVertexIndex();
  prevVertex = vertexes[prevIndex];
  do {
    vertexIndexes.push(prevIndex);

    currentVertex = prevVertex, currentIndex = prevIndex;
    prevIndex = currentVertex.GetPrevVertexIndex();
    if (prevIndex != -1)
      prevVertex = vertexes[prevIndex];
  } while (prevIndex != -1);

  // output path
  int wasFirst = 1;
  while (!vertexIndexes.empty()) {
    currentIndex = vertexIndexes.top();
    vertexIndexes.pop();

    if (wasFirst == 1) {
      std::cout << "S";
      wasFirst = 0;
    }
    else {
      if (currentIndex == finalIndex)
        std::cout << " >> " << "F";
      else
        std::cout << " >> " << (char)(currentIndex + '0');
    }
  }
  std::cout << std::endl;
}

void graphSolver::InitGraphSolverParams(labirintMap& labirint) {
  // init graph
  mapW = labirint.GetW();
  mapH = labirint.GetH();
  map = labirint.GetMap();
  graphType = labirint.GetLabType();
  startX = labirint.GetStartX();
  startY = labirint.GetStartY();
  finishX = labirint.GetFinishX();
  finishY = labirint.GetFinishY();
}

int graphSolver::ParseGraph(labirintMap& labirint) {
  // init graph
  InitGraphSolverParams(labirint);

  // find solution
  if (graphType == terrariumGraph) {
    int res = SolveWaveMethod();
    if (res == lab_true) {
      BuildWavePath();
      PrintLab();
    }
    else if (res == uncorrectMap_noWayToFinish)
      std::cout << "ERROR: no way to finish" << std::endl;
  }
  else {
    InitLabirint(labirint);

    int res = SolveDijkstraMehod();
    if (res == lab_true)
      PrintDijkstraSolve();
    else if (res == uncorrectMap_noWayToFinish)
      std::cout << "ERROR: no way to finish" << std::endl;
  }

  return lab_true;
}

int graphSolver::ParseGraphOnlyWave(labirintMap& labirint) {
  // init graph
  InitGraphSolverParams(labirint);

  // find solution
  int res = SolveWaveMethod();
  if (res == lab_true) {
    BuildWavePath();
    PrintLab();
  }
  else if (res == uncorrectMap_noWayToFinish)
    std::cout << "ERROR: no way to finish" << std::endl;
  
  return lab_true;
}

graphSolver::~graphSolver() {
  delete[] matrAdjacency;
}