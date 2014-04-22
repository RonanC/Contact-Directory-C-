#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "global.h"

// declarations
void initList();
void load();
void getField();
int showMenu();
void add();
void rem();
int sortMenu();
void sort();
void edit();
void view();
void save();

// linked-list
struct contact *listHead;
struct contact *listTail;

int main(){
	int choice;

	printf("Roncon Contact List\n");
	printf("===================\n\n");

	initList();
	load();

	do{
		choice = showMenu();

		switch(choice){
			case 0:	// exit
				printf("Goodbye\n");
				break;

			case 1:	// add
				printf("Add: \n");
				add();
				break;

			case 2:	// remove
				printf("Remove:\n");
				rem();
				break;
				
			case 3:	// sort
				printf("Sort:\n");
				if(listHead->next != listTail){
					sort();
				}
				else{
					printf("No contacts\n");
				}
				break;

			case 4:	// edit
				printf("Edit:\n");
				edit();
				break;

			case 5:	// view
				printf("View:\n");
				if(listHead->next != listTail){
					view();
				}
				else{
					printf("No contacts\n");
				}
				break;

			default:
				printf("Not a valid number, re-enter.\n");
				break;
		} // switch
	}while(choice != 0);

	save();

	printf("\n\n");
	system("pause");
} // main

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

void load(){
	FILE *fp;
	char buffer[60];
	struct contact *loadContact;
	char strBuf[25];
	int i, j;
	// open file for reading
	fp = fopen("contacts.csv", "r");

	if(fp != NULL){
		while( fgets(buffer,sizeof(buffer),fp) != NULL)
		{
			loadContact = (struct contact *)malloc(sizeof(struct contact));
	
			strcpy(loadContact->fName, strtok(buffer,","));
			strcpy(loadContact->sName, strtok(NULL,","));
			strcpy(loadContact->phone, strtok(NULL,","));
			strcpy(loadContact->email, strtok(NULL,","));
			strcpy(loadContact->company, strtok(NULL,","));

			// add to list
			loadContact->next = listTail;
			loadContact->prev = listTail->prev;
			listTail->prev->next = loadContact;
			listTail->prev = loadContact;

			// this makes sure that there is no newline feed ascii character in the company name (this was posing a problem previously)
			i = j = 0;
			while(loadContact->company[i] != '\0'){
				if(loadContact->company[i] == 10){ // 10 == newline ascii character
					loadContact->company[i] = '\0';
				}
				i++;
			}
			//strBuf[j] = '\0';
			//strcpy(loadContact->company, strBuf);
		}
		fclose(fp);
	}
}

int showMenu(){
	int choice;

	printf("\n1: Add Contact\t\t");
	printf("2: Remove Contact\t\t");
	printf("3: Sort Contact\n");
	printf("4: Edit Contact\t\t");
	printf("5: View Contacts\t\t");
	printf("0: Exit\n\n");

	printf("Enter your choice: ");
	// scanf() is the best for reading numbers
	scanf("%d", &choice);
	// fflush() can give undefined behavior so I use: while (getchar() != '\n');
	while (getchar() != '\n');

	printf("\n");

	return choice;
} // showMenu

void add(){
	// create new contact
	struct contact *newContact;
	newContact = (struct contact *)malloc(sizeof(struct contact));
	
	// get data from user
		// gets_s() more secure then gets/scanf (no buffer overflows or wrong types)
	printf("Enter first name: ");
	gets_s(newContact->fName, 15);

	//makes sure 1st letter of name is upper case
	if(islower(newContact->fName[0])){
		toupper(newContact->fName[0]);
	}
	
	printf("Enter surname name: ");
	gets_s(newContact->sName, 15);

	if(islower(newContact->sName[0])){
		toupper(newContact->sName[0]);
	}

	printf("Enter phone number: ");
	gets_s(newContact->phone, 15);

	printf("Enter email: ");
	gets_s(newContact->email, 25);

	printf("Enter company: ");
	gets_s(newContact->company, 25);
	
	// add entry to the list tail
		// change new contact pointers first
	newContact->next = listTail;
	newContact->prev = listTail->prev;

	listTail->prev->next = newContact; // before newContact
	listTail->prev = newContact;	// after newContact
}

void rem(){
	struct contact *curr;

	// temporary node
	curr = listHead->next;

	while(curr->next != NULL){
		printf("%s %s\n%s\n%s\n%s\n", curr->fName, curr->sName, curr->phone, curr->email, curr->company);
		curr = curr->next;
	}
}

int sortMenu(){
	int choice;
	
	printf("\nHow do you want to sort contacts?\n");

	printf("1: first name\n");
	printf("2: surname\n");

	printf("\nEnter your choice: ");
	// scanf() is the best for reading numbers
	scanf("%d", &choice);
	// fflush() can give undefined behavior so I use: while (getchar() != '\n');
	while (getchar() != '\n');

	printf("\n");

	return choice;
} // showMenu

void sort(){
	// sort by fName, lName, phone
	struct contact *curr, *next, *temp;
	int choice, swap;

	// first in the list
	curr = listHead->next;
	next = curr->next;

	do{
		choice = sortMenu();
		switch(choice){
			case 1:	// fName
				// below is my custom made bubble sort
				do{
					curr = listHead->next;
					next = curr->next;
					swap = 0;
					
					while(next->next != NULL){ // while next != tail
						if(curr->fName[0] > next->fName[0]){
							curr->prev->next = next;	// before current ->
							curr->next = next->next;	// currents->
							next->prev = curr->prev;	// nexts<-
							curr->prev = next;			// currents<-
							
							next->next->prev = curr;	// after nexts <-

							next->next = curr;			// nexts->

							swap++;

							next = curr->next;			// next is now after current
														
						} // if
						else{
							curr = curr->next;
							next = curr->next;
						}
					}
					next->prev = curr; // next is now tail so we need to set tails previous to current (which is the last contact in the list)
					// used to test how many swaps is done on each iteration
					//printf("Swaps: %d\n", swap);
				}while(swap != 0);

				break;

			case 2:	// sName
				// below is my custom made bubble sort
				do{
					curr = listHead->next;
					next = curr->next;
					swap = 0;
					
					while(next->next != NULL){ // while next != tail
						if(curr->sName[0] > next->sName[0]){
							curr->prev->next = next;	// before current ->
							curr->next = next->next;	// currents->
							next->prev = curr->prev;	// nexts<-
							curr->prev = next;			// currents<-
							
							next->next->prev = curr;	// after nexts <-

							next->next = curr;			// nexts->

							swap++;

							next = curr->next;			// next is now after current
														
						} // if
						else{
							curr = curr->next;
							next = curr->next;
						}
					}
					next->prev = curr; // next is now tail so we need to set tails previous to current (which is the last contact in the list)
					// used to test how many swaps is done on each iteration
					// printf("Swaps: %d\n", swap);
				}while(swap != 0);

				break;

			default:
				printf("Not a valid number, re-enter.\n");
				break;
		} // switch
	}while(choice < 1 || choice > 2);

	printf("Sort complete!\n");
}

void edit(){

}

void view(){
	// sort by fName, lName, phone
	struct contact *curr;

	// first in the list
	curr = listHead->next;

	while(curr->next != NULL){
		printf("%s %s\n%s\n%s\n%s\n\n", curr->fName, curr->sName, curr->phone, curr->email, curr->company);
		curr = curr->next;
	}
}

void save(){
	FILE *fp;
	struct contact *curr;

	fp = fopen("contacts.csv", "w");
	curr = listHead->next;

	while(curr->next != NULL){
		fprintf(fp, "%s,%s,%s,%s,%s\n", curr->fName, curr->sName, curr->phone, curr->email, curr->company);
		curr = curr->next;
	}

	fclose(fp);
}