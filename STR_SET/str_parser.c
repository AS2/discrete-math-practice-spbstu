#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str_parser.h"
#include "str_set.h"

#pragma warning(disable: 4996)

// operations
static int QueuePush(strQueue_t** Q, char* str) {
  if (StrSetCheckStr(str) != StrSetTrue)
    return StrSetFalse;

  // add new element
  strList_t* newElement = (strList_t*)malloc(sizeof(strList_t));
  if (newElement == NULL)
    return StrSetFalse;

  strcpy(newElement->str, str);
  newElement->next = NULL;

  if ((*Q)->begin == NULL && (*Q)->end == NULL) {
    (*Q)->begin = (*Q)->end = newElement;
  }
  else {
    (*Q)->end->next = newElement;
    (*Q)->end = newElement;
  }
  return StrSetTrue;
}

int QueuePop(strQueue_t** Q, char** str) {
  char* strToReturn;
  strList_t* toDelete;

  // if empty
  if ((*Q)->begin == NULL && (*Q)->end == NULL)
    return StrSetFalse;

  strToReturn = (char*)malloc(sizeof(char) * 81);

  if (strToReturn == NULL)
    return StrSetFalse;
  strcpy(strToReturn, (*Q)->begin->str);
  *str = strToReturn;

  toDelete = (*Q)->begin;
  if ((*Q)->begin == (*Q)->end)
    (*Q)->begin = (*Q)->end = NULL;
  else
    (*Q)->begin = (*Q)->begin->next;

  free(toDelete);
  return StrSetTrue;
}

int QueueIsEmpty(strQueue_t* Q) {
  if (Q->begin == NULL)
    return StrSetTrue;
  return StrSetFalse;
}

void ClearQueue(strQueue_t** Q) {
  char *str;
  while (QueueIsEmpty(*Q) == StrSetFalse)
    QueuePop(Q, &str);
}

// string parser
static int TakeSimpleStrFromBuff(char* buff, char** toReturn, int pos) {
  int i = pos, length = 0;
  char* strToReturn = (char*)malloc(sizeof(char) * 81);

  if (strToReturn == NULL)
    return StrSetFalse;

  // copy string while don't go to limit or end or space
  while (buff[i] != 0 && buff[i] != ' ' && buff[i] != '/t' && buff[i] != '\"' && length < 80)
    strToReturn[i - pos] = buff[i++], length++;
  strToReturn[i - pos] = 0;

  // if go to linit and str dint read to end -> leave with error
  if (length == 80 && buff[i] != 0 && !(buff[i + 1] == ' ' || buff[i + 1] == '/t' || buff[i + 1] == '\"')) {
    free(strToReturn);
    return StrSetFalse;
  }

  if (buff[i] != 0 && buff[i] != '\"')
    i++;
  *toReturn = strToReturn;
  return i;
}

static int TakeHardStrFromBuff(char* buff, char** toReturn, int pos) {
  int i = pos, length = 0;
  char* strToReturn = (char*)malloc(sizeof(char) * 81);

  if (strToReturn == NULL)
    return StrSetFalse;

  if (buff[i] != '\"')
    return StrSetFalse;
  else
    i++;

  // copy string while don't go to limit or end or '"'
  while (buff[i] != 0 && buff[i] != '"' && length < 80)
    strToReturn[i - pos - 1] = buff[i++], length++;

  // if read nothing or go to linit and str dint read to end -> leave with error
  if (length == 0 || (length == 80 && buff[i] != '"')) {
    free(strToReturn);
    return StrSetFalse;
  }

  if (buff[i] == '"') {
    strToReturn[i - pos - 1] = 0;
    i++;
    *toReturn = strToReturn;
    return i;
  }
  else {
    free(strToReturn);
    return StrSetFalse;
  }
}

int ParseString(strQueue_t** Q, char* buff) {
  int i = 0, lastPos;
  char* strToQueue;

  while (buff[i] != 0) {
    // read complex string
    if (buff[i] == '\"') {
      lastPos = TakeHardStrFromBuff(buff, &strToQueue, i);
      if (lastPos < 0)
        return StrSetFalse;
      QueuePush(Q, strToQueue);
      i = lastPos;

      free(strToQueue);
    }
    // skip spaces and tabs
    else if (buff[i] == ' ' || buff[i] == '\t')
      i++;
    // read one-word string
    else {
      lastPos = TakeSimpleStrFromBuff(buff, &strToQueue, i);
      if (lastPos < 0)
        return StrSetFalse;
      QueuePush(Q, strToQueue);
      i = lastPos;

      free(strToQueue);
    }
  }
  return StrSetTrue;
}
