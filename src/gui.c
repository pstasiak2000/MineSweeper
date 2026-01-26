#include "gui.h"

GtkCssProvider *provider;

/*****************************************/
/* General  Purpose                      */
/*****************************************/

// Global or static function
static void reset_response_cb(GtkDialog *dlg, gint response, gpointer user_data) {
    gboolean *result = (gboolean *)user_data;

    if (response == GTK_RESPONSE_ACCEPT) {
        *result = TRUE;
    } else {
        *result = FALSE;
    }

    gtk_window_destroy(GTK_WINDOW(dlg));

    // Quit temporary loop if you used one
    GMainLoop *loop = g_object_get_data(G_OBJECT(dlg), "loop");
    if (loop) g_main_loop_quit(loop);
}

gboolean show_reset_confirm_dialog(GtkWindow *parent) {
    gboolean result = FALSE;
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Confirm Reset",
        parent,
        GTK_DIALOG_MODAL,
        "_No", GTK_RESPONSE_REJECT,
        "_Yes", GTK_RESPONSE_ACCEPT,
        NULL
    );

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *label = gtk_label_new("Are you sure you want to reset the board?");
    gtk_box_append(GTK_BOX(content), label);

    // Store loop in dialog so callback can quit it
    g_object_set_data(G_OBJECT(dialog), "loop", loop);

    // Connect callback (normal C function)
    g_signal_connect(dialog, "response", G_CALLBACK(reset_response_cb), &result);

    gtk_window_present(GTK_WINDOW(dialog));

    g_main_loop_run(loop);   // blocks until callback calls g_main_loop_quit
    g_main_loop_unref(loop);

    return result;
}

static void reveal_block(GameGrid *grid, int row, int col) {
    // bounds check
    if (row < 0 || col < 0 || row >= grid->size || col >= grid->rows[0].size)
        return;

    Block *block = &grid->rows[row].cols[col];

    // Already revealed or flagged
    if (!block->active || block->flag)
        return;

    // Reveal this block
    block->active = FALSE;
    gtk_stack_set_visible_child_name(GTK_STACK(block->stack), "label");

    // Only recurse if value is zero
    if (block->val == 0) {
        // Check all 8 neighbors
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                if (dr == 0 && dc == 0)
                    continue;
                reveal_block(grid, row + dr, col + dc);
            }
        }
    }
}

static void reveal_all_mines(GameGrid *grid){
    for (int row = 0; row < grid->size; row++) {
        for (int col = 0; col < grid->rows[0].size; col++) {    
            if(grid->rows[row].cols[col].mine){
                gtk_stack_set_visible_child_name(
                    GTK_STACK(grid->rows[row].cols[col].stack), "label");
            }
        }
    }
}

static void on_cell_clicked(GtkButton *button, gpointer user_data)
{
    CellCallBackData *data = user_data;
    Game *game = data->game;
    Block *block = data->block;

    // g_print("Clicked cell at row=%d col=%d\n", block->row, block->col);

    // Start the timer if not already started
    if (!game->timer.timeout_id)
        timer_start(&game->timer);

    if (!block->active)
        return;

    if (block->mine) {
        g_print("💥 Mine!\n"); 
        reveal_all_mines(game->grid); // Reveal all mines on the screen
        timer_pause(&game->timer);
    } else {
        reveal_block(game->grid, block->row, block->col);
    }
}

static void on_cell_mouse_press(GtkGestureClick *gesture,
                                int n_press,
                                double x,
                                double y,
                                gpointer user_data)
{
    CellCallBackData *data = user_data;
    Block *block = data->block;

    guint button = gtk_gesture_single_get_current_button(
                       GTK_GESTURE_SINGLE(gesture));

    if (button == GDK_BUTTON_PRIMARY) {
        on_cell_clicked(GTK_BUTTON(block->btn), data);
    } else if (button == GDK_BUTTON_SECONDARY) {
        // toggle the logical flag
        block->is_flag = !block->is_flag;

        // update GUI: show the flag label if flagged, else show button
        gtk_stack_set_visible_child_name(
            GTK_STACK(block->stack),
            block->is_flag ? "flag_label" : "button"
        );
    }
}

static void on_cell_right_clicked(GtkGestureClick *gesture,
                                  int n_press,
                                  double x,
                                  double y,
                                  gpointer user_data)
{
    CellCallBackData *data = user_data;
    Block *block = data->block;
    Game *game = data->game;

    // Set the flag and increase/decrease the number of mines
    block->is_flag = !block->is_flag;
    if(block->is_flag) game->mines--;
    if(!block->is_flag) game->mines++;

    // Set the mines label
    // char buf[32];
    // snprintf(buf,sizeof(buf),"Mines: %d", game->mines);
    // gtk_label_set_text(GTK_LABEL(ctx->ui->mines.label), buf);

    gtk_stack_set_visible_child_name(GTK_STACK(block->stack),
                                     block->is_flag ? "flag_label" : "button");
}

static int select_minimum_box_size(const char *level){
    if(strcmp(level,"easy") == 0)
        return 50;
    else if(strcmp(level,"medium") == 0)
        return 30;
    else if(strcmp(level,"hard") == 0)
        return 10;
    else
        return 0;
}

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
            
            gtk_label_set_text(GTK_LABEL(block->label), buf);
        }
    }  
}

// Generates and fills the GTK grid struct
void generate_gtk_grid(GtkWidget *grid, Game *game) {
    for (int row = 0; row < game->grid_size[0]; row++) {
        for (int col = 0; col < game->grid_size[1]; col++) {

            Block *block = &game->grid->rows[row].cols[col];

            // --- Create stack for the cell ---
            GtkWidget *stack = gtk_stack_new();
            block->stack = stack;

            // --- Create button ---
            GtkWidget *btn = gtk_button_new();
            block->btn = btn;

            gtk_widget_set_hexpand(btn, TRUE);
            gtk_widget_set_vexpand(btn, TRUE);
            gtk_widget_set_halign(btn, GTK_ALIGN_FILL);
            gtk_widget_set_valign(btn, GTK_ALIGN_FILL);

            // --- Create label ---
            char buf[8];
            if (block->mine)
                snprintf(buf, sizeof(buf), "💣");
            else if (!block->val)
                snprintf(buf, sizeof(buf), "");
            else
                snprintf(buf, sizeof(buf), "%d", block->val);

            GtkWidget *label = gtk_label_new(buf);
            block->label = label;

            gtk_widget_set_hexpand(label, TRUE);
            gtk_widget_set_vexpand(label, TRUE);
            gtk_widget_set_halign(label, GTK_ALIGN_FILL);
            gtk_widget_set_valign(label, GTK_ALIGN_FILL);

            // --- Create flag label ---
            GtkWidget *flag_label = gtk_label_new("🚩");
            block->flag = flag_label;

            gtk_widget_set_hexpand(flag_label, TRUE);
            gtk_widget_set_vexpand(flag_label, TRUE);
            gtk_widget_set_halign(flag_label, GTK_ALIGN_FILL);
            gtk_widget_set_valign(flag_label, GTK_ALIGN_FILL);

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

            gtk_stack_set_visible_child_name(GTK_STACK(stack), "button");

            // --- Attach stack to the grid ---
            gtk_grid_attach(GTK_GRID(grid), stack, col, row, 1, 1);

            // --- Prepare callback data ---
            CellCallBackData *data = g_new(CellCallBackData, 1);
            data->block = block;
            data->game = game;

            // --- Left click using the button's native signal ---
            g_signal_connect(btn, "clicked",
                             G_CALLBACK(on_cell_clicked), data);

            // --- Right click using GtkGestureClick on the stack ---
            GtkGestureClick *right_click = GTK_GESTURE_CLICK(gtk_gesture_click_new());
            gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(right_click), GDK_BUTTON_SECONDARY);
            gtk_widget_add_controller(stack, GTK_EVENT_CONTROLLER(right_click));

            g_signal_connect(right_click, "pressed",
                             G_CALLBACK(on_cell_right_clicked), data);
        }
    }
}


GtkWidget *create_header_box(Game *game) {
    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    gtk_widget_set_hexpand(header_box, TRUE); // horizontal fill
    gtk_widget_set_size_request(header_box, -1, 40);
    gtk_box_set_homogeneous(GTK_BOX(header_box), TRUE); // children equal sizes

    return header_box;
}

Panel create_timer_panel(Game *game) {

    /* Timer panel */
    GtkWidget *timer_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_hexpand(timer_panel, TRUE);
    gtk_widget_set_valign(timer_panel, GTK_ALIGN_FILL);

    /* Timer label */
    GtkWidget *timer_label = gtk_label_new(NULL);
    gtk_widget_add_css_class(timer_label, "timer");
    gtk_box_append(GTK_BOX(timer_panel), timer_label);
    gtk_widget_set_halign(timer_label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(timer_label, GTK_ALIGN_CENTER);
    
    /* Timer initialization */
    timer_init(&game->timer, GTK_LABEL(timer_label));  
    
    Panel timer= {.box = timer_panel, .label = timer_label};

    return timer;
}

Panel create_mines_panel(Game *game) {
    /* Mines panel */
    GtkWidget *mines_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_hexpand(mines_panel, TRUE);
    gtk_widget_set_valign(mines_panel, GTK_ALIGN_FILL);

    /* Mines label */
    char buf[32];
    snprintf(buf,sizeof(buf), "Mines: %d", game->mines);
    GtkWidget *mines_label = gtk_label_new(buf);
    gtk_widget_add_css_class(mines_label, "mines");
    gtk_box_append(GTK_BOX(mines_panel), mines_label);
    
    gtk_widget_set_halign(mines_label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(mines_label, GTK_ALIGN_CENTER);

    Panel mines = {.box = mines_panel, .label = mines_label};

    return mines;
}

GtkWidget *create_gui_grid(Game *game) {
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 1);    // vertical spacing
    gtk_grid_set_column_spacing(GTK_GRID( grid), 1); // horizontal spacing
    gtk_widget_set_hexpand(grid, TRUE);             // fill horizontally
    gtk_widget_set_vexpand(grid, TRUE);             // fill vertically
    
    gtk_widget_set_halign(grid, GTK_ALIGN_FILL);
    gtk_widget_set_valign(grid, GTK_ALIGN_FILL);

    int rows = game->grid_size[0];
    int cols = game->grid_size[1];

   game->grid = create_game_grid(game->grid_size);
   initialize_mines(game->mines, game->grid);

    generate_gtk_grid(grid, game);
    
    

    return grid;
}