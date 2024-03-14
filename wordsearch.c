/*

PARTNER: Parthib Roy

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
void toUpperCase(char* word);
// path creation/manipulation
void initializePath();
void createPath(int x, int y);
void theGreatReset();
void finalSearchPath();
// searching helper functions
void directionsAroundChar(char** arr, char* word, int x, int y);
void ReverseChar(char** arr, char* word, int x, int y);
void startingLetter(char** arr, char* word, int x, int y);

// Realized that recursively checking with one conditional was not going to work
// Had to restart work in terms of interative conditional letter checking

int bSize;
int **searchPath;
int startingX = -1;
int startingY = -1;
int initialX = 0;
int initialY = 0;
int wordFound = 0; // determines if wordFound is true or not, initial false.
int count = 0; // Checks letter count

// Main function, DO NOT MODIFY 	
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);
    
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) {
            fscanf(fptr, "%c ", *(block + i) + j);            
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);

    printf("Enter the word to search: ");
    scanf("%s", word);
    
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
    
    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);
    
    return 0;
}

void printPuzzle(char** arr) {  //** means pointer to a pointer, FUTURE REFERENCE
	// This function will print out the complete puzzle grid (arr). 
    // It must produce the output in the SAME format as the samples 
    // in the instructions.
    for (int i = 0; i < bSize; i++) {
        for(int j = 0; j < bSize; j++) {
            printf("%c ", *(*(arr + i) + j)); // Space added to follow output of code
        }
        printf("\n"); 
    }
    printf("\n");
}

void searchPuzzle(char** arr, char* word) { // parameter points to an aray of pointers, as well as uses the word that user gives.
    // This function checks if arr contains the search word. If the 
    // word appears in arr, it will print out a message and the path 
    // as shown in the sample runs. If not found, it will print a 
    // different message as shown in the sample runs.
    toUpperCase(word);

    initializePath();

    int occurrences = 0;

    startingLetter(arr, word, startingX, startingY);

    if (wordFound == 0) {
        printf("Word not found!\n");
    }
    else {
        printf("Word found!\n");
        finalSearchPath();
    }
}

void toUpperCase(char *word) {
    for (int i = 0; *(word + i) != '\0'; i++) {
        if (*(word + i) >= 'a' && *(word + i) <= 'z') {
            *(word + i) = *(word + i) - 32;
        }
        
    }
}
// searches for the starting letter of the word if it exists 
void startingLetter(char** arr, char* word, int x, int y) {
    if (startingX == -1 && startingY == -1) {
        for (int i = 0; i < bSize; i++) {
            for (int  j = 0; j < bSize; j++) {
                if (*(word) == *(*(arr + i) + j)) {
                    initialX = j;
                    initialY = i;
                    
                    startingX = j;
                    startingY = i;

                    wordFound++; //letters instead
                    break;
                }
                if (i == bSize - 1 && j == bSize - 1) {
                    wordFound = 0;
                }
            }
            if (wordFound > 0) {
                break;
            }
        }

        if (*(word) == *(*(arr + initialY) + initialX)) {
            createPath(startingX, startingY);
            directionsAroundChar(arr, word, startingX, startingY);
        }
    }

    else { 
        int i = startingY;
        int j = startingX + 1;

        while (i != bSize) {
            if (j == bSize) {
                j = 0;
                i++;
            }
            if (*(word) == *(*(arr + i) + j)) {
                startingX = j;
                startingY = i;

                initialX = j;
                initialY = i;
                wordFound++;

                break;
            } 
            j++;   
        }
        if (*(word) == *(*(arr + initialY) + initialX)) {
            createPath(startingX, startingY);
            directionsAroundChar(arr, word, startingX, startingY);
        }
        else {
            wordFound = 0;
        }
    }
}
// Created to make a marker on the specific point within the grid if a letter has been found.
void createPath(int x, int y) { // creates a marker if a letter has been found. 
    int total = *(*(searchPath + startingY) + startingX);
    int temp = 1;
    count++;

    for (int i = 0; i < 100; i++) {
        if (temp < total) {
            temp *= 10;
        }
        else if (temp > total) {
            total = temp * count + total;
            break;
        }
    }
    *(*(searchPath + startingY) + startingX) = total;
}
// prints out the final searchpath.
void finalSearchPath() {
    printf("Printing the search path:\n");
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            if (*(*(searchPath + i) + j) < 10) {
                printf("%d       ", *(*(searchPath + i) + j));
            }
            else if (*(*(searchPath + i) + j) >= 10 && *(*(searchPath + i) + j) < 100) {
                printf("%d      ", *(*(searchPath + i) + j));
            }
            else if (*(*(searchPath + i) + j) >= 100 && *(*(searchPath + i) + j) < 1000) {
                printf("%d     ", *(*(searchPath + i) + j));
            }
            else if (*(*(searchPath + i) + j) >= 1000 && *(*(searchPath + i) + j) < 10000) {
                printf("%d    ", *(*(searchPath + i) + j));
            }
            else if (*(*(searchPath + i) + j) >= 10000 && *(*(searchPath + i) + j) < 100000) {
                printf("%d   ", *(*(searchPath + i) + j));
            }
            else if (*(*(searchPath + i) + j) >= 100000 && *(*(searchPath + i) + j) < 1000000) {
                printf("%d  ", *(*(searchPath + i) + j));
            }
            else if (*(*(searchPath + i) + j) >= 1000000 && *(*(searchPath + i) + j) < 10000000) {
                printf("%d ", *(*(searchPath + i) + j));
            }
            else {
                printf("%d", *(*(searchPath + i) + j));
            }
        }
        printf("\n");
    }
    printf("\n");
}
// Resets the searchpath if it fails, meant for resetting failed paths.
void theGreatReset() {
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            *(*(searchPath + i) + j) = 0;
        }
    }
    startingX = initialX;
    startingY = initialY;
    wordFound = 0;
    count = 0;
} 
// This search function makes a check on all directions for characters and stores a path indicator/marker
void directionsAroundChar(char** arr, char* word, int x, int y) { // checks around chars
    if (startingY - 1 >= 0 && startingX - 1 >= 0 && *(word + count) != '\0') { // CHECKS TOPLEFT MOST POSITION
        if (*(word + count) == *(*(arr + (startingY - 1)) + (startingX - 1))) {
            startingY--;
            startingX--;
            createPath(startingX, startingY);
            directionsAroundChar(arr, word, startingX, startingY);
        }
    }
    if (startingY - 1 >= 0 && *(word + count) != '\0') { // CHECKS TOP MOST POSITION
        if (*(word + count) == *(*(arr + (startingY - 1)) + startingX)) {
            startingY--;
            createPath(startingX, startingY);
            directionsAroundChar(arr, word, startingX, startingY);
        }
    }
    if (startingY - 1 >= 0 && startingX + 1 < bSize && *(word + count) != '\0') { // CHECKS TOPRIGHT MOST POSITION
        if (*(word + count) == *(*(arr + (startingY - 1)) + (startingX + 1))) {
            startingY--;
            startingX++;
            createPath(startingX, startingY);
            directionsAroundChar(arr, word, startingX, startingY);
        }
    }
    if (startingX + 1 < bSize && *(word + count) != '\0') { // CHECKS RIGHT MOST POSITION
        if (*(word + count) == *(*(arr + startingY) + (startingX + 1))) {
            startingX++;
            createPath(startingX, startingY);
            directionsAroundChar(arr, word, startingX, startingY);
        }
    }
    if (startingY + 1 < bSize && startingX + 1 < bSize && *(word  + count) != '\0') { // CHECKS BOTTOM RIGHT MOST POSITION
        if (*(word + count) == *(*(arr + (startingY + 1)) + (startingX + 1))) {
            startingY++;
            startingX++;
            createPath(startingX, startingY);
            directionsAroundChar(arr, word, startingX, startingY);
        }
    }
    if (startingY + 1 < bSize && *(word + count) != '\0') { // CHECKS BOTTOM MOST POSTION
        if (*(word + count) == *(*(arr + (startingY + 1)) + startingX)) {
            startingY++;
            createPath(startingX, startingY);
            directionsAroundChar(arr, word, startingX, startingY);
        }
    }
    if (startingY + 1 < bSize && startingX - 1 >= 0 && *(word + count) != '\0') { // CHECKS BOTTOM LEFT MOST POSITION
        if (*(word + count) == *(*(arr + (startingY + 1)) + (startingX - 1))) {
            startingY++;
            startingX--;
            createPath(startingX, startingY);
            directionsAroundChar(arr, word, startingX, startingY);
        }
    }
    if (startingX - 1 >= 0 && *(word + count) != '\0') { // CHECKS LEFT MOST POSITION
        if (*(word + count) == *(*(arr + startingY) + (startingX - 1))) {
            startingX--;
            createPath(startingX, startingY);
            directionsAroundChar(arr, word, startingX, startingY);
        }
    }
    if (*(word + count) != '\0') {
        theGreatReset();
        startingX = initialX;
        startingY = initialY;
        createPath(startingX, startingY);
        ReverseChar(arr, word, startingX, startingY);
    }
    else {
        wordFound = 1;
    }
}
// This function  was added to check for a backtracking path in case a search path had failed. 
void ReverseChar(char** arr, char* word, int x, int y) { // checks backtracking route and around chars
    if (startingX - 1 >= 0 && *(word + count) != '\0') { // CHECKS LEFT MOST POSITION
        if (*(word + count) == *(*(arr + startingY) + (startingX - 1))) {
            startingX--;
            createPath(startingX, startingY);
            ReverseChar(arr, word, startingX, startingY);
        }
    }
    if (startingY + 1 < bSize && startingX - 1 >= 0 && *(word + count) != '\0') { // CHECKS BOTTOM LEFT MOST POSITION
        if (*(word + count) == *(*(arr + (startingY + 1)) + (startingX - 1))) {
            startingY++;
            startingX--;
            createPath(startingX, startingY);
            ReverseChar(arr, word, startingX, startingY);
        }
    }
    if (startingY + 1 < bSize && *(word + count) != '\0') { // CHECKS BOTTOM MOST POSTION
        if (*(word + count) == *(*(arr + (startingY + 1)) + startingX)) {
            startingY++;
            createPath(startingX, startingY);
            ReverseChar(arr, word, startingX, startingY);
        }
    }
    if (startingY + 1 < bSize && startingX + 1 < bSize && *(word + count) != '\0') { // CHECKS BOTTOM RIGHT MOST POSITION
        if (*(word + count) == *(*(arr + (startingY + 1)) + (startingX + 1))) {
            startingY++;
            startingX++;
            createPath(startingX, startingY);
            ReverseChar(arr, word, startingX, startingY);
        }
    }
    if (startingX + 1 < bSize && *(word + count) != '\0') { // CHECKS RIGHT MOST POSITION
        if (*(word + count) == *(*(arr + startingY) + (startingX + 1))) {
            startingX++;
            createPath(startingX, startingY);
            ReverseChar(arr, word, startingX, startingY);
        }
    }
    if (startingX + 1 < bSize && startingY - 1 >= 0 && *(word + count) != '\0') { // CHECKS TOPRIGHT MOST POSITION
        if (*(word + count) == *(*(arr + (startingY - 1)) + (startingX + 1))) {
            startingY--;
            startingX++;
            createPath(startingX, startingY);
            ReverseChar(arr, word, startingX, startingY);
        }
    }
    if (startingY - 1 >= 0 && *(word + count) != '\0') { // CHECKS TOP MOST POSITION
        if (*(word + count) == *(*(arr + (startingY - 1)) + startingX)) {
            startingY--;
            createPath(startingX, startingY);
            ReverseChar(arr, word, startingX, startingY);
        }
    }
    if (startingX - 1 >= 0 && startingY - 1 >= 0 && *(word + count) != '\0') { // CHECKS TOPLEFT MOST POSITION
        if (*(word + count) == *(*(arr + (startingY - 1)) + (startingX - 1))) {
            startingY--;
            startingX--;
            createPath(startingX, startingY);
            ReverseChar(arr, word, startingX, startingY);
        }
    }
    if (*(word + count) != '\0') {
        theGreatReset();
        startingX = initialX;
        startingY = initialY;
        startingLetter(arr, word, startingX, startingY);
    }
    else {
        wordFound = 1;
    }
}

void initializePath() {
    searchPath = (int**)malloc(bSize * sizeof(int*));
    for (int i = 0; i < bSize; i++) {
        *(searchPath + i) = (int*)malloc(bSize * sizeof(int));
    }

    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            *(*(searchPath + i) + j) = 0;
        }
    }
}
