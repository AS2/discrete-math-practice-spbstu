#ifndef str_parser_h
#define str_parser_h

typedef struct strList_t strList_t;
struct strList_t {
  char str[81];
  strList_t* next;
};

typedef struct strQueue_t strQueue_t;
struct strQueue_t {
  strList_t* begin;
  strList_t* end;
};

// operations
int QueuePop(strQueue_t** Q, char** str);
int QueueIsEmpty(strQueue_t* Q);
void ClearQueue(strQueue_t** Q);

// string parser
int ParseString(strQueue_t** Q, char* buff);

#endif
