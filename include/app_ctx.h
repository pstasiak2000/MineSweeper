#ifndef APP_CTX_H
#define APP_CTX_H 

#include "gui/gui.h"
#include "game_engine.h"
#include "timer/timer.h"

typedef struct {
    GtkApplication *app;
    Game *game;
    GameUI *ui;
    Timer timer;
} AppCtx;

typedef struct {
    AppCtx *ctz; // AppCtx pointer
    Block *block; // Current block
} CellCallBackData;

/* Fill the GTK GameGrid struct and attach it to grid */
void generate_gtk_grid(GtkWidget *grid, Game *game);


#endif