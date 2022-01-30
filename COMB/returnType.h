#ifndef __RETURNTYPE_H__
#define __RETURNTYPE_H__

typedef unsigned int uint;

enum returnEnum {
  // logic returns
  TRUE = -1,
  FALSE = -2,

  // info/exit returns
  H_RETURNS = -3,
  Q_RETURNS = -4,

  // commands returns type
  U_RETURNS = -5,
  A_RETURNS = -6,
  P_RETURNS = -7,
  C_RETURNS = -8,
  S_RETURNS = -9,
  B_RETURNS = -10,

  // errors returns type
  ERROR_BAD_INPUT_STRING = -11,
  ERROR_NEGATIVE_ARGUMENT = -12,
  ERROR_M_MORE_THAN_N = -13,
  ERROR_NUM_TOO_BIG = -14,                  // 0 - because funcs returns not negative numers
  ERROR_NOT_ENOUGHT_ARGS = -15,
  ERROR_TOO_MANY_ARGS = -16,
  ERROR_ARG_TOO_BIG = -17
};

typedef struct return_t {
  int returnedCode;
} return_t;

void PrintError(return_t returned);

#endif