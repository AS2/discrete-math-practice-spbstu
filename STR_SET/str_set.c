#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "str_set.h"

#pragma warning (disable: 4996)

// total list of sets
struct {
  sets_t* begin;
  sets_t* end;
} allSetsList;

int lastId = 1;

int StrSetConstruct(void) {
  // make new set
  strSet_t* set = (strSet_t*)malloc(sizeof(strSet_t));
  if (set == NULL)
    return StrSetNoMemory;

  set->setId = lastId;
  set->begin = NULL;
  set->setPower = 0;

  //update ID
  lastId++;

  // update sets list
  sets_t* newSet = (sets_t*)malloc(sizeof(sets_t));
  if (newSet == NULL)
    return StrSetNoMemory;

  newSet->currentSet = set;
  newSet->next = NULL;

  if (allSetsList.begin == NULL && allSetsList.end == NULL) {
    newSet->prev = NULL;
    allSetsList.begin = allSetsList.end = newSet;
  }
  else {
    allSetsList.end->next = newSet;
    newSet->prev = allSetsList.end;
    allSetsList.end = newSet;
  }

  return set->setId;
}


// destruct
static int FoundSetInList(int setId, sets_t** setToReturn) {
  int isFound = 0;
  sets_t* setsPtr = allSetsList.begin;

  while (setsPtr != NULL) {
    if (setsPtr->currentSet->setId < setId)
      setsPtr = setsPtr->next;
    else if (setsPtr->currentSet->setId == setId) {
      isFound = 1;
      *setToReturn = setsPtr;
      break;
    }
    else
      break;
  }

  return isFound;
}

static void ClearList(list_t** list) {
  list_t* elementToDelete;

  while ((*list) != NULL) {
    elementToDelete = *list;
    *list = (*list)->next;
    free(elementToDelete);
  }
}

int StrSetDestruct(int setId) {
  // find set in list of sets: if founded - clear list and delete them from list, else - return 0
  sets_t* setsPtr;

  if (!FoundSetInList(setId, &setsPtr))
    return StrSetNoSetInList;

  // clear set list
  if (setsPtr->currentSet->setPower > 0)
    ClearList(&(setsPtr->currentSet->begin));
  
  // delete set from sets list
  if (setsPtr->next != NULL && setsPtr->prev != NULL) {
    setsPtr->next->prev = setsPtr->prev;
    setsPtr->prev->next = setsPtr->next;
  }
  else if (setsPtr->next != NULL && setsPtr->prev == NULL) {
    allSetsList.begin = setsPtr->next;
    setsPtr->next->prev = NULL;
  }
  else if (setsPtr->next == NULL && setsPtr->prev != NULL) {
    allSetsList.end = setsPtr->prev;
    setsPtr->prev->next = NULL;
  }
  else {
    allSetsList.begin = allSetsList.end = NULL;
  }
  free(setsPtr->currentSet);
  free(setsPtr);

  return StrSetTrue;
}

// methods
int StrSetOutputPower(int setId) {
  sets_t* setsPtr;
  if (!FoundSetInList(setId, &setsPtr))
    return StrSetNoSetInList;

  printf("Power of %i set is %i\n", setsPtr->currentSet->setId, setsPtr->currentSet->setPower);
  return StrSetTrue;
}

int StrSetAddElement(int setId, char* newStr) {
  // check set in sets list
  sets_t* setsPtr;
  if (!FoundSetInList(setId, &setsPtr))
    return StrSetNoSetInList;

  list_t* currentElement = setsPtr->currentSet->begin;
  list_t* newElement = (list_t*)malloc(sizeof(list_t));

  if (newElement == NULL)
    return StrSetNoMemory;
  strcpy(newElement->str, newStr);

  if (currentElement == NULL) {
    newElement->next = newElement->prev = NULL;
    setsPtr->currentSet->begin = newElement;
    setsPtr->currentSet->setPower++;
    return StrSetTrue;
  }

  int strcmpRes = strcmp(currentElement->str, newStr);
  // if new str lower than first element
  if (strcmpRes > 0) {
    newElement->prev = NULL;
    newElement->next = setsPtr->currentSet->begin;
    setsPtr->currentSet->begin->prev = newElement;
    setsPtr->currentSet->begin = newElement;
    setsPtr->currentSet->setPower++;
    return StrSetTrue;
  }
  else if (strcmpRes == 0) {
    free(newElement);
    return SetStrAlreadyExist;
  }

  // if new str bigger than first
  while (currentElement->next != NULL) {
    strcmpRes = strcmp(currentElement->next->str, newStr);

    // alreasy exist in set
    if (strcmpRes == 0) {
      free(newElement);
      return SetStrAlreadyExist;
    }
    else if (strcmpRes < 0)
      currentElement = currentElement->next;
    else if (strcmpRes > 0)
      break;
  }

  // add new element in list
  if (currentElement->next != NULL) {
    newElement->next = currentElement->next;
    currentElement->next->prev = newElement;
  }
  else
    newElement->next = NULL;

  newElement->prev = currentElement;
  currentElement->next = newElement;

  // update power
  setsPtr->currentSet->setPower++;
  return StrSetTrue;
}

int StrSetFindElement(int setId, char* strFind) {
  // check set in sets list
  sets_t* setsPtr;
  if (!FoundSetInList(setId, &setsPtr))
    return StrSetNoSetInList;

  list_t* currentElement = setsPtr->currentSet->begin;

  if (currentElement == NULL)
    return StrSetFalse;

  int strcmpRes;
  while (currentElement != NULL) {
    strcmpRes = strcmp(currentElement->str, strFind);

    if (strcmpRes == 0)
      return StrSetTrue;
    else if (strcmpRes < 0)
      currentElement = currentElement->next;
    else if (strcmpRes > 0)
      return StrSetFalse;
  }

  return StrSetFalse;
}

int StrSetDeleteElement(int setId, char* strToDelete) {
  // check set in sets list
  sets_t* setsPtr;
  if (!FoundSetInList(setId, &setsPtr))
    return StrSetNoSetInList;

  list_t* currentElement = setsPtr->currentSet->begin;

  if (currentElement == NULL)
    return StrSetFalse;

  int strcmpRes;
  while (currentElement != NULL) {
    strcmpRes = strcmp(currentElement->str, strToDelete);

    if (strcmpRes == 0)
      break;
    else if (strcmpRes < 0)
      currentElement = currentElement->next;
    else if (strcmpRes > 0)
      return StrSetFalse;
  }

  // if went to the end of list and there is no match with element -> leave with error
  if (currentElement == NULL)
    return StrSetFalse;

  // delete element from list
  list_t* prev = currentElement->prev, * next = currentElement->next;
  
  if (prev != NULL && next != NULL) {
    prev->next = next;
    next->prev = prev;
  }
  else if (prev != NULL && next == NULL) {
    prev->next = NULL;
  }
  // delete from begin
  else if (prev == NULL && next != NULL) {
    next->prev = NULL;
    setsPtr->currentSet->begin = next;
  }
  else {
    setsPtr->currentSet->begin = NULL;
  }

  free(currentElement);
  setsPtr->currentSet->setPower--;
  return StrSetTrue;
}

/* 
 * operations
 */

static int StrSetAddListInEnd(sets_t* set, char* elementToAdd, list_t** listEnd) {
  list_t* newElement = (list_t*)malloc(sizeof(list_t));
  if (newElement == NULL)
    return StrSetNoMemory;

  strcpy(newElement->str, elementToAdd);
  newElement->next = NULL;

  if (set->currentSet->begin == NULL) {
    set->currentSet->begin = newElement;
    newElement->prev = NULL;
  }
  else {
    (*listEnd)->next = newElement;
    newElement->prev = *listEnd;
  }

  set->currentSet->setPower++;
  *listEnd = newElement;
  return StrSetTrue;
}

int StrSetUnion(int firstSetId, int secondSetId) {
  sets_t* firstSet, *secondSet;
  if (!FoundSetInList(firstSetId, &firstSet) || !FoundSetInList(secondSetId, &secondSet))
    return StrSetNoSetInList;

  int newStrSetId = StrSetConstruct();
  if (newStrSetId < 0)
    return newStrSetId;
  
  // save new set
  sets_t* newSet;
  FoundSetInList(newStrSetId, &newSet);

  // merge two sets in one union set
  list_t
    * firstPtr = firstSet->currentSet->begin,
    * secondPtr = secondSet->currentSet->begin,
    * newSetEnd = NULL;
  int strcmpRes;
  while (firstPtr != NULL && secondPtr != NULL) {
    strcmpRes = strcmp(firstPtr->str, secondPtr->str);

    // if same elements: add one and shift both sets
    if (strcmpRes == 0) {
      StrSetAddListInEnd(newSet, firstPtr->str, &newSetEnd);
      firstPtr = firstPtr->next;
      secondPtr = secondPtr->next;
    }
    // if element in first set less than in second: add this and shift first set
    else if (strcmpRes < 0) {
      StrSetAddListInEnd(newSet, firstPtr->str, &newSetEnd);
      firstPtr = firstPtr->next;
    }
    // if element in second set less than in first: add this and shift second set
    else {
      StrSetAddListInEnd(newSet, secondPtr->str, &newSetEnd);
      secondPtr = secondPtr->next;
    }
  }
  // add remain elements from first set
  while (firstPtr != NULL) {
    StrSetAddListInEnd(newSet, firstPtr->str, &newSetEnd);
    firstPtr = firstPtr->next;
  }

  // add remain elements from second set
  while (secondPtr != NULL) {
    StrSetAddListInEnd(newSet, secondPtr->str, &newSetEnd);
    secondPtr = secondPtr->next;
  }

  return newStrSetId;
}

int StrSetIntersection(int firstSetId, int secondSetId) {
  sets_t* firstSet, * secondSet;
  if (!FoundSetInList(firstSetId, &firstSet) || !FoundSetInList(secondSetId, &secondSet))
    return StrSetNoSetInList;

  int newStrSetId = StrSetConstruct();
  if (newStrSetId < 0)
    return newStrSetId;
  sets_t* newSet;
  FoundSetInList(newStrSetId, &newSet);

  // merge two sets in one union set
  list_t
    * firstPtr = firstSet->currentSet->begin,
    * secondPtr = secondSet->currentSet->begin, 
    *newSetEnd = NULL;
  int strcmpRes;
  while (firstPtr != NULL && secondPtr != NULL) {
    strcmpRes = strcmp(firstPtr->str, secondPtr->str);

    // if same elements: add one and shift both sets
    if (strcmpRes == 0) {
      StrSetAddListInEnd(newSet, firstPtr->str, &newSetEnd);
      firstPtr = firstPtr->next;
      secondPtr = secondPtr->next;
    }
    // if element in first set less than in second: shift first set
    else if (strcmpRes < 0)
      firstPtr = firstPtr->next;
    // if element in first set less than in second: shift second set
    else 
      secondPtr = secondPtr->next;
  }

  return newStrSetId;
}

int StrSetDifference(int firstSetId, int secondSetId) {
  sets_t* firstSet, *secondSet;
  if (!FoundSetInList(firstSetId, &firstSet) || !FoundSetInList(secondSetId, &secondSet))
    return StrSetNoSetInList;

  int newStrSetId = StrSetConstruct();
  if (newStrSetId < 0)
    return newStrSetId;
  sets_t* newSet;
  FoundSetInList(newStrSetId, &newSet);

  // merge two sets in one union set
  list_t
    * firstPtr = firstSet->currentSet->begin,
    * secondPtr = secondSet->currentSet->begin,
    *newSetEnd = NULL;
  int strcmpRes;
  while (firstPtr != NULL && secondPtr != NULL) {
    strcmpRes = strcmp(firstPtr->str, secondPtr->str);

    // if same elements: shift both sets
    if (strcmpRes == 0) {
      firstPtr = firstPtr->next;
      secondPtr = secondPtr->next;
    }
    // if element in first set less than in second: add this element and shift first set
    else if (strcmpRes < 0) {
      StrSetAddListInEnd(newSet, firstPtr->str, &newSetEnd);
      firstPtr = firstPtr->next;
    }
    // if element in second set less than in first: shift second set
    else
      secondPtr = secondPtr->next;
  }

  // add remain elements from first set
  while (firstPtr != NULL) {
    StrSetAddListInEnd(newSet, firstPtr->str, &newSetEnd);
    firstPtr = firstPtr->next;
  }

  return newStrSetId;
}

int StrSetSymmetricDifference(int firstSetId, int secondSetId) {
  sets_t* firstSet, *secondSet;
  if (!FoundSetInList(firstSetId, &firstSet) || !FoundSetInList(secondSetId, &secondSet))
    return StrSetNoSetInList;

  int newStrSetId = StrSetConstruct();
  if (newStrSetId < 0)
    return newStrSetId; // <-- contain error of construction new set
  sets_t* newSet;
  FoundSetInList(newStrSetId, &newSet);

  // merge two sets in one union set
  list_t
    * firstPtr = firstSet->currentSet->begin,
    * secondPtr = secondSet->currentSet->begin, 
    *newSetEnd = NULL;
  int strcmpRes;
  while (firstPtr != NULL && secondPtr != NULL) {
    strcmpRes = strcmp(firstPtr->str, secondPtr->str);

    // if same elements: shift both sets
    if (strcmpRes == 0) {
      firstPtr = firstPtr->next;
      secondPtr = secondPtr->next;
    }
    // if element in first set less than in second: add this element and shift first set
    else if (strcmpRes < 0) {
      StrSetAddListInEnd(newSet, firstPtr->str, &newSetEnd);
      firstPtr = firstPtr->next;
    }
    // if element in second set less than in first: add this element shift second set
    else {
      StrSetAddListInEnd(newSet, secondPtr->str, &newSetEnd);
      secondPtr = secondPtr->next;
    }
  }

  // add remain elements from first set
  while (firstPtr != NULL) {
    StrSetAddListInEnd(newSet, firstPtr->str, &newSetEnd);
    firstPtr = firstPtr->next;
  }

  // add remain elements from second set
  while (secondPtr != NULL) {
    StrSetAddListInEnd(newSet, secondPtr->str, &newSetEnd);
    secondPtr = secondPtr->next;
  }

  return newStrSetId;
}

int StrSetCheckInner(int innerSetId, int outerSetId) {
  // check first & second sets in list
  sets_t* innerSet, *outerSet;
  if (!FoundSetInList(innerSetId, &innerSet) || !FoundSetInList(outerSetId, &outerSet))
    return StrSetNoSetInList;

  // merge two sets in one union set
  list_t
    * innerPtr = innerSet->currentSet->begin,
    * outerPtr = outerSet->currentSet->begin;
  int strcmpRes;
  while (innerPtr != NULL && outerPtr != NULL) {
    strcmpRes = strcmp(innerPtr->str, outerPtr->str);

    // if same elements: shift both sets
    if (strcmpRes == 0) {
      innerPtr = innerPtr->next;
      outerPtr = outerPtr->next;
    }
    // if element in first set less than in second: return false
    else if (strcmpRes < 0)
      return StrSetFalse;
    // if element in second set less than in first: shift second set
    else
      outerPtr = outerPtr->next;
  }

  // if second set is on end and first isn't: return false
  if (innerPtr != NULL)
    return StrSetFalse;

  return StrSetTrue;
}

/*
 * DEMONSTRATION
 */

void StrSetSetsOutput(void) {
  sets_t* currentSet = allSetsList.begin;

  if (currentSet == NULL) {
    printf("NO SETS IN MEMORY!\n");
    return;
  }

  printf("CURRENT SETS:\n");
  while (currentSet != NULL) {
    printf("Set-Id: %i, power: %i\n", currentSet->currentSet->setId, currentSet->currentSet->setPower);
    currentSet = currentSet->next;
  }
}

int StrSetOneSetOutput(int setId) {
  sets_t* setsPtr;
  if (!FoundSetInList(setId, &setsPtr))
    return StrSetNoSetInList;

  int i = 1;
  list_t* currentList = setsPtr->currentSet->begin;
  printf("CURRENT SET - set-id: %i, set-power: %i\n", setsPtr->currentSet->setId, setsPtr->currentSet->setPower);
  while (currentList != NULL) {
    printf("%i) \"%s\"\n", i++, currentList->str);
    currentList = currentList->next;
  }
  return StrSetTrue;
}

/*
 * other
 */

void ClearAllSets(void) {
  while (allSetsList.begin != NULL)
    StrSetDestruct(allSetsList.end->currentSet->setId);
}

void PrintError(int errorType) {
  if (errorType == StrSetNoMemory)
    printf("ERROR: not enough memory for operation!\n");
  else if (errorType == StrSetNoSetInList)
    printf("ERROR: there is no set with this id!\n");
  else if (errorType == StrSetBigString)
    printf("ERROR: string is too long!\n");
  else if (errorType == StrSetUncorrectStr)
    printf("ERROR: string has uncorrect sumbols!\n");
  else if (errorType == SetStrAlreadyExist)
    printf("ERROR: string already exist in set!\n");
  else if (errorType == SetStrUncorrectAnswer)
    printf("ERROR: uncorrect answer, write a number!\n");
}

int StrSetCheckStr(char* str) {
  if (strlen(str) > 80)
    return StrSetBigString;
  else {
    int i = 0;
    while (str[i] != 0)
      if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9') ||
        str[i] == '.' || str[i] == ',' || str[i] == ':' || str[i] == ';' || str[i] == '!' || str[i] == '?' || str[i] == '+' || 
        str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '=' || str[i] == '%' || str[i] == 92 /* '\' */ || str[i] == '|' || 
        str[i] == '(' || str[i] == ')' || str[i] == '[' || str[i] == ']' || str[i] == '{' || str[i] == '}' || str[i] == '<' || str[i] == '\'' ||
        str[i] == '>' || str[i] == '~' || str[i] == '@' || str[i] == '#' || str[i] == '$' || str[i] == '^' || str[i] == '&' || str[i] == ' '))
        return StrSetUncorrectStr;
      else
        i++;
  }
  return StrSetTrue;
}

int StrSetCheckBuff(char* str) {
  int i = 0;
  while (str[i] != 0)
    if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9') ||
      str[i] == '.' || str[i] == ',' || str[i] == ':' || str[i] == ';' || str[i] == '!' || str[i] == '?' || str[i] == '+' ||
      str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '=' || str[i] == '%' || str[i] == 92 /* '\' */ || str[i] == '|' ||
      str[i] == '(' || str[i] == ')' || str[i] == '[' || str[i] == ']' || str[i] == '{' || str[i] == '}' || str[i] == '<' || str[i] == '\'' ||
      str[i] == '>' || str[i] == '~' || str[i] == '@' || str[i] == '#' || str[i] == '$' || str[i] == '^' || str[i] == '&' || str[i] == ' ' || str[i] == '"'))
      return StrSetUncorrectStr;
    else
      i++;
  return StrSetTrue;
}

int StrSetCheckIdInList(int id) {
  sets_t* setsPtr;
  if (!FoundSetInList(id, &setsPtr))
    return StrSetNoSetInList;
  return StrSetTrue;
}
