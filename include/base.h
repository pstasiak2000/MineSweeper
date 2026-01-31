#ifndef BASE_H
#define BASE_H

#include <gtk/gtk.h>
#include "app_ctx.h"

typedef GtkWidget *(*WidgetFactory)(void *user_data);

/* Updated all of the mine labels */
void update_mine_labels(Game *game);


void generate_gtk_grid(GtkWidget *grid, Game *game);

#endif