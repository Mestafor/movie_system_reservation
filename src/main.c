#include "./movieList.h"
#include "./ticketList.h"


int main(int argc, char *argv[])
{
    /**
     * Modes:
     * 0 - user
     * 1 - admin
     * 2 - moderator
     * */

    enum Modes
    {
        USER,
        ADMIN,
        MODERATOR
    };

    int mode = 0;
    if (argc > 1)
    {
        if (strcmp(argv[1], "-a") == 0)
        {
            mode = ADMIN;
        }
        else if (strcmp(argv[1], "-m") == 0)
        {
            mode = MODERATOR;
        }
    }

    MovieList *movieList;
    MovieListCreate(&movieList);
    loadMovies(movieList);

    // Program loop
    short isRunning = 1;

    char title[BUFFERSIZE];
    int price;
    int capacity;
    char buffer[BUFFERSIZE];

    TicketList * ticketList;

    char username[BUFFERSIZE];
    if(mode == USER) {
        printf("What is your username? \n");
        fgets(username, BUFFERSIZE, stdin);
        strtok(username, "\n");
        printf("Hello, %s\n", username);
    }

    while (isRunning > 0)
    {
        printf("\n");
        memset(buffer,0,BUFFERSIZE);

        switch (mode)
        {
        case ADMIN:
        {
            printf("1. Create new Movie\n");
            printf("2. Delete Movie\n");
            printf("3. View Movies\n");
            printf("4. Exit\n");

            int answer = 0;
            fgets(buffer,BUFFERSIZE, stdin);
            answer = atoi(buffer);
            switch (answer)
            {
            case 1:
            {
                // Create
                printf("Please enter movie name.\n");
                fgets(title, BUFFERSIZE, stdin);
                strtok(title, "\n");

                printf("What is the ticket price?\n");
                scanf(" %d", &price);

                printf("What is capacity in the movie theater?\n");
                scanf(" %d", &capacity);

                Movie *movie = MovieCreate(movieList->length, title, price, capacity);
                MovieListInsert(movieList, movie);

                break;
            }
            case 2:
            {
                // Delete
                int id;
                printf("Please enter movie ID you would like to delete:\n");
                scanf(" %d", &id);
                MovieListDelete(movieList, id);
                break;
            }
            case 3:
            {
                // View
                printf("\n");
                MovieListPrint(movieList, stdout);
                printf("\n");
                break;
            }
            case 4:
            {
                isRunning = -1;
                break;
            }
            default:
                printf("Unknown command...\n");
                break;
            }

            break;
        }
        case MODERATOR:
        {
            printf("1. View movies\n");
            printf("2. View user tickets\n");
            printf("3. Delete user tickets\n");
            printf("4. Exit\n");

            int answer = 0;
            fgets(buffer,BUFFERSIZE, stdin);
            answer = atoi(buffer);
            switch (answer)
            {
            case 1:
            {
                // View movies
                printf("\n");
                MovieListPrint(movieList, stdout);
                printf("\n");
                break;
            }
            case 2:
            {
                TicketListCreate(&ticketList);
                // View user tickets
                printf("What is the username? \n");
                fgets(username, BUFFERSIZE, stdin);
                strtok(username, "\n");
                TicketListInput(ticketList, username);
                TicketListPrint(ticketList);
                TicketListDestroy(ticketList);
                break;
            }
            case 3:
            {
                TicketListCreate(&ticketList);
                // Delete user tickets
                printf("What is the username? \n");
                fgets(username, BUFFERSIZE, stdin);
                strtok(username, "\n");
                TicketListInput(ticketList, username);
                
                int id;
                printf("What is the movie id?\n");
                scanf(" %d", &id);
                getchar();

                TicketListDelete(ticketList, id);
                TicketListOutput(ticketList, username);
                TicketListDestroy(ticketList);
                break;
            }
            case 4:
            {
                isRunning = -1;
                break;
            }
            default:
                printf("Unknown command...\n");
                break;
            }

            break;
        }

        default: {
            TicketListCreate(&ticketList);
            TicketListInput(ticketList, username);
            printf("1. View Movies\n");
            printf("2. Book a ticket\n");
            printf("3. View tickets\n");
            printf("4. Exit\n");

            int answer = 0;
            fgets(buffer,BUFFERSIZE, stdin);
            answer = atoi(buffer);

            switch (answer)
            {
                case 1:
                {
                    // View
                    printf("\n");
                    MovieListPrint(movieList, stdout);
                    printf("\n");

                    break;
                }
                case 2:
                {
                    // Booking
                    int id;
                    printf("Please enter the movie id you would like to book:\n");
                    scanf(" %d", &id);

                    int tickets = 0;
                    printf("Please enter count of tickets you want to buy:\n");
                    int notValid = 1;
                    while(notValid) {
                        scanf(" %d", &tickets);
                        getchar();
                        if(tickets > 0) {
                            notValid = 0;
                        } else {
                            printf("Not valid tickets number...\n");
                        }
                    }

                    MovieListBookData reservedTickets = MovieListBook(movieList, id, tickets);

                    if(reservedTickets.count == -1) {
                        printf("\nNot enough seats available\n");
                    } else if(reservedTickets.count == 0) {
                        printf("\nMovie does not found\n");
                    } else {
                        printf("\nBooked %d tickets for movie %s \n", reservedTickets.count, reservedTickets.movieName);
                        printf("The price of tickets is $%d\n", reservedTickets.price);
                        printf("Would you like pay online(0) or in the theater(1)?\n");
                        fgets(buffer,BUFFERSIZE,stdin);
                        int pending = atoi(buffer);
                        TicketListInsert(ticketList, TicketCreate(reservedTickets.movieName, id, tickets, pending));
                    }

                    break;
                }
                case 3:
                {
                    // Tikets
                    TicketListPrint(ticketList);
                    
                    break;
                }
                case 4:
                {
                    isRunning = -1;
                    break;
                }
                default:
                    printf("Unknown command...\n");
            }

            TicketListOutput(ticketList, username);
            TicketListDestroy(ticketList);

            memset(buffer, 0, BUFFERSIZE);
        }
            
        }
    }

    storeMovies(movieList);
    MovieListDestroy(movieList);

    printf("Closing program\n");
}
