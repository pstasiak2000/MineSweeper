#ifndef APP_CTX_H
#define APP_CTX_H 

#include "gui.h"
#include "game_engine.h"

typedef struct {
    Game *game;
    GameUI *ui;
} AppCtx;



#endif