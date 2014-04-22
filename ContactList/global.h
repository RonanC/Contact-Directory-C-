//#define NULL 0

struct contact{
	char fName[15];
	char sName[15];
	char phone[15];
	char email[25];
	char company[25];
	struct contact *next;
	struct contact *prev;
};
