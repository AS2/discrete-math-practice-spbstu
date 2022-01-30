#ifndef __COMBIN_H__
#define __COMBIN_H__

#include "returnType.h"

uint factorials[13], pascalTriangle[35][35], stirlingTable[17][17];
uint bellNumSeries[16];

void InitConstants(void);

uint PlacementsNum(uint m, uint n, return_t* funcResult);
uint PlacementsWithoutRepetitionsNum(uint m, uint n, return_t* funcResult);
uint PermutationsNum(uint m, return_t* funcResult);
uint CombinationsNum(uint m, uint n, return_t* funcResult);
uint StirlingNum(uint m, uint n, return_t* funcResult);
uint BellsNum(uint m, return_t *funcResult);

#endif
