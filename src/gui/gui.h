#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

// Global provider
extern GtkCssProvider *provider;

typedef struct {
    GtkWidget *box;
    GtkWidget *label;
} Panel ;

typedef struct {
    GtkWidget *window;
    GtkWidget *header_box;
    Panel timer;
    Panel mines;
    GtkWidget *grid;
} GameUI ;

/* Create a header box for the panels */
GtkWidget *create_header_box(void);

/* Create an empty gui grid */
GtkWidget *create_empty_gui_grid(void);

/* Create a header panel with the chosen text*/
Panel create_header_panel(const char *text, const char *css_class_name);

/* Update the text of a label */
void update_label_text(GtkWidget *label, const char *new_text);

/* Create a confirmation box*/
gboolean show_reset_confirm_dialog(GtkWindow *parent, const char *text);

/* Create a quick message */
void quick_message(GtkWindow *parent, char *title, char *message);

/* Updates the labels of the mine boxes when making a new game */
// void update_mine_labels(Game *game);

#endif 
