#include <stdio.h>

#include "returnType.h"

void PrintError(return_t returned) {
  switch (returned.returnedCode) {
  case ERROR_BAD_INPUT_STRING:
    printf("ERROR - bad input string\n");
    break;
  case ERROR_NEGATIVE_ARGUMENT:
    printf("ERROR - arguments can't be negative\n");
    break;
  case ERROR_NUM_TOO_BIG:
    printf("ERROR - too big result\n");
    break;
  case ERROR_ARG_TOO_BIG:
    printf("ERROR - argument is too big [max value: 4294967295]\n");
    break;
  case ERROR_NOT_ENOUGHT_ARGS:
    printf("ERROR - not enought arguments\n");
    break;
  case ERROR_TOO_MANY_ARGS:
    printf("ERROR - too many arguments\n");
    break;
  case ERROR_M_MORE_THAN_N:
    printf("ERROR - second argument must be more than first\n");
    break;
  default:
    break;
  }
}