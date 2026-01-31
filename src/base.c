
#include "base.h"

void update_mine_labels(Game *game){
    for (int row = 0; row < game->grid_size[0]; row++) {
        for (int col = 0; col < game->grid_size[1]; col++) {

            Block *block = &game->grid->rows[row].cols[col];

            // Make the label
            char buf[8];
            if(block->mine)
                snprintf(buf,sizeof(buf),"💣");
            else if(!block->val)
                snprintf(buf,sizeof(buf),"");
            else
                snprintf(buf,sizeof(buf),"%d",block->val);
            
            update_label_text(block->label, buf);
            gtk_stack_set_visible_child_name(GTK_STACK(block->stack), "label");
        }
    }  
}

static int select_minimum_box_size(const char *level){
    if(strcmp(level,"easy") == 0)
        return 40;
    else if(strcmp(level,"medium") == 0)
        return 30;
    else if(strcmp(level,"hard") == 0)
        return 10;
    else
        return 0;
}

static GtkWidget *button_factory(void *user_data){
    (void)user_data;   // silence unused warning
    return gtk_button_new();
}

static GtkWidget *label_factory(void *user_data){
    const char *text = user_data;
    return gtk_label_new(text);
}

static GtkWidget *stack_factory(void *user_data){
    (void)user_data; // silence unused warning
    return gtk_stack_new();
}

static GtkWidget *create_block_widget(WidgetFactory factory, void *user_data){
    GtkWidget *w = factory(user_data);

    gtk_widget_set_hexpand(w, TRUE);
    gtk_widget_set_vexpand(w, TRUE);
    gtk_widget_set_halign(w, GTK_ALIGN_FILL);
    gtk_widget_set_valign(w, GTK_ALIGN_FILL);

    return w;
}

void generate_gtk_grid(GtkWidget *grid, Game *game) {
    int rows = game->grid_size[0];
    int cols = game->grid_size[1];

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {

            Block *block = &game->grid->rows[row].cols[col];

            // --- Create stack for the cell ---
            GtkWidget *stack = create_block_widget(stack_factory,NULL);
            block->stack = stack;

            // --- Create button ---
            GtkWidget *btn = create_block_widget(button_factory,NULL);
            block->btn = btn;

            // --- Create label ---
            char buf[8];
            if (block->mine)
                snprintf(buf, sizeof(buf), "💣");
            else if (!block->val)
                snprintf(buf, sizeof(buf), "");
            else
                snprintf(buf, sizeof(buf), "%d", block->val);

            GtkWidget *label = create_block_widget(label_factory,buf);
            block->label = label;

            // // --- Create flag label ---
            GtkWidget *flag_label = create_block_widget(label_factory,"🚩");
            block->flag = flag_label;

            // --- CSS classes ---
            gtk_widget_add_css_class(btn, "cell");
            if (strcmp(game->difficulty, "easy") == 0){
                gtk_widget_add_css_class(label, "cell-label-easy");
                gtk_widget_add_css_class(flag_label, "flag-label-easy");
            } else if (strcmp(game->difficulty, "medium") == 0){
                gtk_widget_add_css_class(label, "cell-label-medium");
                gtk_widget_add_css_class(flag_label, "flag-label-medium");
            } else {
                gtk_widget_add_css_class(label, "cell-label-hard");
                gtk_widget_add_css_class(flag_label, "flag-label-hard");
            }

            // --- Minimum box size ---
            int min_box_size = select_minimum_box_size(game->difficulty);
            gtk_widget_set_size_request(stack, min_box_size, min_box_size);

            // --- Add children to stack ---
            gtk_stack_add_named(GTK_STACK(stack), btn, "button");
            gtk_stack_add_named(GTK_STACK(stack), label, "label");
            gtk_stack_add_named(GTK_STACK(stack), flag_label, "flag_label");

            gtk_stack_set_visible_child_name(GTK_STACK(stack), "label");

            // --- Attach stack to the grid ---
            gtk_grid_attach(GTK_GRID(grid), stack, col, row, 1, 1);

            // // --- Prepare callback data ---
            // CellCallBackData *data = g_new(CellCallBackData, 1);
            // data->block = block;
            // data->game = game;

            // // --- Left click using the button's native signal ---
            // g_signal_connect(btn, "clicked",
            //                  G_CALLBACK(on_cell_clicked), data);

            // // --- Right click using GtkGestureClick on the stack ---
            // GtkGestureClick *right_click = GTK_GESTURE_CLICK(gtk_gesture_click_new());
            // gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(right_click), GDK_BUTTON_SECONDARY);
            // gtk_widget_add_controller(stack, GTK_EVENT_CONTROLLER(right_click));

            // g_signal_connect(right_click, "pressed",
            //                  G_CALLBACK(on_cell_right_clicked), data);
        }
    }
}