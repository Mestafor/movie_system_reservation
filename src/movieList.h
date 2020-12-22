#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./main.h"

#define MOVIE_LIST_DELIM ","

typedef struct Movie {
    int id;
    char* title;
    int price;
    int capacity;
    struct Movie * next;
} Movie;

typedef struct MovieList {
    int length;
    Movie * head;
} MovieList;

typedef struct MovieListBookData {
    int count;
    char * movieName;
    int price;
} MovieListBookData;

// create movie
Movie * MovieCreate(int, char*, int, int);
// initialize a movie list
void MovieListCreate(MovieList **);
// destroy a movie list
void MovieListDestroy(MovieList *);
// insert a movie into a movie list
void MovieListInsert(MovieList *, Movie *);
// print movie list
void MovieListPrint(MovieList *, FILE *);
// delete a movie from the list
void MovieListDelete(MovieList *, int);
// Book a movie
MovieListBookData MovieListBook(MovieList *, int, int);

void storeMovies(MovieList *);
void loadMovies(MovieList *);
