#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "game_engine.h"


gboolean show_reset_confirm_dialog(GtkWindow *parent);

typedef struct {
    GtkWidget *box;
    GtkWidget *label;
} Panel ;

typedef struct {
    Panel timer;
    Panel mines;
    GtkWidget *grid;
} GameUI ;

// Global provider
extern GtkCssProvider *provider;

// Updates the labels of the mine boxes when making a new game
void update_mine_labels(Game *game);

GtkWidget *create_header_box(Game *game);


Panel create_timer_panel(Game *game);
Panel create_mines_panel(Game *game);

// Generates and fills the GTK grid struct;
void generate_gtk_grid(GtkWidget *grid, Game *game);

GtkWidget *create_gui_grid(Game *game);

#endif 