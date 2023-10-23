// Rimsha Rizvi
// Project - Using the implementation of an ADT create an interactive program for the user to manage a queue of buzzers.

#include <stdio.h>
#include <stdlib.h>
#include "sq.h"


#define TRUE 1
#define FALSE 0

// A linked list implementation using double-linked nodes
typedef struct node {
  struct node* next;
  struct node* prev;
  int val;
} NODE;

// Using nodes to create the list struct
typedef struct list_struct{
  NODE* front;
  NODE* back;
} LIST;

// Dynamically allocated array to store buzzers
typedef struct {
  NODE** buzzers;
  int current_index;
  int current_size;  // max cap
} dArr;


// SQ struct
typedef struct service_queue {
  dArr* buzzers_array;
  LIST* the_queue;
  LIST* buzzer_bucket;
  int index;
} SQ;



// Functions from llist.c
// these functions help enable me to create a linked list implementation using double-linked nodes

LIST* list_create() {
  LIST* l = malloc(sizeof(LIST));
  
  l->front = NULL;
  l->back = NULL;
  return l;
}

void list_free(LIST *l) {
NODE *p = l->front;
NODE *pnext;

  while(p != NULL) {
    pnext = p->next;   // keeps us from de-referencing a freed ptr
    free(p);
    p = pnext;
  }
  // now free the LIST 
  free(l);
}


void list_push_front(LIST *l, int val) {
  NODE* p = (NODE*) malloc (sizeof(NODE));
  
  p->val = val;
  p->next = l->front; 
  p->prev = NULL;

  l->front = p;
  if(l->back == NULL) {
    l->back = p;
  } else {
    p->next->prev = p;
  }
}


void list_push_back(LIST *l, int val) {
  NODE* p;

  if(l->back == NULL) {  // list empty - same as push_front
    list_push_front(l, val);
    return;
  } else {  // at least one element before push
    p = (NODE*) malloc (sizeof(NODE));
    p->val = val;
    p->next = NULL;
    p->prev = l->back;
    l->back->next = p;
    
    l->back = p;
    return;
  }
}


int list_is_empty(LIST *l) {
  if (l->front == NULL) {
    return TRUE;
  }
  return FALSE;
}


int list_pop_front(LIST *l) {
  int ret;
  NODE *p;

  if(list_is_empty(l)) {
    return -1;  // no-op
  }

  ret = l->front->val;
  
  if(l->front == l->back) {  // one element
    free(l->front);
    l->front = NULL;
    l->back = NULL;
  } else {
    p = l->front;  // don't lose node being deleted
    l->front = l->front->next;  // hop over
    l->front->prev = NULL;
    free(p);
  }
  return ret;
}

// Array helper functions
dArr* array_create() {
  dArr* darr = (dArr*) malloc (sizeof(dArr));
  darr->buzzers = (NODE**) calloc (4, sizeof(NODE*));
  darr->current_index = 0;
  darr->current_size = 4;  // initializing the length to 4
  return darr;
}



/**
* Function: sq_create()
* Description: creates and intializes an empty service queue.
* 	It is returned as an SQ pointer.
* 
* RUNTIME REQUIREMENT: O(1)
*/
extern SQ * sq_create() {
  SQ* q;
  q = (SQ*) malloc (sizeof(SQ));
  q->buzzers_array = array_create();

  q->the_queue = list_create();
  q->buzzer_bucket = list_create();

  q->index = -1;
  return q;
}


/**
* Function: sq_free()
* Description:  see sq.h
*
* RUNTIME REQUIREMENT:  O(N_t)  where N_t is the number of buzzer 
*	IDs that have been used during the lifetime of the
*	service queue; in general, at any particular instant
*	the actual queue length may be less than N_t.
*
*	[See discussion of in header file]
*
* RUNTIME ACHIEVED:  O(N_t)
*
*/
void sq_free(SQ *q) {
  list_free(q->the_queue);
  list_free(q->buzzer_bucket);

  free(q->buzzers_array->buzzers);
  free(q->buzzers_array);

  free(q);
}


/**
* Function: sq_display()
* Description:  see sq.h
*
* REQUIRED RUNTIME:  O(N)  (N is the current queue length).
* ACHIEVED RUNTIME:  O(N)  YAY!!
*/
extern void sq_display(SQ *q){
  NODE* current = q->the_queue->front;
  while (current != NULL){
    printf("%d ", current->val);
    current = current->next;
  }
  printf("\n");
}


/**
* Function: sq_length()
* Description:  see sq.h
*
* REQUIRED RUNTIME:  O(1)
* ACHIEVED RUNTIME:  O(1)
*/
extern int sq_length(SQ *q) {
  return q->buzzers_array->current_index;
}

// inserting
// adding a new node to the given dynamic array
void inserting(NODE* insert, dArr* arr) {
  if (arr->current_index >= arr->current_size) {  // does it exceed the max capacity?
    arr->current_size = arr->current_size * 2;
    NODE** new_node = realloc(arr->buzzers, sizeof(NODE*) * arr->current_size);
    arr->buzzers = new_node;
  }
  arr->buzzers[insert->val] = insert;
}


/*
* Function: sq_give_buzzer()
* Description:  see sq.h
*
* REQUIRED RUNTIME:  O(1)  ON AVERAGE or "AMORTIZED":  in other words, if 
*		there have been M calls to sq_give_buzzer, the total
*		time taken for those M calls is O(M).
*
*		An individual call may therefore not be O(1) so long
*		as when taken as a whole they average constant time.
*
*		(Hopefully this reminds you of an idea we employed in
*		the array-based implementation of the stack ADT).
*
* ACHIEVED RUNTIME:  O(1)
*/
extern int sq_give_buzzer(SQ *q) {
  if (list_is_empty(q->buzzer_bucket) == TRUE) {  // empty?
    list_push_back(q->the_queue, q->index + 1);
  } else {
    int num = q->buzzer_bucket->front->val;
    list_pop_front(q->buzzer_bucket);
    list_push_back(q->the_queue, num);
  }
  inserting(q->the_queue->back, q->buzzers_array);

  if (q->the_queue->back->val > q->index) {
    q->index = q->the_queue->back->val;
  }

  q->buzzers_array->current_index++;
  return q->the_queue->back->val;
}        


/**
* function: sq_seat()
* description:  see sq.h
*
* REQUIRED RUNTIME:  O(1)
* ACHIEVED RUNTIME:  O(1)
*/
extern int sq_seat(SQ *q) {
  if (list_is_empty(q->the_queue)) {
    return -1;
  }
  NODE* popped = q->the_queue->front;
  int num = popped->val;
  
  list_pop_front(q->the_queue);
  list_push_front(q->buzzer_bucket, num);
  
  q->buzzers_array->current_index--;
  
  if ((q->index > 0) && (num == q->index)) {
    q->index--;
  }
  return num;
}


/**
* function: sq_kick_out()
*
* description:  see sq.h
*
* REQUIRED RUNTIME:  O(1)
* ACHIEVED RUNTIME:  O(1)
*/
extern int sq_kick_out(SQ *q, int buzzer) {
  NODE* node = q->buzzers_array->buzzers[buzzer];

  if (list_is_empty(q->the_queue) == TRUE) {
    return 0;
  }
  if (node == NULL){  // doesn't exist
    return 0;
  }

  if ((buzzer > q->index) || (buzzer > q->buzzers_array->current_size)) {
    return 0;
  }

  if (node == q->the_queue->front) {
    sq_seat(q);
  } else {
    if (node == q->the_queue->back) {
      q->the_queue->back = q->the_queue->back->prev;
      q->the_queue->back->next = NULL;
    } else {
      node->prev->next = node->next;
      node->next->prev = node->prev;
    }
    list_push_front(q->buzzer_bucket, node->val);
    // free
    q->buzzers_array->buzzers[buzzer] = NULL;
    free(node);

    q->buzzers_array->current_index--;
  }
  return 1;
}


/**
* function:  sq_take_bribe()
* description:  see sq.h
*
* REQUIRED RUNTIME:  O(1)
* ACHIEVED RUNTIME:  O(1)
*/
extern int sq_take_bribe(SQ *q, int buzzer) {
  if (q->the_queue->front == NULL) {
    return 0;
  }
  if (q->buzzers_array->buzzers[buzzer] == NULL) {
    return 0;
  }
  if (q->buzzers_array->buzzers[buzzer]->val > q->index) {
    return 0;
  }
  sq_kick_out(q, buzzer);

  int buzzerID = q->buzzer_bucket->front->val;
  list_pop_front(q->buzzer_bucket);
  list_push_front(q->the_queue, buzzerID);
  inserting(q->the_queue->front, q->buzzers_array);
  q->buzzers_array->current_index++;
  return 1;
}
