#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./main.h"

#define TICKET_LIST_DELIM ","

typedef struct Ticket {
    char * movieName;
    int movieId;
    int count;
    int pending; // 0 payment done. 1 payment pending
    struct Ticket * next;
} Ticket;

typedef struct TicketList {
    int length;
    Ticket * head;
} TicketList;

Ticket * TicketCreate(char *, int, int, int);
void TicketListCreate(TicketList **);
void TicketListDestroy(TicketList *);
void TicketListInsert(TicketList*, Ticket *);
void TicketListPrint(TicketList *);
void TicketListOutput(TicketList *, char *);
void TicketListInput(TicketList *, char *);
void TicketListDelete(TicketList *, int);