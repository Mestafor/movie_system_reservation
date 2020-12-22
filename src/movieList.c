#include "./movieList.h"

#define MOVIES_STORE "movieList.txt"

Movie* MovieCreate(int id, char* title, int price, int capacity) {
    Movie * movie = malloc(sizeof(Movie));

    movie->id = id;
    movie->price = price;
    movie->capacity = capacity;

    movie->title = malloc(sizeof(char) * strlen(title) + 1);
    if(movie->title == NULL) {
        perror("malloc");
    }
    strcpy(movie->title, title);

    return movie;
}

void MovieListCreate(MovieList ** movieList) {
    (*movieList) = malloc(sizeof(MovieList));
    if((*movieList) == NULL) {
        perror("malloc");
    }
    (*movieList)->length = 0;
    (*movieList)->head = NULL;
}

void MovieListDestroy(MovieList* movieList) {
    Movie *  movie = NULL;

    while(movieList->head) {
        movie = movieList->head;
        movieList->head = movieList->head->next;
        movieList->length--;

        free(movie->title);
        free(movie);
    }

    free(movieList);
}

void MovieListInsert(MovieList * movieList, Movie * movie) {
    if(movieList->length == 0) {
        movie->next = NULL;
    } else {
        movie->next = movieList->head;
    }

    movieList->head = movie;
    movieList->length++;
}

void MovieListPrint(MovieList * movieList, FILE * fStream) {
    if(movieList->length == 0) {
        fprintf(stdout, "No movies available.\n");
    } else {
        Movie * movie = movieList->head;
        while(movie) {
            fprintf(fStream, "%d,%s,%d,%d \n", movie->id, movie->title, movie->price, movie->capacity);
            movie = movie->next;
        }
    }
}

void MovieListDelete(MovieList *  movieList, int id) {
    if(movieList->length == 0) {
        printf("List already empty...");
    } else {
        int found = 0;
        Movie * movie = movieList->head;
        Movie * prevMovie = movieList->head;
        while(movie) {
            if(movie->id == id) {
                // Only if elements is first
                if(prevMovie->id == movie->id) {
                    movieList->head = movieList->head->next;
                } else {
                    prevMovie->next = movie->next;
                }

                free(movie->title);
                free(movie);

                printf("\nElement %d was deleted \n", id);
                movieList->length--;
                found++;
                break;
            } else {
                prevMovie = movie;
                movie = movie->next;
            }
        }

        if(found == 0) {
            printf("Movie not found. \n");
        }
    }
}

MovieListBookData MovieListBook(MovieList * movieList, int id, int tickets) {
    MovieListBookData movieListbookdata;

    if(movieList->length == 0) {
        printf("List already empty...");
    } else {
        Movie * movie = movieList->head;
        for(int i = 0; i < movieList->length; i++) {
            if(id == movie->id) {
                movieListbookdata.movieName = movie->title;
                if(movie->capacity < tickets) {
                    movieListbookdata.count = -1;
                    return movieListbookdata; // means to many tickets
                } else {
                    movie->capacity -= tickets;
                    movieListbookdata.count = tickets;
                    movieListbookdata.price = tickets * movie->price;
                    return movieListbookdata;
                }
            }
            movie = movie->next;
        }
    }

    movieListbookdata.count = 0;
    return movieListbookdata; // means nothing fount
}

void storeMovies(MovieList *movieList)
{
    if (movieList->length > 0)
    {
        FILE *file = fopen(MOVIES_STORE, "w"); // create file if does not exist
        MovieListPrint(movieList, file);
        fclose(file);
    }
}

void loadMovies(MovieList *movieList)
{
    FILE *file = fopen(MOVIES_STORE, "r");

    if (file == NULL)
    {
        file = fopen(MOVIES_STORE, "w");
    }
    else
    {
        char buffer[BUFFERSIZE];

        int id;
        char *title;
        int price;
        int capacity;

        while (fgets(buffer, BUFFERSIZE, file) != NULL)
        {
            if (buffer == "\n" || buffer == " " || buffer == "\t")
                continue;

            id = atoi(strtok(buffer, MOVIE_LIST_DELIM));
            title = strtok(NULL, MOVIE_LIST_DELIM);
            price = atoi(strtok(NULL, MOVIE_LIST_DELIM));
            capacity = atoi(strtok(NULL, MOVIE_LIST_DELIM));

            Movie *movie = MovieCreate(id, title, price, capacity);
            MovieListInsert(movieList, movie);

            memset(buffer, 0, BUFFERSIZE); // make sure the buffer is empty
        }
    }

    fclose(file);
}


// Test it
// int main( void ) {
//     MovieList * movieList;
//     MovieListCreate(&movieList);

//     MovieListPrint(movieList);

//     MovieListInsert(movieList, MovieCreate(movieList->length, "Title 1", 5, 100));
//     MovieListInsert(movieList, MovieCreate(movieList->length, "Title 2", 10, 110));
//     MovieListInsert(movieList, MovieCreate(movieList->length, "Title 3", 15, 130));
//     MovieListInsert(movieList, MovieCreate(movieList->length, "Title 4", 20, 150));

//     MovieListPrint(movieList);

//     MovieListDelete(movieList, 3);

//     MovieListPrint(movieList);

//     MovieListDelete(movieList, 0);

//     MovieListPrint(movieList);

//     MovieListDelete(movieList, 30);
//     MovieListDelete(movieList, 1);
//     MovieListDelete(movieList, 2);

//     MovieListDelete(movieList, 2);

//     MovieListDestroy(movieList);
// }
