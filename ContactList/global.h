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
