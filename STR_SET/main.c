#include <stdio.h>
#include <string.h>

#include "str_set.h"
#include "str_parser.h"

#pragma warning(disable: 4996)

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

char buff[100000];

int ReadNumber(void) {
  char* ptrend;
  double res;
  unsigned int i;

  gets(buff);

  for (i = 0; i < strlen(buff); i++)
    if (buff[i] < '0' || buff[i] > '9')
      return SetStrUncorrectAnswer;

  res = strtod(buff, &ptrend);

  return (int)res;
}

int main(void) {
  int result, id1, id2,
    choice;
  char* currentString;
  strQueue_t* strQueue = (strQueue_t *)malloc(sizeof(strQueue_t));
  strQueue->begin = strQueue->end = NULL;

  while (1) {
    printf("\n-= MENU =-\n"
      "[1] - create new set\n"
      "[2] - delete set by id\n"
      "[3] - output set power\n"
      "[4] - input element(-s) in set\n"
      "[5] - find element(-s) in set\n"
      "[6] - delete element(-s) from set\n"
      "[7] - union sets\n"
      "[8] - intersection sets\n"
      "[9] - difference sets\n"
      "[10] - symm. diff. sets\n"
      "[11] - check one set in another\n"
      "[12] - output all sets\n"
      "[13] - output set content\n"
      "[14] - exit\n");
    choice = ReadNumber();

    if (choice == SetStrUncorrectAnswer) {
      PrintError(SetStrUncorrectAnswer);
      continue;
    }

    //ClearQueue(&strQueue);

    switch (choice) {
    case 1:
      result = StrSetConstruct();
      if (result > StrSetTrue)
        printf("New set created with ID %i!\n", result);
      else
        PrintError(result);
      break;

    case 2:
      printf("Input set ID: ");
      id1 = ReadNumber();
      if (id1 == SetStrUncorrectAnswer) {
        PrintError(SetStrUncorrectAnswer);
        break;
      }

      result = StrSetDestruct(id1);
      if (result == StrSetTrue)
        printf("Set with ID %i was deleted!\n", id1);
      else
        PrintError(result);
      break;

    case 3:
      printf("Input set ID: ");
      id1 = ReadNumber();
      if (id1 == SetStrUncorrectAnswer) {
        PrintError(SetStrUncorrectAnswer);
        break;
      }

      result = StrSetOutputPower(id1);
      if (result != StrSetTrue)
        PrintError(result);
      break;

    case 4:
      printf("Input set ID: ");
      id1 = ReadNumber();
      if (id1 == SetStrUncorrectAnswer) {
        PrintError(SetStrUncorrectAnswer);
        break;
      }
      if (StrSetCheckIdInList(id1) != StrSetTrue) {
        PrintError(StrSetNoSetInList);
        break;
      }

      printf("Input elements separated by a space (elements with spaces put \"in double-quotes\"!):\n");
      gets(buff);

      result = StrSetCheckBuff(buff);
      if (result == StrSetTrue) {
        if (ParseString(&strQueue, buff) != StrSetTrue) {
          printf("ERROR: can't parse this string. Check right count of quotes or content in them.");
          break;
        }

        while (QueueIsEmpty(strQueue) == StrSetFalse) {
          QueuePop(&strQueue, &currentString);

          printf("Current string - \"%s\": ", currentString);
          result = StrSetAddElement(id1, currentString);
          if (result != StrSetTrue)
            PrintError(result);
          else
            printf("added!\n");

          free(currentString);
        }
      }
      else
        PrintError(result);
      break;

    case 5:
      printf("Input set ID: ");
      id1 = ReadNumber();
      if (id1 == SetStrUncorrectAnswer) {
        PrintError(SetStrUncorrectAnswer);
        break;
      }
      if (StrSetCheckIdInList(id1) != StrSetTrue) {
        PrintError(StrSetNoSetInList);
        break;
      }

      printf("Input elements separated by a space (elements with spaces put \"in double-quotes\"!):\n");
      gets(buff);

      result = StrSetCheckBuff(buff);
      if (result == StrSetTrue) {
        if (ParseString(&strQueue, buff) != StrSetTrue) {
          printf("ERROR: can't parse this string. Check right count of quotes or content in them.");
          break;
        }

        while (QueueIsEmpty(strQueue) == StrSetFalse) {
          QueuePop(&strQueue, &currentString);

          printf("Current string - \"%s\": ", currentString);
          result = StrSetFindElement(id1, currentString);
          if (result == StrSetTrue)
            printf("founded!\n");
          else if (result == StrSetFalse)
            printf("NOT founded!\n");
          else
            PrintError(result);

          free(currentString);
        }
      }
      else
        PrintError(result);
      break;

    case 6:
      printf("Input set ID: ");
      id1 = ReadNumber();
      if (id1 == SetStrUncorrectAnswer) {
        PrintError(SetStrUncorrectAnswer);
        break;
      }
      if (StrSetCheckIdInList(id1) != StrSetTrue) {
        PrintError(StrSetNoSetInList);
        break;
      }

      printf("Input elements separated by a space (elements with spaces put \"in double-quotes\"!):\n");
      gets(buff);

      result = StrSetCheckBuff(buff);
      if (result == StrSetTrue) {
        if (ParseString(&strQueue, buff) != StrSetTrue) {
          printf("ERROR: can't parse this string. Check right count of quotes or content in them.");
          break;
        }

        while (QueueIsEmpty(strQueue) == StrSetFalse) {
          QueuePop(&strQueue, &currentString);
          
          printf("Current string - \"%s\": ", currentString);
          result = StrSetDeleteElement(id1, currentString);
          if (result == StrSetTrue)
            printf("deleted!\n");
          else if (result == StrSetFalse)
            printf("NOT founded!\n");
          else
            PrintError(result);

          free(currentString);
        }
      }
      else
        PrintError(result);
      break;

    case 7:
      printf("Input first set ID: ");
      id1 = ReadNumber();
      printf("Input second set ID: ");
      id2 = ReadNumber();

      if (id1 == SetStrUncorrectAnswer || id2 == SetStrUncorrectAnswer) {
        PrintError(SetStrUncorrectAnswer);
        break;
      }

      result = StrSetUnion(id1, id2);
      if (result > StrSetTrue)
        printf("Union complited, created new result-set with id %i\n", result);
      else
        PrintError(result);
      break;

    case 8:
      printf("Input first set ID: ");
      id1 = ReadNumber();
      printf("Input second set ID: ");
      id2 = ReadNumber();

      if (id1 == SetStrUncorrectAnswer || id2 == SetStrUncorrectAnswer) {
        PrintError(SetStrUncorrectAnswer);
        break;
      }

      result = StrSetIntersection(id1, id2);
      if (result > StrSetTrue)
        printf("Intersection complited, created new result-set with id %i\n", result);
      else
        PrintError(result);
      break;

    case 9:
      printf("Input first set ID: ");
      id1 = ReadNumber();
      printf("Input second set ID: ");
      id2 = ReadNumber();

      if (id1 == SetStrUncorrectAnswer || id2 == SetStrUncorrectAnswer) {
        PrintError(SetStrUncorrectAnswer);
        break;
      }

      result = StrSetDifference(id1, id2);
      if (result > StrSetTrue)
        printf("Difference complited, created new result-set with id %i\n", result);
      else
        PrintError(result);
      break;

    case 10:
      printf("Input first set ID: ");
      id1 = ReadNumber();
      printf("Input second set ID: ");
      id2 = ReadNumber();

      if (id1 == SetStrUncorrectAnswer || id2 == SetStrUncorrectAnswer) {
        PrintError(SetStrUncorrectAnswer);
        break;
      }

      result = StrSetSymmetricDifference(id1, id2);
      if (result > StrSetTrue)
        printf("Symmetric difference complited, created new result-set with id %i\n", result);
      else
        PrintError(result);
      break;

    case 11:
      printf("Input inner set ID: ");
      id1 = ReadNumber();
      printf("Input outer set ID: ");
      id2 = ReadNumber();

      if (id1 == SetStrUncorrectAnswer || id2 == SetStrUncorrectAnswer) {
        PrintError(SetStrUncorrectAnswer);
        break;
      }

      result = StrSetCheckInner(id1, id2);
      if (result == StrSetTrue)
        printf("Set %i is inner to set %i\n", id1, id2);
      else if (result == StrSetFalse)
        printf("Set %i is NOT inner to set %i\n", id1, id2);
      else
        PrintError(result);
      break;

    case 12:
      StrSetSetsOutput();
      break;

    case 13:
      printf("Input set ID: ");
      id1 = ReadNumber();

      if (id1 == SetStrUncorrectAnswer) {
        PrintError(SetStrUncorrectAnswer);
        break;
      }

      result = StrSetOneSetOutput(id1);
      if (result != StrSetTrue)
        PrintError(result);
      break;

    case 14:
      ClearAllSets();
      free(strQueue);
      _CrtDumpMemoryLeaks();
      return 0;

    default:
      printf("Uncorrect choice, try again\n");
      break;
    }
  }

  _CrtDumpMemoryLeaks();
  return 0;
}