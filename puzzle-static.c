// header files
#include <stdio.h> // to perform IO operations
#include <string.h> // to perform string manipulation functions
#include <stdbool.h> // to perform boolean operations

// defining the Constant value N for static array sizes
#define N 50

// declaring and initializing global variables
int num_words;            // number of words
int cols = 0, rows = 0;   // number of columns and rows in the grid
char puzzle[N * N] = {0}; // 1D array to store spaces of the grid
char words[N][N] = {0};   // 2D array to store spaces of the words

// function prototypes
void getMatrix(char *matrix, int *cols);
void getWords();
int puzzleCheck(char matrix[N * N], int rows, int cols);
int wordCheck(char matrix[N][N]);
bool fits(char *word, int row, int col, int row_dir, int col_dir, int cols);
void place(char *word, int row, int col, int row_dir, int col_dir);
void getBackup(char *word, int row, int col, int row_dir, int col_dir, char *backup);
bool solve(int k);
void printPuzzle();

// function definitions

// main function
int main()
{
    // get input for the grid
    getMatrix(puzzle, &cols);

    // get input for the words
    getWords();

    // if invalid grid or invalid words entered
    if (puzzleCheck(puzzle, rows, cols) || wordCheck(words))
    {
        printf("INVALID INPUT");
        return 0;
    }

    // solve the puzzle
    printPuzzle();

    return 0;
}

// get the input of the grid into a 1D array from the user
void getMatrix(char *matrix, int *cols)
{
    for (int i = 0; i < N; i++)
    {
        char text[N];
        fgets(text, N, stdin);
        if (text[0] == '\n')  // continues to read lines of the matrix until a newline character is entered
        {
            matrix[i * N] = '\0';
            break;
        }
        int newline_pos = strlen(text);
        strncpy(&matrix[i * N], text, newline_pos); // to copy each line of the matrix into the "matrix" array
        *cols = strlen(text) - 1; // to remove the newline character from the end of each line
        rows++;
    }
}

// get the words into a 2D array from the user
void getWords()
{
    int i = 0;
    char word[N];
    while (fgets(word, N, stdin))
    {
        if (word[0] == '\n') // continues to read words until a newline character is entered
            break;

        word[strcspn(word, "\n")] = 0; // to remove the newline character from the end of each word
        strcpy(words[i], word); // to copy each word into the "words" array
        i++;
    }
    num_words = i;
}

// checking for invalid grid types enterd by the user
int puzzleCheck(char matrix[N * N], int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // checking if the grid consists of the correct character types ('*', '#', A-Z and a-z)
            if (matrix[i * N + j] != '*' && matrix[i * N + j] != '#' && (matrix[i * N + j] > 122 || matrix[i * N + j] < 65 || (matrix[i * N + j] > 90 && matrix[i * N + j] < 97)))
                return 1; // return 1 for the invalid grid types 
        }
    }

    return 0; 
}

// checking for invalid words entered by the user
int wordCheck(char matrix[N][N])
{
    for (int i = 0; i < N; i++)
    {
        if (matrix[i][0] == 0)
        {
            matrix[i][0] = 0;
            break;
        }

        if (strlen(matrix[i]) == 1)
            return 1;

        // checking for valid characters and return 1 for the invalid character type
        for (int j = 0; j < N; j++)
        {
            if (matrix[i][j] == 10 || matrix[i][j] == 0) 
                break; // breaks the loop if the matrix is empty or if it encounters a newline character
            if (matrix[i][j] > 'z' || matrix[i][j] < 'A') 
                return 1; // return 1 for words consisting othe than uppercase or lowercase letters
            if (matrix[i][j] > 'Z' && matrix[i][j] < 'a')
                return 1; // return 1 for the characters between 'Z' and 'a' ( [, \, ], ^, _, ')
        }
    }
    if (num_words < 1)
        return 1; // return 1, if the number of words are less than 1

    return 0;
}

// check if the word fits in the grid at the given position and orientation (row_dir --> row direction and col_dir --> column direction)
bool fits(char *word, int row, int col, int row_dir, int col_dir, int cols) 
{
    // check if the word fits within the grid boundaries
    if ((row + row_dir * (strlen(word) - 1) < 0) || (row + row_dir * (strlen(word) - 1) >= rows) || (col + col_dir * (strlen(word) - 1) < 0) || (col + col_dir * (strlen(word) - 1) >= cols))
        return false;

    // check if the word fits in the grid
    for (int i = 0; i < strlen(word); i++)
    {
        if ((puzzle[(row + i * row_dir) * N + col + i * col_dir] != '#') && (puzzle[(row + i * row_dir) * N + col + i * col_dir] != word[i]))
            return false;
    }

    return true;
}

// place the word in the grid (row_dir --> row direction and col_dir --> column direction)
void place(char *word, int row, int col, int row_dir, int col_dir)
{
    for (int i = 0; i < strlen(word); i++)
        if (puzzle[(row + i * row_dir) * N + col + i * col_dir] != '*')
            puzzle[(row + i * row_dir) * N + col + i * col_dir] = word[i];
}

// keeping a backup of the previous state
void getBackup(char *word, int row, int col, int row_dir, int col_dir, char *backup)
{
    for (int i = 0; i < strlen(word); i++)
        backup[i] = puzzle[(row + i * row_dir) * N + col + i * col_dir];
}

// recursive function to solve the puzzle using backtracking
bool solve(int k)
{
    // base case: all words have been placed in the puzzle
    if (k == num_words)
        return true;

    // try placing the current word in all positions and orientations
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (puzzle[i * N + j] == '#' || puzzle[i * N + j] == words[k][0])
            {
                // try horizontal orientation
                if (fits(words[k], i, j, 0, 1, cols))
                {
                    char backupTxt[strlen(words[k])];
                    getBackup(words[k], i, j, 0, 1, backupTxt);
                    place(words[k], i, j, 0, 1);
                    if (solve(k + 1))
                        return true;
                    
                    place(backupTxt, i, j, 0, 1); // undo placement
                }

                // try vertical orientation
                if (fits(words[k], i, j, 1, 0, cols))
                {
                    char backupTxt[strlen(words[k])];
                    getBackup(words[k], i, j, 1, 0, backupTxt);
                    place(words[k], i, j, 1, 0);
                    if (solve(k + 1))
                        return true;
                
                    place(backupTxt, i, j, 1, 0); // undo placement
                }
            }
        }
    }

    // the current word cannot be placed in the puzzle
    return false;
}

// printing the final grid with the filled words to the screen
void printPuzzle()
{
    if (solve(0))
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
                printf("%c", puzzle[i * N + j]);
            printf("\n");
        }
    }

    // prints 'IMPOSSIBLE' when the words cannot be filled with given words considering the lengths of words and empty spaces
    else
        printf("IMPOSSIBLE"); 
}

