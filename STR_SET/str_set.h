#ifndef STR_SET_H
#define STR_SET_H

/*
 * STRUCTS AND DEFINES
 */

#define STR_SIZE 81

// return -1 - ALL GOOD!, else negative - something bad((, not negative - number of set
enum returnType {
  StrSetTrue = -1,
  StrSetFalse = -2,

  StrSetNoMemory = -3,
  StrSetNoSetInList = -4,
  StrSetBigString = -5,
  StrSetUncorrectStr = -6,
  SetStrAlreadyExist = -7,
  SetStrUncorrectAnswer = -8
};

// element
typedef struct list_t list_t;
struct list_t {
  char str[STR_SIZE];
  list_t* prev;
  list_t* next;
};

// list of elements - set
typedef struct strSet_t strSet_t;
struct strSet_t {
  int setId;

  list_t* begin;
  int setPower;
};

// list of sets
typedef struct sets_t sets_t;
struct sets_t {
  strSet_t* currentSet;
  sets_t* next;
  sets_t* prev;
};

/*
 * FUNCTIONS
 */

// construct and destruct
int StrSetConstruct(void);
int StrSetDestruct(int setId);

// methods
int StrSetOutputPower(int setId);
int StrSetAddElement(int setId, char *newStr);
int StrSetFindElement(int setId, char* strFind);
int StrSetDeleteElement(int setId, char* strToDelete);

//operations
int StrSetUnion(int firstSetId, int secondSetId);
int StrSetIntersection(int firstSetId, int secondSetId);
int StrSetDifference(int firstSetId, int secondSetId);
int StrSetSymmetricDifference(int firstSetId, int secondSetId);
int StrSetCheckInner(int innerSetId, int outerSetId);

// demonstration
void StrSetSetsOutput(void);
int StrSetOneSetOutput(int setId);

// other
void ClearAllSets(void);
void PrintError(int errorType);
int StrSetCheckStr(char* str);
int StrSetCheckBuff(char* str);

int StrSetCheckIdInList(int id);

#endif STR_SET_H