#include "game_engine.h"

int reset_mine_number(const char* level){
    if(strcmp(level,"easy") == 0)
        return MINES_EASY;
    else if(strcmp(level,"medium") == 0)
        return MINES_MEDIUM;
    else if(strcmp(level,"hard") == 0)
        return MINES_HARD; 
    else 
        return 10;
}

void reset_grid_size(const char *level, int grid_size[2]) {
    if (strcmp(level, "easy") == 0) {
        grid_size[0] = GRID_SIZE_EASY;
        grid_size[1] = GRID_SIZE_EASY;
    } else if (strcmp(level, "medium") == 0) {
        grid_size[0] = GRID_SIZE_MEDIUM;
        grid_size[1] = GRID_SIZE_MEDIUM;
    } else if (strcmp(level, "hard") == 0) {
        grid_size[0] = GRID_SIZE_HARD;
        grid_size[1] = GRID_SIZE_HARD;
    } else {  // default
        grid_size[0] = GRID_SIZE_EASY;
        grid_size[1] = GRID_SIZE_EASY;
    }
}

void print_board(GameGrid *grid){
    int rows = grid->size;
    int cols = grid->rows[0].size;

    for (size_t i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if(grid->rows[i].cols[j].mine == TRUE)
                printf("X ");
            else
                printf("%d ", grid->rows[i].cols[j].val);
        }
        printf("\n");
    }
    printf("\n");
}

GameGrid *create_game_grid(int grid_size[2]){
    GameGrid *grid = malloc(sizeof(GameGrid));
    if(!grid) return NULL;

    grid->size = grid_size[0];
    grid->rows = malloc(sizeof(BlockRow) * grid->size);
    if(!grid->rows) return NULL;

    for (int i = 0; i < grid_size[0]; i++){
        grid->rows[i].size = grid_size[1];
        grid->rows[i].cols = malloc(sizeof(Block) * grid_size[1]);
        for (int j = 0; j < grid_size[1]; j++)
        {
           grid->rows[i].cols[j] = (Block){
                .row = i,
                .col = j,
                .val = 0,
                .active = TRUE,
                .mine = FALSE,
                .is_flag = FALSE
           };
        }
    }

    return grid;
}

void destroy_game_grid(GameGrid *grid){
    if(!grid) return;

    for(int i=0; i < grid->size; i++)
        free(grid->rows[i].cols);
    
    free(grid->rows);

    free(grid);
}

void reset_game_grid(GameGrid *grid){
    int rows = grid->size;
    int cols = grid->rows[0].size;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
                    grid->rows[i].cols[j].val = 0;
                    grid->rows[i].cols[j].active = TRUE;
                    grid->rows[i].cols[j].flag = FALSE;
                    grid->rows[i].cols[j].mine = FALSE;
                    gtk_stack_set_visible_child_name(
                        GTK_STACK(grid->rows[i].cols[j].stack), 
                        "button");
        }
    }   
}

void set_mines(int mines, GameGrid *grid) {
    int rows = grid->size;
    int cols = grid->rows[0].size;

    // Clamp mine number
    if (mines > rows * cols)
        mines = rows * cols;

    // Place the mines
    int n_mines = mines;
    while(n_mines > 0) {
        int i = rand() % rows;
        int j = rand() % cols;

        if (!grid->rows[i].cols[j].mine) {
            grid->rows[i].cols[j].mine = TRUE;
            n_mines--;
        }
    }

    // Now count the neighbours
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if(!grid->rows[i].cols[j].mine) continue;

            // Plus 1 to all of the neighbours around the mine      
            for (int ii = i - 1; ii <= i + 1; ii++) {
                for (int jj = j - 1; jj <= j + 1; jj++) {

                    if (ii < 0 || ii >= rows ||
                        jj < 0 || jj >= cols ||
                        (ii == i && jj == j))
                        continue;

                    grid->rows[ii].cols[jj].val++;
                }
            }
        }
    }

    // print_board(grid);
}




