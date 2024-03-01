#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 100

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
void toUpperCase(char* word);
int searchPath(char** arr, char* word, int row, int col);
int bSize;

typedef struct {
    int row;
    int col;
} Move;

Move* stack = NULL; // Pointer is added to mark position for the stack.
int top = -1; // This is to identify the top of the stack. 

void push(Move location) {
    if (top < MAX_SIZE - 1) { // Checks if stack is not full
        top++;
        stack[top] = location; // Add location to the top of stack
    }
}

void initializeMemory() {
    stack = (Move*)malloc(MAX_SIZE * sizeof(Move));
}

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
}

void toUpperCase(char* word) {
    while (*word) {
        if (*(word) >= 'a' && *(word) <= 'z') {
            *(word) = *(word) - ('a' + 'A');
        }
    }
}

int searchPath(char** arr, char* word, int row, int col) {
    //If the word has been matched, return true
    if (*word == '\0') {
        return 1;
    }
    //check if the cell we are in is within the puzzle. 
    if (row < bSize && row >= 0 && col < bSize && col >= 0) {
        if ((*(*arr + row) + col) == *word) { // checks if our current row and col coordinates are equal to word chars
            // we are reassigning our position of the Move stack to a new coordinate/cell within the puzzle
            Move location = {row, col}; // these are not square brackets, OK ?
            push(location);
            
            // have the cell recursively check through each direction for the next character. 
            if (searchPath(arr, word + 1, row + 1, col) || searchPath(arr, word + 1, row - 1, col) ||
            searchPath(arr, word + 1, row, col + 1) || searchPath(arr, word + 1, row, col - 1) || 
            searchPath(arr, word + 1, row - 1, col - 1) || searchPath(arr, word + 1, row - 1, col + 1) ||
            searchPath(arr, word + 1, row + 1, col - 1) || searchPath(arr, word + 1, row + 1, col + 1)) {
                return 1;
            }
            // In case path ends, we need to backtrack to initial position
            top--;
        }
    }
    return 0;
}

void searchPuzzle(char** arr, char* word) { // parameter points to an aray of pointers, as well as uses the word that user gives.
    // This function checks if arr contains the search word. If the 
    // word appears in arr, it will print out a message and the path 
    // as shown in the sample runs. If not found, it will print a 
    // different message as shown in the sample runs.
    toUpperCase(word);
    
    int valueFound = 0;

    // Iterate through each cell of the array grid to search common elements
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            if ((*(*arr + i) + j) == *word) {
                initializeMemory();
                top = -1;
            }
        }
    }
}
