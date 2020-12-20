#include "list.h"
#include <signal.h>
#include <conio.h>

//--------------------------------------------------------------------
#define $ printLog(logfile, __FILE__, __func__, __LINE__); setLog(__FILE__, __func__, __LINE__); LOG.test_str = (char*) calloc(100, 1); free(LOG.test_str);


const char *log_file_name = "C:/Users/Pasha/my_programs/Stack/stack_by_array/logfile.txt";
FILE *logfile;

struct ProcessInfo {
    const char *_FILE = "";
    const char *_FUNC = "";
    int _LINE = 0;

    char *test_str;
} LOG;

void listener(int sig) {
    fprintf(logfile, "\nError: access violation\nFile: %s\nFunction: %s\nLine: %zu\n", LOG._FILE, LOG._FUNC, LOG._LINE);
    fflush(logfile);
    fclose(logfile);
}

void setLog(const char *file, const char *func, int line)  {
    LOG._FILE = file;
    LOG._FUNC = func;
    LOG._LINE = line;
}

void printLog(FILE *logfile_, const char *file, const char *func, int line) {
    if (file != LOG._FILE) {
        fprintf(logfile_, "file: '%s' \n", file);
    }
    if (func != LOG._FUNC) {
        fprintf(logfile_, "func: %s \n", func);
    }
    fprintf(logfile_, "line: %zu\n", line);
}

//--------------------------------------------------------------------



list_t *listConstruct(list_t *list, int capacity) {
	assert(list);

	list->array = (node_t*) calloc(capacity + 1, sizeof(node_t));
	list->capacity = capacity;
	list->size = 0;

	list->status = true;

	list->first = 0;
	list->last = 0;
	list->last_free = 1;

	for (int i = 1; i <= capacity; i++) {
		list->array[i].prev = -1;
		list->array[i].next = (i + 1) % (capacity + 1);
	}

	return list;
}

list_t *newList(int capacity) {
	list_t *list = (list_t*) calloc(1, sizeof(list_t));
	return listConstruct(list, capacity);
}

int listPush(list_t *list, int prev, int next, elem_t elem) {
	assert(list);
	assert(prev >= 0 && prev <= list->capacity);
	assert(next >= 0 && next <= list->capacity);

	if (list->size == list->capacity) {
		changeListCapacity(list, max(list->capacity * 2, 8));
	}

	int next_free = list->array[list->last_free].next;

	list->array[list->last_free].prev = prev;
	list->array[list->last_free].next = next;
	list->array[list->last_free].value = elem;


	if (prev !=  0) {
		list->array[prev].next = list->last_free;
	} else {
		list->first = list->last_free;
	}

	if (next != 0) {
		list->array[next].prev = list->last_free;
	} else {
		list->last = list->last_free;
	}

	list->last_free = next_free;

	list->size++;

	return 0;
}

int listPushBetween(list_t *list, int prev, int next, elem_t elem) {
	assert(list);

	list->status = false;
	return listPush(list, prev, next, elem);
}

int listPushAfter(list_t *list, int index, elem_t elem) {
	assert(list);

	list->status = false;
	return listPush(list, index, list->array[index].next, elem);
}

int listPushBefore(list_t *list, int index, elem_t elem) {
	assert(list);

	list->status = false;
	return listPush(list, list->array[index].prev, index, elem);
}

int listPushBegin(list_t *list, elem_t elem) {
	assert(list);

	list->status = (list->size == 0);
	return listPush(list, 0, list->first, elem);
}

int listPushEnd(list_t *list, elem_t elem) {
	assert(list);

	return listPush(list, list->last, 0, elem);
}

int numToInd(list_t *list, int num) {
	assert(list);

	if (num > list->size || num < 1) {
		fprintf(stderr, "Error: invalid number: %d\n", num);
		return 1;
	}

	if (list->status) {
		return num;
	} else {
		int ind = list->first;
		for (int i = 1; i < num; i++) {
			ind = list->array[ind].next;
		}
        return ind;
	}
}

void listDump(list_t *list) {
	if (list) {
		printf("Adress of list: %p\n", list);
	} else {
		printf("Error: adress of list is nulptr\n");
		return;
	}
	printf("Size: %d, Capacity: %d\n", list->size, list->capacity);
	printf("Last free: %d\n", list->last_free);
	printf("status: %d\n", list->status);

	printf("first: %d, last: %d\n", list->first, list->last);
	for (int i = 0; i < list->capacity + 1; i++) {
		printf(" %f |", list->array[i].value);
	}
	printf("\n");
	for (int i = 0; i < list->capacity + 1; i++) {
		printf(" %d |", list->array[i].next);
	}
	printf("\n");
	for (int i = 0; i < list->capacity + 1; i++) {
		printf(" %d |", list->array[i].prev);
	}
	printf("\n\n\n\n");
}

void listSwap(list_t *list, int ind_1, int ind_2) {
	assert(ind_1 >= 0 && ind_1 <= list->capacity);
	assert(ind_2 >= 0 && ind_2 <= list->capacity);

	int prev_for_ind_2 = list->array[ind_2].prev;
	int next_for_ind_2 = list->array[ind_2].next;
	if (list->array[ind_1].prev != -1) {
		list->array[list->array[ind_1].prev].next = ind_2;
		list->array[list->array[ind_1].next].prev = ind_2;
	}
	if (prev_for_ind_2 != -1) {
		list->array[prev_for_ind_2].next = ind_1;
		list->array[next_for_ind_2].prev = ind_1;
	}

	list->array[0] = {0, 0, 0};

	node_t temp_node = list->array[ind_1];
	list->array[ind_1] = list->array[ind_2];
	list->array[ind_2] = temp_node;


	if (ind_1 == list->first) {
		list->first = ind_2;
	} else if (ind_2 == list->first) {
		list->first = ind_1;
	}

	if (ind_1 == list->last) {
		list->last = ind_2;
	} else if (ind_2 == list->last) {
		list->last = ind_1;
	}
}

void listSort(list_t *list) {
	int ind = list->first;
	int next_ind;
	for (int i = 1; i <= list->size; i++) {
		next_ind = list->array[ind].next;
		if (next_ind == i) {
			next_ind = ind;
		}

		listSwap(list, i, ind);
		ind = next_ind;
	}

	list->first = 1;
	list->last = list->size;

	if (list->size < list->capacity) {
		list->last_free = list->size + 1;
	} else {
		list->last_free = 0;
	}

	for (int i = list->size + 1; i < list->capacity + 1; i++) {
		list->array[i] = {0, -1, (i + 1) % (list->capacity + 1)};
	}

	list->status = true;
}

bool isSorted(list_t *list) {
	assert(list);

	int node = list->first;
	int num = 1;

	while (list->array[node].next != 0) {
		if (node != num++) {
			return false;
		}
	}

	return true;
}

void listDeleteInd(list_t *list, int ind) {
	assert(list);
	assert(ind >= 0 && ind <= list->capacity);


	if (ind != list->last && list->array[ind].prev != -1) {
		list->status = false;
	}

	if (list->array[ind].prev != -1) {

		list->array[list->array[ind].prev].next = list->array[ind].next;
		list->array[list->array[ind].next].prev = list->array[ind].prev;
		list->array[0] = {0, 0, 0};

		list->array[ind].prev = -1;
		list->array[ind].next = list->last_free;
		list->last_free = ind;

		list->size--;

		if (list->size * 4 < list->capacity) {
			changeListCapacity(list, max(list->capacity / 2, 8));
		}
	}
}

void listDeleteNum(list_t *list, int num) {
	listDeleteInd(list, numToInd(list, num));
}

void listPop(list_t *list) {
	listDeleteInd(list, list->last);
}

void changeListCapacity(list_t *list, int new_cap) {
	assert(list);
	assert(new_cap > 0);

	list->array = (node_t*) realloc(list->array, (new_cap + 1) * sizeof(node_t));

	for (int i = list->capacity + 1; i < new_cap + 1; i++) {
		list->array[i].value = 0;
		list->array[i].next = (i + 1) % (new_cap + 1);
		list->array[i].prev = -1;
	}

	int free_node = list->last_free;
	if (free_node == 0) {
		list->last_free = list->capacity + 1;
	} else {
		while (list->array[free_node].next != 0) {
			free_node = list->array[free_node].next;
		}
		list->array[free_node].next = list->capacity + 1;
	}

	list->capacity = new_cap;
}

void makeGraph(list_t *list) {
	assert(list);

$	FILE *graph = fopen("C:/Users/Pasha/my_programs/list/graph.txt", "w");
$
$	fprintf(graph, "digraph structs {\nrankdir=LR;\n{edge[color=white];");
$
$	for (int i = 0; i < list->capacity + 1; i++) {
$		fprintf(graph, "\"%d\"", i);
$		if (i < list->capacity) {
$			fprintf(graph, " -> ");
$		}
$	}
$
$	fprintf(graph, ";\n}\n");
$
$
$	for (int i = 0; i < list->capacity + 1; i++) {
$		fprintf(graph, "{rank = same; \"%d\"; e%d [style = \"filled\", color = ", i, i);
$
		if (list->array[i].prev != -1) {
$			if (i == 0) {
				fprintf(graph, "\"#fd7e82\"");
			} else {
				fprintf(graph, "\"#b5bdfc\"");
			}
$		} else {
$			fprintf(graph, "\"#8ff5b9\"");
$		}

$		fprintf(graph, ", shape=record, label=\"value: %f | next: %d | prev: %d\"];}\n", list->array[i].value, list->array[i].next, list->array[i].prev);
$	}
$
$	int node = list->first;
$	while (list->array[node].next != 0) {
$		fprintf(graph, "e%d -> e%d;\ne%d -> e%d;\n", node, list->array[node].next, list->array[node].next, node);
$		node = list->array[node].next;
$	}
$
$	node = list->last_free;
$	while (list->array[node].next != 0) {
$		fprintf(graph, "e%d -> e%d[color=\"green\"];\n", node, list->array[node].next);
$		node = list->array[node].next;
$	}
$	fprintf(graph, "}");
	fclose(graph);
$
$	system("dot -Tjpg C:/Users/Pasha/my_programs/list/graph.txt > output.jpg");
	system("output.jpg");
}

int max(int a, int b) {
	return (a > b) ? a: b;
}

int main() {
	logfile = fopen(log_file_name, "w");
    signal(SIGSEGV, listener);

	list_t *list = newList(5);

	listPushEnd(list, 1.5);
	listPushAfter(list, 1, 2.5);
	listPushEnd(list, 3.5);
	listPushEnd(list, 4.5);
	listPushEnd(list, 5.5);

	listPushEnd(list, 6);

	listSort(list);

	listDeleteNum(list, 3);
	listDeleteNum(list, 4);


	makeGraph(list);

	return 0;
}
