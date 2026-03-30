#ifndef BASE_H
#define BASE_H

#include <gtk/gtk.h>
#include "app_ctx.h"

typedef GtkWidget *(*WidgetFactory)(void *user_data);

/* Updated all of the mine labels */
void update_mine_labels(Game *game);

/* Running the events which occur on winning */
void run_win_events(AppCtx *ctx);

/* Fill the Gtk GameGrid struct and attach it to UI grid */
void generate_gtk_grid(GtkWidget *grid, AppCtx *ctx);

/* Re-generate the Gtk GameGrid struct fully and re-attach it to UI grid */
void regenerate_gtk_grid(AppCtx *ctx);

/* Reveal all of the blocks in the game */ 
void uncover_all_blocks(GameGrid *grid, Level level);

/* Sets the activity and visibility for all blocks to TRUE */ 
void reset_visibility(Game *game);


#endif
