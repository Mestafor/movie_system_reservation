#include "./ticketList.h"

Ticket* TicketCreate(char* movieName, int movieId, int count, int pending) {
    Ticket * ticket = malloc(sizeof(Ticket));

    ticket->movieId = movieId;
    ticket->count = count;
    ticket->pending = pending;

    ticket->movieName = malloc(sizeof(char) * strlen(movieName) + 1);
    if(ticket->movieName == NULL) {
        perror("malloc");
    }
    strcpy(ticket->movieName, movieName);

    return ticket;
}

void TicketListCreate(TicketList ** ticketList) {
    (*ticketList) = malloc(sizeof(TicketList));
    if((*ticketList) == NULL) {
        perror("malloc");
    } else {
        (*ticketList)->length = 0;
        (*ticketList)->head = NULL;
    }
}

void TicketListDestroy(TicketList * ticketList) {
    Ticket * ticket = NULL;
    while(ticketList->length) {
        ticket = ticketList->head;
        ticketList->head = ticket->next;
        free(ticket->movieName);
        free(ticket);
        ticketList->length--;
    }
    free(ticketList);
}

void TicketListInsert(TicketList * ticketList, Ticket * ticket) {
    if(ticketList->length == 0) {
        ticket->next = NULL;
    } else {
        ticket->next = ticketList->head;
    }

    ticketList->head = ticket;
    ticketList->length++;
}

void TicketListDelete(TicketList * ticketList, int id) {
    if(ticketList->length == 0) {
        printf("List already empty. Nothing to delete...\n");
    } else {
        int found = 0;
        Ticket * ticket = ticketList->head;
        Ticket * prevTicket = ticketList->head;
        while(ticket) {
            if(ticket->movieId == id) {
                if(ticket->pending) {
                    printf("Confirm local payment by pressing enter\n");
                    char buffer[BUFFERSIZE];
                    fgets(buffer, BUFFERSIZE, stdin);
                    if(buffer[0] != '\n') {
                        printf("Payment rejected.\n");
                        return;
                    }
                }

                // Only if elements is first
                if(prevTicket->movieId == ticket->movieId) {
                    ticketList->head = ticketList->head->next;
                } else {
                    prevTicket->next = ticket->next;
                }

                free(ticket->movieName);
                free(ticket);

                printf("\nElement %d was deleted \n", id);
                ticketList->length--;
                found++;
                break;
            } else {
                prevTicket = ticket;
                ticket = ticket->next;
            }
        }

        if(found == 0) {
            printf("Ticket not found. \n");
        } else {
            printf("Deleting complete!\n");
        }
    }
}

void TicketListPrint(TicketList * ticketList) {
    if(ticketList->length == 0) {
        printf("No tickets available.\n");
    } else {
        Ticket * ticket = ticketList->head;
        while(ticket) {
            printf("%d,%s,%d,%d\n", ticket->movieId, ticket->movieName, ticket->count, ticket->pending);
            ticket = ticket->next;
        }
    }
}

void TicketListOutput(TicketList * ticketList, char * userName) {
    char filename[256] = "./Users/";
    strcat(filename, userName);
    strcat(filename, ".txt");
    // Example ./Users/username.txt

    FILE * file = fopen(filename, "w");

    if(file == NULL) {
        printf("Cannot write to the file...\n");
    } else {
        Ticket * ticket = ticketList->head;
        while(ticket) {
            fprintf(file, "%d,%s,%d,%d\n", ticket->movieId, ticket->movieName, ticket->count, ticket->pending);
            ticket = ticket->next;
        }
    }

    fclose(file);
}

void TicketListInput(TicketList *ticketList, char * userName)
{
    char filename[256] = "./Users/";
    strcat(filename, userName);
    strcat(filename, ".txt");
    // Example ./Users/username.txt
    
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        file = fopen(filename, "w");
    }
    else
    {
        char buffer[BUFFERSIZE];
        memset(buffer, 0, BUFFERSIZE);

        int id;
        char *movieName;
        int count;
        int pending;

        while (fgets(buffer, BUFFERSIZE, file) != NULL)
        {
            if (buffer == "\n" || buffer == " " || buffer == "\t")
                continue;

            id = atoi(strtok(buffer, TICKET_LIST_DELIM));
            movieName = strtok(NULL, TICKET_LIST_DELIM);
            count = atoi(strtok(NULL, TICKET_LIST_DELIM));
            pending = atoi(strtok(NULL, TICKET_LIST_DELIM));

            Ticket *ticket = TicketCreate(movieName, id, count, pending);
            TicketListInsert(ticketList, ticket);

            memset(buffer, 0, BUFFERSIZE); // make sure the buffer is empty
        }
    }

    fclose(file);
}


// Test it
// int main(void) {
//     TicketList * ticketList;
//     TicketListCreate(&ticketList);

//     TicketListInput(ticketList, "user1");

//     TicketListInsert(ticketList, TicketCreate("Name movie1", ticketList->length, 5));
//     TicketListInsert(ticketList, TicketCreate("Name movie2", ticketList->length, 2));
//     TicketListInsert(ticketList, TicketCreate("Name movie3", ticketList->length, 6));

//     TicketListPrint(ticketList);
//     TicketListOutput(ticketList, "user1");

//     TicketListDelete(ticketList, 2);
//     TicketListPrint(ticketList);

//     TicketListDelete(ticketList, 2);
//     TicketListPrint(ticketList);

//     TicketListDelete(ticketList, 1);
//     TicketListPrint(ticketList);

//     TicketListOutput(ticketList, "user1");

//     TicketListDestroy(ticketList);
// }