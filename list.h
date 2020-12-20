#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>

typedef double elem_t;

typedef struct {
	elem_t value;
	int prev;
	int next;
} node_t;

typedef struct {
	node_t *array;
	int capacity;
	int size;

	bool status;

	int first;
	int last;

	int last_free;
} list_t;



int max(int a, int b);

list_t *listConstruct(list_t *list, int capacity);

list_t *newList(int capacity);

int listPush(list_t *list, int prev, int next, elem_t elem);

int listPushBetween(list_t *list, int prev, int next, elem_t elem);

int listPushBegin(list_t *list, elem_t elem);

int listPushEnd(list_t *list, elem_t elem);

int numToInd(list_t *list, int num);

void listDump(list_t *list);

void listSwap(list_t *list, int ind_1, int ind_2);

void listSort(list_t *list);

void listDeleteInd(list_t *list, int ind);

void listDeleteNum(list_t *list, int num);

void listPop(list_t *list);

void changeListCapacity(list_t *list, int new_cap);

void makeGraph(list_t *list);

bool isSorted(list_t *list);
