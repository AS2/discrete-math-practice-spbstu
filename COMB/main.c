#include <stdio.h>

#include "returnType.h"

#include "strParser.h"
#include "comb.h"

int main(void) {
  uint n = 0, m = 0, res;
  return_t parseResult, functionResult;

  InitConstants();

  while (1) {
    printf("Input command ['H' - for help]: ");
    ParseInputString(&m, &n, &parseResult);

    switch (parseResult.returnedCode) {
    // output info
    case H_RETURNS:
      printf("Programm helper. Big letter - command, 'm' and 'n' - input params (integer values)\n"
        "H - call help\n"
        "Q - quit\n"
        "U m n - number of placements\n"
        "A m n - number of placements without repetitions \n"
        "P n - number of permutations\n"
        "C m n - number of combinations\n"
        "S m n - Stirling number of the second kind\n"
        "B n - Bell number\n");
      break;

    // command
    case U_RETURNS:
      res = PlacementsNum(m, n, &functionResult);
      if (functionResult.returnedCode == TRUE)
        printf("U(%u, %u) = %u\n", m, n, res);
      else
        PrintError(functionResult);
      break;
    case A_RETURNS:
      res = PlacementsWithoutRepetitionsNum(m, n, &functionResult);
      if (functionResult.returnedCode == TRUE)
        printf("A(%u, %u) = %u\n", m, n, res);
      else
        PrintError(functionResult);
      break;
    case P_RETURNS:
      res = PermutationsNum(n, &functionResult);
      if (functionResult.returnedCode == TRUE)
        printf("P(%u) = %u\n", n, res);
      else
        PrintError(functionResult);
      break;
    case C_RETURNS:
      res = CombinationsNum(m, n, &functionResult);
      if (functionResult.returnedCode == TRUE)
        printf("C(%u, %u) = %u\n", m, n, res);
      else
        PrintError(functionResult);
      break;
    case S_RETURNS:
      res = StirlingNum(m, n, &functionResult);
      if (functionResult.returnedCode == TRUE)
        printf("S(%u, %u) = %u\n", m, n, res);
      else
        PrintError(functionResult);
      break;
    case B_RETURNS:
      res = BellsNum(n, &functionResult);
      if (functionResult.returnedCode == TRUE)
        printf("B(%u) = %u\n", n, res);
      else
        PrintError(functionResult);
      break;

    // error string
    case ERROR_BAD_INPUT_STRING:
    case ERROR_M_MORE_THAN_N:
    case ERROR_NEGATIVE_ARGUMENT:
    case ERROR_NOT_ENOUGHT_ARGS:
    case ERROR_TOO_MANY_ARGS:
    case ERROR_ARG_TOO_BIG:
      PrintError(parseResult);
      break;

    // quit
    case Q_RETURNS:
      return 0;
    }
  }
}