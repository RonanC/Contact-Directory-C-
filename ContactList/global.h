#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// declarations
void initList();

//#define NULL 0 // already defined be default in VS

typedef int BOOL;
#define true 1
#define false 0

struct contact{
	char fName[15];
	char sName[15];
	char phone[15];
	char email[25];
	char company[25];
	struct contact *next;
	struct contact *prev;
};

// linked-list
struct contact *listHead;
struct contact *listTail;



// initializes the list with a head and a tail
void initList(){
	// set head and tail
	listHead = (struct contact *)malloc(sizeof(struct contact));
	listTail = (struct contact *)malloc(sizeof(struct contact));

	// point head to tail, and tail to head
	listHead->next = listTail;
	listHead->prev = NULL;

	listTail->prev = listHead;
	listTail->next = NULL;
} // initList