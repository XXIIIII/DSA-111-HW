#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define MAX_GRID_SIZE 25
#define MAX_CLUES 30

// Cell states
#define EMPTY '0'
#define BLACK 'o'
#define WHITE '_'
#define OUT_OF_BOUNDS '1'

/**
 * Structure to hold clue information for rows and columns
 */
typedef struct {
    int segment_count[MAX_CLUES];           // Number of segments per row/column
    int segment_lengths[MAX_CLUES][MAX_CLUES]; // Length of each segment
} ClueData;

/**
 * Initializes the game board with empty cells and marks out-of-bounds areas
 * @param board: Game board to initialize
 * @param rows: Number of rows in the puzzle
 * @param cols: Number of columns in the puzzle
 */
void initialize_game_board(char board[MAX_GRID_SIZE][MAX_GRID_SIZE], int rows, int cols) {
    for (int i = 0; i < MAX_GRID_SIZE; i++) {
        for (int j = 0; j < MAX_GRID_SIZE; j++) {
            if (i >= rows || j >= cols) {
                board[i][j] = OUT_OF_BOUNDS;
            } else {
                board[i][j] = EMPTY;
            }
        }
    }
}

/**
 * Checks if all cells in the valid area are filled (no empty cells remain)
 * @param board: Game board to check
 * @param rows: Number of rows in the puzzle
 * @param cols: Number of columns in the puzzle
 * @return: true if all cells are filled, false otherwise
 */
bool is_board_complete(char board[MAX_GRID_SIZE][MAX_GRID_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}

/**
 * Counts consecutive black segments in a row or column
 * @param line: Array representing a row or column
 * @param length: Length of the line
 * @param segment_count: Output - number of segments found
 * @param segment_lengths: Output - lengths of each segment
 */
void count_black_segments(char line[], int length, int* segment_count, int segment_lengths[]) {
    *segment_count = 0;
    int current_segment_length = 0;
    
    for (int i = 0; i < length; i++) {
        if (line[i] == BLACK) {
            current_segment_length++;
        } else {
            if (current_segment_length > 0) {
                segment_lengths[*segment_count] = current_segment_length;
                (*segment_count)++;
                current_segment_length = 0;
            }
        }
    }
    
    // Handle case where line ends with black cells
    if (current_segment_length > 0) {
        segment_lengths[*segment_count] = current_segment_length;
        (*segment_count)++;
    }
}

/**
 * Validates if current board state matches all clues
 * @param board: Game board to validate
 * @param rows: Number of rows
 * @param cols: Number of columns
 * @param clues: Clue data structure
 * @return: true if valid, false otherwise
 */
bool validate_complete_board(char board[MAX_GRID_SIZE][MAX_GRID_SIZE], int rows, int cols, ClueData* clues) {
    // Validate all rows
    for (int i = 0; i < rows; i++) {
        int found_segments = 0;
        int found_lengths[MAX_CLUES] = {0};
        
        count_black_segments(board[i], cols, &found_segments, found_lengths);
        
        // Check if segment count matches
        if (found_segments != clues->segment_count[i]) {
            return false;
        }
        
        // Check if segment lengths match
        for (int j = 0; j < found_segments; j++) {
            if (found_lengths[j] != clues->segment_lengths[i][j]) {
                return false;
            }
        }
    }
    
    // Validate all columns
    for (int j = 0; j < cols; j++) {
        char column[MAX_GRID_SIZE];
        for (int i = 0; i < rows; i++) {
            column[i] = board[i][j];
        }
        
        int found_segments = 0;
        int found_lengths[MAX_CLUES] = {0};
        
        count_black_segments(column, rows, &found_segments, found_lengths);
        
        // Check if segment count matches (column clues start after row clues)
        if (found_segments != clues->segment_count[rows + j]) {
            return false;
        }
        
        // Check if segment lengths match
        for (int k = 0; k < found_segments; k++) {
            if (found_lengths[k] != clues->segment_lengths[rows + j][k]) {
                return false;
            }
        }
    }
    
    return true;
}

/**
 * Finds the first empty cell in the board
 * @param board: Game board to search
 * @param rows: Number of rows
 * @param cols: Number of columns
 * @return: Pointer to first empty cell, NULL if none found
 */
char* find_first_empty_cell(char board[MAX_GRID_SIZE][MAX_GRID_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j] == EMPTY) {
                return &board[i][j];
            }
        }
    }
    return NULL;
}

/**
 * Counts how many complete rows have been filled
 * @param board: Game board
 * @param rows: Number of rows
 * @param cols: Number of columns
 * @return: Number of complete rows
 */
int count_complete_rows(char board[MAX_GRID_SIZE][MAX_GRID_SIZE], int rows, int cols) {
    int complete_rows = 0;
    
    for (int i = 0; i < rows; i++) {
        bool row_complete = true;
        for (int j = 0; j < cols; j++) {
            if (board[i][j] == EMPTY) {
                row_complete = false;
                break;
            }
        }
        if (row_complete) {
            complete_rows++;
        } else {
            break; // Stop at first incomplete row
        }
    }
    
    return complete_rows;
}

/**
 * Counts how many complete columns have been filled
 * @param board: Game board
 * @param rows: Number of rows
 * @param cols: Number of columns
 * @return: Number of complete columns
 */
int count_complete_columns(char board[MAX_GRID_SIZE][MAX_GRID_SIZE], int rows, int cols) {
    int complete_columns = 0;
    
    for (int j = 0; j < cols; j++) {
        bool column_complete = true;
        for (int i = 0; i < rows; i++) {
            if (board[i][j] == EMPTY) {
                column_complete = false;
                break;
            }
        }
        if (column_complete) {
            complete_columns++;
        } else {
            break; // Stop at first incomplete column
        }
    }
    
    return complete_columns;
}

/**
 * Validates completed rows against their clues
 * @param board: Game board
 * @param complete_rows: Number of complete rows to check
 * @param cols: Number of columns
 * @param clues: Clue data structure
 * @return: true if all complete rows are valid
 */
bool validate_complete_rows(char board[MAX_GRID_SIZE][MAX_GRID_SIZE], int complete_rows, int cols, ClueData* clues) {
    for (int i = 0; i < complete_rows; i++) {
        int found_segments = 0;
        int found_lengths[MAX_CLUES] = {0};
        
        count_black_segments(board[i], cols, &found_segments, found_lengths);
        
        if (found_segments != clues->segment_count[i]) {
            return false;
        }
        
        for (int j = 0; j < found_segments; j++) {
            if (found_lengths[j] != clues->segment_lengths[i][j]) {
                return false;
            }
        }
    }
    return true;
}

/**
 * Validates completed columns against their clues
 * @param board: Game board
 * @param complete_columns: Number of complete columns to check
 * @param rows: Number of rows
 * @param clues: Clue data structure
 * @return: true if all complete columns are valid
 */
bool validate_complete_columns(char board[MAX_GRID_SIZE][MAX_GRID_SIZE], int complete_columns, int rows, ClueData* clues) {
    for (int j = 0; j < complete_columns; j++) {
        char column[MAX_GRID_SIZE];
        for (int i = 0; i < rows; i++) {
            column[i] = board[i][j];
        }
        
        int found_segments = 0;
        int found_lengths[MAX_CLUES] = {0};
        
        count_black_segments(column, rows, &found_segments, found_lengths);
        
        if (found_segments != clues->segment_count[rows + j]) {
            return false;
        }
        
        for (int k = 0; k < found_segments; k++) {
            if (found_lengths[k] != clues->segment_lengths[rows + j][k]) {
                return false;
            }
        }
    }
    return true;
}

/**
 * Prints the solved nonogram board
 * @param board: Solved game board
 * @param rows: Number of rows
 * @param cols: Number of columns
 */
void print_solution(char board[MAX_GRID_SIZE][MAX_GRID_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

/**
 * Recursive backtracking algorithm to solve the nonogram
 * @param board: Game board
 * @param rows: Number of rows
 * @param cols: Number of columns
 * @param clues: Clue data structure
 * @return: true if solution found, false otherwise
 */
bool solve_nonogram_recursive(char board[MAX_GRID_SIZE][MAX_GRID_SIZE], int rows, int cols, ClueData* clues) {
    // Base case: if board is complete, validate and return result
    if (is_board_complete(board, rows, cols)) {
        if (validate_complete_board(board, rows, cols, clues)) {
            print_solution(board, rows, cols);
            return true;
        }
        return false;
    }
    
    // Early pruning: check completed rows and columns
    int complete_rows = count_complete_rows(board, rows, cols);
    if (complete_rows > 0 && !validate_complete_rows(board, complete_rows, cols, clues)) {
        return false;
    }
    
    int complete_columns = count_complete_columns(board, rows, cols);
    if (complete_columns > 0 && !validate_complete_columns(board, complete_columns, rows, clues)) {
        return false;
    }
    
    // Find next empty cell and try both possibilities
    char* current_cell = find_first_empty_cell(board, rows, cols);
    assert(current_cell != NULL);
    
    // Try painting cell black
    *current_cell = BLACK;
    if (solve_nonogram_recursive(board, rows, cols, clues)) {
        return true;
    }
    
    // Try painting cell white
    *current_cell = WHITE;
    if (solve_nonogram_recursive(board, rows, cols, clues)) {
        return true;
    }
    
    // Backtrack: reset cell to empty
    *current_cell = EMPTY;
    return false;
}

/**
 * Main function - entry point of the program
 */
int main() {
    int rows, cols;
    ClueData clues = {0};
    char board[MAX_GRID_SIZE][MAX_GRID_SIZE];
    
    // Read puzzle dimensions
    scanf("%d %d", &rows, &cols);
    
    // Initialize the game board
    initialize_game_board(board, rows, cols);
    
    // Read clues for rows and columns
    for (int i = 0; i < rows + cols; i++) {
        scanf("%d", &clues.segment_count[i]);
        for (int j = 0; j < clues.segment_count[i]; j++) {
            scanf("%d", &clues.segment_lengths[i][j]);
        }
    }
    
    // Solve the nonogram puzzle
    solve_nonogram_recursive(board, rows, cols, &clues);
    
    return 0;
}
