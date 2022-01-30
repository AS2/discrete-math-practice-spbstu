#include <stdio.h>

#include "comb.h"

static uint maxUINT = 4294967295;
static uint maxMCombForSomeN[] = { 92682, 2954, 568, 222, 123, 83, 53, 52, 46, 42, 39, 37, 36, 35, 35 };
static uint maxMStirlingForSomeN[] = { 33, 21, 18, 16, 16, 16, 16, 16, 17, 17, 18, 19, 19, 20, 21, 
                                       22, 23, 24, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };

static void CheckSumLimit(uint Num1, uint Num2, return_t* returned) {
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

void InitConstants( void ) {
  // init factorials
  factorials[0] = factorials[1] = 1;
  for (int i = 2; i < 13; i++)
    factorials[i] = (uint)i * factorials[i - 1];

  // init pascal triangle
  pascalTriangle[0][0] = 1;
  for (int i = 1; i < 35; i++)
    for (int j = 0; j <= i; j++) {
      if (j == 0)
        pascalTriangle[i][j] = pascalTriangle[i - 1][j];
      else
        pascalTriangle[i][j] = pascalTriangle[i - 1][j] + pascalTriangle[i - 1][j - 1];
    }

  // init stirling table
  stirlingTable[0][0] = 1;
  for (uint i = 1; i < 17; i++)
    for (uint j = 0; j <= i; j++) {
      if (j == 0)
        stirlingTable[i][j] = 0;
      else
        stirlingTable[i][j] = j * stirlingTable[i - 1][j] + stirlingTable[i - 1][j - 1];
    }

  // init bells nums
  for (uint i = 0; i < 16; i++)
    for (uint j = 0; j <= i; j++)
      bellNumSeries[i] += stirlingTable[i][j];
}

uint CombinationsNum(uint m, uint n, return_t* funcResult) {
  if (n > m) {
    funcResult->returnedCode = TRUE;
    return 0;
  }
  else if (n > m / 2)
    n = m - n;

  // if we know that result will be too big
  if ((n >= 2 && n <= 16 && m > maxMCombForSomeN[n - 2]) || (n > 16 && m > 34)) {
    funcResult->returnedCode = ERROR_NUM_TOO_BIG;
    return 0;
  }

  // if in pascal triangle table
  if (m < 35 && n >= 0) {
    funcResult->returnedCode = TRUE;
    return pascalTriangle[m][n];
  }
  // if answer is obvious
  else if (m < 35 && n < 0) {
    funcResult->returnedCode = TRUE;
    return 0;
  }
  else if (n == 0) {
    funcResult->returnedCode = TRUE;
    return 1;
  }
  else if (n == 1) {
    funcResult->returnedCode = TRUE;
    return m;
  }
  else if (n == 2) {
    uint res = m - 1;
    return_t checkRes;

    if (m % 2 == 0)
      m = m / 2;
    else
      res = res / 2;

    CheckMulLimit(res, m, &checkRes);
    if (checkRes.returnedCode == TRUE) {
      funcResult->returnedCode = TRUE;
      return res * m;
    }
    else {
      funcResult->returnedCode = ERROR_NUM_TOO_BIG;
      return res * m;
    }
  }
  // if not obvious: count
  else {
    return_t combRes, comb2Res, checkRes;
    uint sum = CombinationsNum(m - 1, n - 1, &combRes), sum2;

    if (m % n == 0)
      m /= n;
    else if (sum % n == 0)
      sum /= n;
    else {
      sum2 = CombinationsNum(m - 1, n, &comb2Res);
      if (combRes.returnedCode != TRUE || comb2Res.returnedCode != TRUE) {
        funcResult->returnedCode = ERROR_NUM_TOO_BIG;
        return 0;
      }
      else {
        CheckSumLimit(sum, sum2, &checkRes);
        if (checkRes.returnedCode == TRUE) {
          funcResult->returnedCode = TRUE;
          return sum + sum2;
        }
        else {
          funcResult->returnedCode = checkRes.returnedCode;
          return 0;
        }
      }
    }

    if (combRes.returnedCode != TRUE) {
      funcResult->returnedCode = combRes.returnedCode;
      return 0;
    }
    else {
      CheckMulLimit(sum, m, &checkRes);
      if (checkRes.returnedCode == TRUE) {
        funcResult->returnedCode = TRUE;
        return sum * m;
      }
      else {
        funcResult->returnedCode = checkRes.returnedCode;
        return 0;
      }
    }
  }
}

uint PlacementsNum(uint m, uint n, return_t* funcResult) {
  uint res = 1;
  return_t checkRes;

  for (uint i = 0; i < n; i++) {
    CheckMulLimit(res, m, &checkRes);
    if (checkRes.returnedCode == TRUE)
      res *= m;
    else {
      funcResult->returnedCode = ERROR_NUM_TOO_BIG;
      return 0;
    }
  }
  funcResult->returnedCode = TRUE;
  return res;
}

uint PlacementsWithoutRepetitionsNum(uint m, uint n, return_t* funcResult) {
  if (n == 0) {
    funcResult->returnedCode = TRUE;
    return 1;
  }
  else if (n > m) {
    funcResult->returnedCode = TRUE;
    return 0;
  }
  else if (n > 12) {
    funcResult->returnedCode = ERROR_NUM_TOO_BIG;
    return 0;
  }
  else {
    return_t combRes, checkRes;
    uint combNum = CombinationsNum(m, n, &combRes);

    if (combRes.returnedCode == ERROR_NUM_TOO_BIG) {
      funcResult->returnedCode = ERROR_NUM_TOO_BIG;
      return 0;
    }
    else {
      CheckMulLimit(combNum, factorials[n], &checkRes);
      if (checkRes.returnedCode == TRUE) {
        funcResult->returnedCode = TRUE;
        return combNum * factorials[n];
      }
      else {
        funcResult->returnedCode = ERROR_NUM_TOO_BIG;
        return 0;
      }
    }
  }
}

uint PermutationsNum(uint m, return_t* funcResult) {
  // if too big
  if (m > 12) {
    funcResult->returnedCode = ERROR_NUM_TOO_BIG;
    return 0;
  }
  else {
    funcResult->returnedCode = TRUE;
    return factorials[m];
  }
}

uint StirlingNum(uint m, uint n, return_t* funcResult) {
  // if res is obvious
  if (m == n || n == 1) {
    funcResult->returnedCode = TRUE;
    return 1;
  }
  else if (n == 0) {
    funcResult->returnedCode = TRUE;
    return 0;
  }
  else if (n > m) {
    funcResult->returnedCode = TRUE;
    return 0;
  }
  else if (m - n == 1) {
    uint res = 0;
    return_t sumReturn;

    for (uint i = 1; i <= n; i++) {
      CheckSumLimit(res, i, &sumReturn);
      if (sumReturn.returnedCode == TRUE)
        res = res + i;
      else {
        funcResult->returnedCode = ERROR_NUM_TOO_BIG;
        return 0;
      }
    }

    funcResult->returnedCode = TRUE;
    return res;
  }
  // if we can say that result would be too big (founded in experiments)
  else if (n >= 2 && n < 32 && m > maxMStirlingForSomeN[n - 2]) {
    funcResult->returnedCode = ERROR_NUM_TOO_BIG;
    return 0;
  }
  else if (n >= 32 && n < 76 && m - n > 4) {
    funcResult->returnedCode = ERROR_NUM_TOO_BIG;
    return 0;
  }
  else if (n >= 76 && n < 430 && m - n > 2) {
    funcResult->returnedCode = ERROR_NUM_TOO_BIG;
    return 0;
  }
  else if (n >= 430 && n < 92682 && m - n > 1) {
    funcResult->returnedCode = ERROR_NUM_TOO_BIG;
    return 0;
  }
  else if (n >= 92682 && m > n) {
    funcResult->returnedCode = ERROR_NUM_TOO_BIG;
    return 0;
  }
  // if in table
  else if (m < 17) {
    funcResult->returnedCode = TRUE;
    return stirlingTable[m][n];
  }
  // count num
  else {
    return_t stirReturn1, stirReturn2, checkRes;
    uint stirling1 = StirlingNum(m - 1, n - 1, &stirReturn1), 
      stirling2 = StirlingNum(m - 1, n, &stirReturn2),
      mul;

    if (stirReturn1.returnedCode != TRUE || stirReturn2.returnedCode != TRUE) {
      funcResult->returnedCode = ERROR_NUM_TOO_BIG;
      return 0;
    }

    CheckMulLimit(stirling2, n, &checkRes);
    if (checkRes.returnedCode == TRUE)
      mul = stirling2 * n;
    else {
      funcResult->returnedCode = ERROR_NUM_TOO_BIG;
      return 0;
    }

    CheckSumLimit(mul, stirling1, &checkRes);
    if (checkRes.returnedCode == TRUE) {
      funcResult->returnedCode = TRUE;
      return mul + stirling1;
    }
    else {
      funcResult->returnedCode = ERROR_NUM_TOO_BIG;
      return 0;
    }
  }
}

uint BellsNum(uint m, return_t* funcResult) {
  // if too big
  if (m >= 16) {
    funcResult->returnedCode = ERROR_NUM_TOO_BIG;
    return 0;
  }
  else {
    funcResult->returnedCode = TRUE;
    return bellNumSeries[m];
  }
}
