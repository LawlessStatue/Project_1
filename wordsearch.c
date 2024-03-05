#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 100

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
void toUpperCase(char* word);
void initializePath();
void createPath(int x, int y);
void theGreatReset();
void finalSearchPath();
void directionsAroundChar(char** arr, char* word, int row, int col);
void directionsReverseChar(char** arr, char* word, int row, int col);
void startingLetter(char** arr, char* word, int row, int col);

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

void toUpperCase(char *word) {
    for (int i = 0; *(word + i) != '\0'; i++) {
        if (*(word + i) >= 'a' && *(word + i) <= 'z') {
            *(word + i) = *(word + i) - 32;
        }
        
    }
}
// searches for the starting letter of the word if it exists...
void startingLetter(char** arr, char* word, int row, int col) {
    if (startingX == -1 && startingY == -1) {
        for (int i = 0; i < bSize; i++) {
            for (int  j = 0; j < bSize; j++) {
                if (*(word) == *(*(arr + i) + j)) {
                    startingX = j;
                    startingY = i;

                    initialX = j;
                    initialY = i;

                    wordFound++;
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
            directionsAroundChar(arr, word, row, col);
        }
    }

    else { 
        int row = startingX + 1;
        int col = startingY;

        while (row != bSize) {
            if (col == bSize) {
                col = 0;
                row++;
            }
            if (*(word) == *(*(arr + row) + col)) {
                startingX = col;
                startingY = row;

                initialX = col;
                initialY = row;
                wordFound++;

                break;
            } 
            row++;   
        }
        if (*(word) == *(*(arr + initialY) + initialX)) {
            createPath(startingX, startingY);
            directionsAroundChar(arr, word, row, col);
        }
        else {
            wordFound = 0;
        }
    }
}

void createPath(int x, int y) { // creates a makrer if a letter has been found. 
    int total = *(*(searchPath + y) + x);
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
    *(*(searchPath + y) + x) = total;
}

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

void finalSearchPath() {

}

void directionsAroundChar(char** arr, char* word, int row, int col) { // checks around chars

} 

void directionsReverseChar(char** arr, char* word, int row, int col) { // checks backtracking route and around chars

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

void searchPuzzle(char** arr, char* word) { // parameter points to an aray of pointers, as well as uses the word that user gives.
    // This function checks if arr contains the search word. If the 
    // word appears in arr, it will print out a message and the path 
    // as shown in the sample runs. If not found, it will print a 
    // different message as shown in the sample runs.
    toUpperCase(word);

    initializePath();

    // Iterate through each cell of the array grid to search common elements
    for (int i = 0; i < bSize; i++) {
        *(arr + i) = (char*)malloc(bSize * sizeof(char));
        for (int j = 0; j < bSize; j++) {
            if ((*(*(arr + i) + j)) == *word) {
                count = -1;
            }
        }
    }
}
