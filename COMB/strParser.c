#include <stdio.h>

#include "strParser.h"

#define MAX_STR_SIZE 10000

static uint maxUINT = 4294967295;

static void CheckSumLimit(uint Num1, uint Num2, return_t *returned) {
  uint maxSum = maxUINT - Num1;

  if (Num2 <= maxSum)
    returned->returnedCode = TRUE;
  else
    returned->returnedCode = FALSE;
}

static void CheckMulLimit(uint Num1, uint Num2, return_t* returned) {
  if (Num1 == 0)
    returned->returnedCode = TRUE;
  else {
    uint maxMul = maxUINT / Num1;

    if (Num2 <= maxMul)
      returned->returnedCode = TRUE;
    else
      returned->returnedCode = FALSE;
  }
}

static int ParseNumFromStr(char* str, int pos, uint* num, return_t *returned) {
  uint numRes = 0, newDigit;
  int wasNum = 0;

  return_t checkResults;

  if (str[pos] == '-') {
    returned->returnedCode = ERROR_NEGATIVE_ARGUMENT;
    return 0;
  }

  while (str[pos] != 0) {
    wasNum = 1;

    if (str[pos] >= '0' && str[pos] <= '9') {
      newDigit = (uint)(str[pos++] - '0');

      CheckMulLimit(numRes, 10, &checkResults);
      if (checkResults.returnedCode == TRUE)
        numRes *= 10;
      else {
        returned->returnedCode = ERROR_ARG_TOO_BIG;
        return 0;
      }

      CheckSumLimit(numRes, newDigit, &checkResults);
      if (checkResults.returnedCode == TRUE)
        numRes += newDigit;
      else {
        returned->returnedCode = ERROR_ARG_TOO_BIG;
        return 0;
      }
    }
    else if (str[pos] == ' ' || str[pos] == '\t') {
      *num = numRes;
      returned->returnedCode = TRUE;
      return pos;
    }
    else {
      returned->returnedCode = ERROR_BAD_INPUT_STRING;
      return 0;
    }
  }

  if (wasNum == 0) {
    returned->returnedCode = ERROR_BAD_INPUT_STRING;
    return 0;
  }

  *num = numRes;
  returned->returnedCode = TRUE;
  return pos;
}

void ParseInputString(uint* m, uint* n, return_t *returned) {
  char inputString[MAX_STR_SIZE];
  int i = 0, argsCnt = 0, newPos;
  uint num;

  gets(inputString);

  // skip all spaces and tabs
  while (inputString[i] != 0 && (inputString[i] == ' ' || inputString[i] == '\t'))
    i++;

  // if info / quit
  if (inputString[i] == 'H' || inputString[i] == 'Q') {
    // save command
    if (inputString[i] == 'H')
      returned->returnedCode = H_RETURNS;
    else
      returned->returnedCode = Q_RETURNS;
    i++;

    // skip all spaces and tabs
    while (inputString[i] != 0 && (inputString[i] == ' ' || inputString[i] == '\t'))
      i++;

    if (inputString[i] != 0)
      returned->returnedCode = ERROR_BAD_INPUT_STRING;
  }
  // if command
  else if ((inputString[i] == 'U' || inputString[i] == 'A' || inputString[i] == 'P' || 
    inputString[i] == 'C' || inputString[i] == 'S' || inputString[i] == 'B') && 
    (inputString[i + 1] == ' ' || inputString[i + 1] == '\t')) {
    
    if (inputString[i] == 'U')
      returned->returnedCode = U_RETURNS;
    else if (inputString[i] == 'A')
      returned->returnedCode = A_RETURNS;
    else if (inputString[i] == 'P')
      returned->returnedCode = P_RETURNS;
    else if (inputString[i] == 'C')
      returned->returnedCode = C_RETURNS;
    else if (inputString[i] == 'S')
      returned->returnedCode = S_RETURNS;
    else if (inputString[i] == 'B')
      returned->returnedCode = B_RETURNS;

    // skip at least one space or one tab
    i += 2;

    // read all arguments
    while (1) {
      if (inputString[i] != 0 && (inputString[i] == ' ' || inputString[i] == '\t'))
        i++;

      if (inputString[i] == 0)
        break;
      else {
        return_t parseNumResult;

        newPos = ParseNumFromStr(inputString, i, &num, &parseNumResult);

        // if bad aargument
        if (parseNumResult.returnedCode != TRUE) {
          returned->returnedCode = parseNumResult.returnedCode;
          return;
        }
        else {
          i = newPos;

          if (argsCnt == 0)
            *n = num;
          else if (argsCnt == 1 && (returned->returnedCode == B_RETURNS || returned->returnedCode == P_RETURNS)) {
            returned->returnedCode = ERROR_TOO_MANY_ARGS;
            return;
          }
          else if (argsCnt == 1) {
            *m = *n;
            *n = num;
          }
          else if (argsCnt == 2) {
            returned->returnedCode = ERROR_TOO_MANY_ARGS;
            return;
          }

          argsCnt++;
        }
      }
    }

    // check possible errors in arguments
    if (argsCnt == 0 || (argsCnt == 1 && !(returned->returnedCode == B_RETURNS || returned->returnedCode == P_RETURNS)))
      returned->returnedCode = ERROR_NOT_ENOUGHT_ARGS;
  }
  else
    returned->returnedCode = ERROR_BAD_INPUT_STRING;
}