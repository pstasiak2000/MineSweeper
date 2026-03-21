#ifndef BASE_H
#define BASE_H

#include <gtk/gtk.h>
#include "app_ctx.h"

typedef GtkWidget *(*WidgetFactory)(void *user_data);

/* Updated all of the mine labels */
void update_mine_labels(Game *game);

/* Running the events which occur on winning */
void run_win_events(AppCtx *ctx);

/* Fill the GTK GameGrid struct and attach it to grid */
void generate_gtk_grid(GtkWidget *grid, AppCtx *ctx);

/* Reveal all of the blocks in the game */ 
void reveal_all_blocks(GameGrid *grid);

void reset_visibility(Game *game);
#endif
