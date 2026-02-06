#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <gtk/gtk.h>
#include "../timer/timer.h"

#define MINES_EASY 10
#define MINES_MEDIUM 40
#define MINES_HARD 150

#define GRID_SIZE_EASY 10
#define GRID_SIZE_MEDIUM 15
#define GRID_SIZE_HARD 30



typedef struct {
    int row;
    int col;
    int val;
    GtkWidget *stack;
    GtkWidget *btn;
    GtkWidget *label;
    GtkWidget *flag;
    gboolean active;
    gboolean mine;
    gboolean is_flag;
} Block;

typedef struct {
    int size; // Gives the number of columns
    Block *cols;
} BlockRow;

typedef struct {
    int size;
    BlockRow *rows;
} GameGrid;

typedef enum {
    GAME_PLAYING = 0,   // game is ongoing
    GAME_WON     = 1,   // player has won
    GAME_LOST    = 2    // player has lost
} GameStatus;

typedef struct {
    const char *difficulty;
    int mines;
    int grid_size[2];
    GameStatus status;
    GameGrid *grid;
} Game ;


// Creates the game grid structure
GameGrid *create_game_grid(int grid_size[2]);

// Destroy the game grid structure
void destroy_game_grid(GameGrid *grid);

// Resets the mine counter of the selected difficulty
int reset_mine_number(const char* level);

// Resets the grid size counters of the selected difficulty
void reset_grid_size(const char *level, int grid_size[2]);

// Initializes the positions of the mines on the grid and updates the neighbours
void set_mines(int mines, GameGrid *grid);

// Resets the game grid to the default state for the same grid size
void reset_game_grid(GameGrid *grid);

// Prints the board the solved board in ASCII
void print_board(GameGrid *grid);

#endif 

