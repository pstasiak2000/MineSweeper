#ifndef APP_CTX_H
#define APP_CTX_H 

#include "gui/gui.h"
#include "game_engine.h"
#include "timer/timer.h"
#include "ldbd/ldbd.h"

typedef struct {
    GtkApplication *app;
    Game *game;
    GameUI *ui;
    Timer timer;
} AppCtx;

typedef struct {
    AppCtx *ctx; // AppCtx pointer
    Block *block; // Current block
} CellCallBackData;




#endif
