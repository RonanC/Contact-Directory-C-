// Ronan Connolly - G00274374 - GMIT - 22nd April 2014
// Advanced Procedural Programming - Contacts List Project - for Damien Costello

#include "global.h"

// declarations
void load();
void getField();
int showMenu();
void add();
void rem();
int sortMenu();
void sort();
int editMenu();
void edit();
void view();
int searchMenu();
void search();
void save();

void encryptWord(char *);
void encryptFile();

BOOL encrypted = false;

// header and do while switch that keeps showing you options until you exit
int main(){
	int choice;
	
	printf("Roncon Contact Directory\n");
	printf("========================\n");

	initList();
	load();

	do{
		choice = showMenu();

		switch(choice){
			case 0:	// exit
				printf("Goodbye\n");
				break;

			case 1:	// view all
				printf("View all:\n");
				if(listHead->next != listTail){
					view();
				}
				else{
					printf("No contacts\n");
				}
				break;

			case 2:	// search
				printf("Search:\n");
				if(encrypted){
					printf("Decrypt file first!\n");
				}
				else{
					search();
				}
				break;

			case 3:	// add
				printf("Add: \n");
				if(encrypted){
					printf("Decrypt file first!\n");
				}
				else{
					add();
				}
				break;

			case 4:	// remove
				printf("Remove:\n");
				if(encrypted){
					printf("Decrypt file first!\n");
				}
				else{
					rem();
				}
				break;

			case 5:	// edit
				printf("Edit:\n");
				if(encrypted){
					printf("Decrypt file first!\n");
				}
				else{
					edit();
				}
				break;	

			case 6:	// sort
				printf("Sort:\n");
				if(listHead->next != listTail){
					sort();
				}
				else{
					printf("No contacts\n");
				}
				break;

			case 7:	// encrypt
				printf("En/Decrypt:\n");
				encryptFile();
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

// loads the .csv file by reading in each line and splitting said line into tokens (seperated by commas)
void load(){
	FILE *fp;
	char buffer[60];
	struct contact *loadContact;
	int i, j;
	// open file for reading
	fp = fopen("contacts.csv", "r");

	if(fp != NULL){
		while( fgets(buffer,sizeof(buffer),fp) != NULL)
		{
			loadContact = (struct contact *)malloc(sizeof(struct contact));
	
			// split contact into tokens
			strcpy(loadContact->fName, strtok(buffer,","));
			strcpy(loadContact->sName, strtok(NULL,","));
			strcpy(loadContact->phone, strtok(NULL,","));
			strcpy(loadContact->email, strtok(NULL,","));
			strcpy(loadContact->company, strtok(NULL,","));

			// could have done it this way either:
				/*
				char buffer[] = "line to be tokenized";
				char *tok;
				char *ptr = buffer;

				tok = strtok(ptr, ",")
				strcpy(loadContact->fName, tok);
				||
				strcpy(loadContact->fName, strtok(ptr, ","));
					
				*/

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
		}
		fclose(fp);
	}

	// checks if the file is en/decrypted
	if(listHead->next->next == NULL){
		printf("No Contacts!\n");
	}
	else{
		if(isascii(listHead->next->fName[0])){
			encrypted = false;
			printf("Contacts are decrypted.\n");
		}
		else{
			encrypted = true;
			printf("Contacts are encrypted, decrypt for full functionality.\n");
		}
	}
}

// shows the main menu options
int showMenu(){
	int choice;

	printf("\n1: View all Contacts\t");
	printf("2: Search Contacts\t");
	printf("3: Add Contact\t\t\t");
	printf("4: Remove Contact\n");
	printf("5: Edit Contact\t\t");
	printf("6: Sort Contact\t\t");
	printf("7: En/Decrypt File\t\t");
	printf("0: Exit\n\n");

	printf("Enter your choice: ");
	// scanf() is the best for reading numbers
	scanf("%d", &choice);
	// fflush() can give undefined behavior so I use: while (getchar() != '\n');
	while (getchar() != '\n');

	printf("\n");

	return choice;
} // showMenu

// creates a new contact, asks for values (in a very secure way, gets_s) then inserts it into the end of the list
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
		newContact->fName[0] = toupper(newContact->fName[0]);
	}
	
	printf("Enter surname name: ");
	gets_s(newContact->sName, 15);

	if(islower(newContact->sName[0])){
		newContact->sName[0] = toupper(newContact->sName[0]);
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

// shows the various sort options
int sortMenu(){
	int choice;
	
	printf("\nHow do you want to sort contacts?\n");

	printf("1: First name\n");
	printf("2: Surname\n");

	printf("\nEnter your choice: ");
	// scanf() is the best for reading numbers
	scanf("%d", &choice);
	// fflush() can give undefined behavior so I use: while (getchar() != '\n');
	while (getchar() != '\n');

	printf("\n");

	return choice;
} // showMenu

// sorts the list by either first or surname ascendin
// I created my own bubble sort from scratch to do this (the pencil and paper method was essential for this piece of code ;) )
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

// shows the various edit options
int editMenu(){
	int choice;

	printf("\nEdit:\n");
	printf("1: First Name\t\t");
	printf("2: Surname\t\t");
	printf("3: Phone Number\n");
	printf("4: Email\t\t");
	printf("5: Company\t\t");
	printf("0: Cancel\n\n");

	printf("Enter your choice: ");
	// scanf() is the best for reading numbers
	scanf("%d", &choice);
	// fflush() can give undefined behavior so I use: while (getchar() != '\n');
	while (getchar() != '\n');

	printf("\n");

	return choice;
}

// allows you to search for a contact by first/surname and select mutliple fields to edit for that chosen contact
void edit(){
	struct contact *curr;
	char toEdit[15], confirm[4], updated[25];
	BOOL found = false, edited = false;
	int choice;

	printf("Enter contact to edit? (case sensitive) : ");
	scanf("%s", &toEdit);
	while (getchar() != '\n');
	
	// temporary node
	curr = listHead->next;

	while(curr->next != NULL && edited == false){
		if(strstr(curr->fName, toEdit) || strstr(curr->sName, toEdit)){
			found = true;
			printf("\n%s %s\n%s\n%s\n%s\n\n", curr->fName, curr->sName, curr->phone, curr->email, curr->company);
			
			printf("Is this the contact? (y/es n/o) : ");
			scanf("%s", &confirm);
			while (getchar() != '\n');

			if(tolower(confirm[0]) == 121){
				do{
					choice = editMenu();

					switch(choice){
						case 0:	// exit
							break;

						case 1:	// First
							printf("New First Name: ");
							scanf("%s", &updated);
							strcpy(curr->fName, updated);
							edited = true;

							break;

						case 2:	// Surname
							printf("Surname:\n");
							scanf("%s", &updated);
							strcpy(curr->sName, updated);
							edited = true;

							break;
				
						case 3:	// Phone
							printf("Phone Number:\n");
							scanf("%s", &updated);
							strcpy(curr->phone, updated);
							edited = true;

							break;

						case 4:	// Email
							printf("Email:\n");
							scanf("%s", &updated);
							strcpy(curr->email, updated);
							edited = true;

							break;

						case 5:	// Company
							printf("Company:\n");
							scanf("%s", &updated);
							strcpy(curr->company, updated);
							edited = true;

							break;

						default:
							printf("Not a valid number, re-enter.\n");
							break;
					} // switch
				}while(choice != 0);

				if(edited == true){
					printf("Update Successful!\n");
					printf("\n%s %s\n%s\n%s\n%s\n\n", curr->fName, curr->sName, curr->phone, curr->email, curr->company);
					edited = true;
				} // if
			}
			else{
				curr = curr->next;
			}
		} // if
		else{
			curr = curr->next;
		}
	} // while
	if(found == false){
		printf("Could not find: %s\n", toEdit);
	}
	else if(found == true && edited == false){
		printf("No more entries for: %s\n", toEdit);
	}
}

// allows you to search for a contact by first or surname and remove it, you may iterate through various contacts with the same/similar names
void rem(){
	struct contact *curr;
	char toDelete[15], confirm[4];
	BOOL found = false, deleted = false;

	printf("Enter contact to remove? (case sensitive) : ");
	scanf("%s", &toDelete);
	while (getchar() != '\n');
	
	// temporary node
	curr = listHead->next;

	while(curr->next != NULL && deleted == false){
		if(strstr(curr->fName, toDelete) || strstr(curr->sName, toDelete)){
			found = true;
			printf("\n%s %s\n%s\n%s\n%s\n\n", curr->fName, curr->sName, curr->phone, curr->email, curr->company);
			
			printf("Is this the contact? (y/es n/o) : ");
			scanf("%s", &confirm);
			while (getchar() != '\n');

			if(tolower(confirm[0]) == 121){
				curr->next->prev = curr->prev;
				curr->prev->next = curr->next;

				printf("\%s %s is now deleted\n", curr->fName, curr->sName);
				deleted = true;
				free(curr);
			}
			else{
				curr = curr->next;
			}
		} // if
		else{
			curr = curr->next;
		}
	} // while
	if(found == false){
		printf("Could not find: %s\n", toDelete);
	}
	else if(found == true && deleted == false){
		printf("No more entries for: %s\n", toDelete);
	}
}

// prints all contacts on list to the screen
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

// shows the search options
int searchMenu(){
	int choice;

	printf("\Search by:\n");
	printf("1: First Name\t\t");
	printf("2: Surname\t\t");
	printf("3: Phone Number\n");
	printf("4: Email\t\t");
	printf("5: Company\t\t");
	printf("0: Cancel\n\n");

	printf("Enter your choice: ");
	scanf("%d", &choice);
	while (getchar() != '\n');

	return choice;
}

// this function allows you to seach a contact by any of its fields/values, you can do sub string searches also, it then prints out all matching results
void search(){
	struct contact *curr;
	char toSearch[25];
	BOOL found = false;
	int choice, foundCount;

	do{
		// temporary node
		curr = listHead->next;
		foundCount = 0;
		choice = searchMenu();

		switch(choice){
			case 0:	// exit
				break;

			case 1:	// First
				printf("Enter first name to search (case sensitive) : ");
				scanf("%s", &toSearch);
				while (getchar() != '\n');
				toSearch[0] = toupper(toSearch[0]);

				while(curr->next != NULL){
					if(strstr(curr->fName, toSearch)){
						found = true;
						foundCount++;
						printf("\n%s %s\n%s\n%s\n%s\n", curr->fName, curr->sName, curr->phone, curr->email, curr->company);
					} // if
					curr = curr->next;
				} // while
				if(found == false){
					printf("Could not find: %s\n", toSearch);
				}
				else{
					printf("\nFound %d Matching Contact(s).\n", foundCount);
				}

				break;

			case 2:	// Surname
				printf("Enter surname to search (case sensitive) : ");
				scanf("%s", &toSearch);
				while (getchar() != '\n');
				toSearch[0] = toupper(toSearch[0]);

				while(curr->next != NULL){
					if(strstr(curr->sName, toSearch)){
						found = true;
						foundCount++;
						printf("\n%s %s\n%s\n%s\n%s\n", curr->fName, curr->sName, curr->phone, curr->email, curr->company);
					} // if
					curr = curr->next;
				} // while
				if(found == false){
					printf("Could not find: %s\n", toSearch);
				}
				else{
					printf("\nFound %d Matching Contact(s).\n", foundCount);
				}

				break;
				
			case 3:	// Phone
				printf("Enter phone number to search: ");
				scanf("%s", &toSearch);
				while (getchar() != '\n');

				while(curr->next != NULL){
					if(strstr(curr->phone, toSearch)){
						found = true;
						foundCount++;
						printf("\n%s %s\n%s\n%s\n%s\n", curr->fName, curr->sName, curr->phone, curr->email, curr->company);
					} // if
					curr = curr->next;
				} // while
				if(found == false){
					printf("Could not find: %s\n", toSearch);
				}
				else{
					printf("\nFound %d Matching Contact(s).\n", foundCount);
				}

				break;

			case 4:	// Email
				printf("Enter email to search (case sensitive) : ");
				scanf("%s", &toSearch);
				while (getchar() != '\n');

				while(curr->next != NULL){
					if(strstr(curr->email, toSearch)){
						found = true;
						foundCount++;
						printf("\n%s %s\n%s\n%s\n%s\n", curr->fName, curr->sName, curr->phone, curr->email, curr->company);
					} // if
					curr = curr->next;
				} // while
				if(found == false){
					printf("Could not find: %s\n", toSearch);
				}
				else{
					printf("\nFound %d Matching Contact(s).\n", foundCount);
				}

				break;

			case 5:	// Company
				printf("Enter company to search (case sensitive) : ");
				scanf("%s", &toSearch);
				while (getchar() != '\n');

				while(curr->next != NULL){
					if(strstr(curr->company, toSearch)){
						found = true;
						foundCount++;
						printf("\n%s %s\n%s\n%s\n%s\n", curr->fName, curr->sName, curr->phone, curr->email, curr->company);
					} // if
					curr = curr->next;
				} // while
				if(found == false){
					printf("Could not find: %s\n", toSearch);
				}
				else{
					printf("\nFound %d Matching Contact(s).\n", foundCount);
				}

				break;

			default:
				printf("Not a valid number, re-enter.\n");
				break;
		} // switch
		printf("\n");
	}while(choice != 0);	
}

// flips all the bits in a string using the bitwise complement operator
void encryptWord(char *ptr){
	while(*ptr != '\0'){
		*ptr = ~*ptr;
		ptr++;
	}
}

// iterates through each contact in the list and send each fiedl to the encryptWord function
void encryptFile(){
	struct contact *curr;
	char *ptr;
	int encryptCount = 0;
	curr = listHead->next;

	while(curr->next != NULL){
		//printf("%s,%s,%s,%s,%s\n", curr->fName, curr->sName, curr->phone, curr->email, curr->company);
		encryptWord(&curr->fName[0]);
		encryptWord(&curr->sName[0]);
		encryptWord(&curr->phone[0]);
		encryptWord(&curr->email[0]);
		encryptWord(&curr->company[0]);
		//printf("%s,%s,%s,%s,%s\n\n", curr->fName, curr->sName, curr->phone, curr->email, curr->company);
		curr = curr->next;
		encryptCount++;
	}

	if(encryptCount > 0){
		if(encrypted){
			printf("%d contacts decrypted!\n", encryptCount);
			encrypted = false;
		}
		else{
			printf("%d contacts encrypted!\n", encryptCount);
			encrypted = true;
		}
	}
	else printf("No contacts to En/Decrypt! :(\n");
}	

//  Saves the contacts (list) in .csv format
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