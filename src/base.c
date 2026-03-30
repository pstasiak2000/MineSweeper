
#include "base.h"

void 
victory_message(GtkWindow *window, Timer *timer)
{
    char popup_title[128] = "🎉 You win! 🎉 ";
    char popup_message[128];
    sprintf(popup_message,
         "Congratulations! You completed the puzzle in %02d:%02d"
         ,get_minutes(timer),get_seconds(timer));

    quick_message(window, popup_title, popup_message);
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
                snprintf(buf,sizeof(buf)," ");
            else
                snprintf(buf,sizeof(buf),"%d",block->val);
            
            update_label_text(block->label, buf);
            // gtk_stack_set_visible_child_name(GTK_STACK(block->stack), "label");
        }
    }  
}

static int select_minimum_box_size(Level level){
	switch(level) {
		case EASY:   return 30;
		case MEDIUM: return 20;
		case HARD:   return 10;
		default:     return 0;
	}
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
/* Reveal a singular block */
static void uncover_block(Block *block, Level level){	
	block->active = FALSE;
	gtk_stack_set_visible_child_name(
	GTK_STACK(block->stack), "label");
}

/* Reveal a singular mine */
static void uncover_mine_lose(Block *block, Level level){
	uncover_block(block, level);
	switch(level){
		case(EASY):
			gtk_widget_add_css_class(block->label, "mine-label-easy-lose");
			break;
		case(MEDIUM):
			gtk_widget_add_css_class(block->label, "mine-label-medium-lose");
			break;
		case(HARD):
			gtk_widget_add_css_class(block->label, "mine-label-hard-lose");
			break;
		default:
			gtk_widget_add_css_class(block->label, "mine-label-easy-lose");
	}
}

/* Reveal a singular mine */
static void uncover_mine_win(Block *block, Level level){
	uncover_block(block, level);
	switch(level){
		case(EASY):
			gtk_widget_add_css_class(block->label, "mine-label-easy-win");
			break;
		case(MEDIUM):
			gtk_widget_add_css_class(block->label, "mine-label-medium-win");
			break;
		case(HARD):
			gtk_widget_add_css_class(block->label, "mine-label-hard-win");
			break;
		default:
			gtk_widget_add_css_class(block->label, "mine-label-easy-win");
	}
}

void uncover_all_blocks(GameGrid *grid, Level level){
    for (int row = 0; row < grid->size; row++) {
        for (int col = 0; col < grid->rows[0].size; col++) { 
			Block *block = &grid->rows[row].cols[col];
			if(block->mine)
				uncover_mine_win(block, level);
			else
				uncover_block(block, level);
		}
    }
}

static void uncover_all_mines(GameGrid *grid, Level level){
    for (int row = 0; row < grid->size; row++) {
        for (int col = 0; col < grid->rows[0].size; col++) { 
			Block *block = &grid->rows[row].cols[col];
           	block->active = FALSE; 
			if(block->mine)
				uncover_mine_lose(block, level);
        }
    }
}



static int check_win_status(GameGrid *grid){
    for (int row = 0; row < grid->size; row++) {
        for (int col = 0; col < grid->rows[0].size; col++) { 
            
            Block *block = &grid->rows[row].cols[col];

            /* Check if all mines have been covered with a flag*/
            if(block->mine && !block->is_flag)
                return 0;
            
            /* Check if all non-mines have been revealed */
            // GtkWidget *visible = gtk_stack_get_visible_child(GTK_STACK(block->stack));
            // if(!block->mine && visible == "button")
            //     return 0;
        }
    }    
    return 1;
}


static void on_cell_clicked(GtkButton *button, gpointer user_data)
{
    CellCallBackData *data = user_data;
    AppCtx *ctx = data->ctx;
    
    Game *game = ctx->game;
    Block *block = data->block;

    // g_print("Clicked cell at row=%d col=%d\n", block->row, block->col);

    if (!block->active)
        return;

    // Start the timer if not already started
    if (!ctx->timer.timeout_id)
        timer_start(&ctx->timer);

    if (block->mine) {
        g_print("💥 Mine!\n"); 
        uncover_all_mines(game->grid,game->level); // Reveal all mines on the screen
        timer_pause(&ctx->timer);
        game->status = GAME_LOST;
    } else if(check_win_status(game->grid)) { 
	    run_win_events(ctx);
    }  else {
        reveal_block(game->grid, block->row, block->col);
    }
}

void run_win_events(AppCtx *ctx){	
	ctx->game->mines = 0;
	update_mine_labels(ctx->game);	
	g_print("Congratulations - You won!!!\n");
	uncover_all_blocks(ctx->game->grid, ctx->game->level);
	timer_pause(&ctx->timer);
	ctx->game->status = GAME_WON;
	
    Level level = ctx->game->level;

	// Leaderboard events will happen here
	char *entry_name = "No Name";
	LDBD_Entry entry = LDBD_create_entry(entry_name,level,100,0);
	
	printf("%s",entry.name);

	LDBD_save_entry(&entry);

        // Message box;
        victory_message(
            GTK_WINDOW(ctx->ui->window),
            &ctx->timer);
	

}

static void on_cell_right_clicked(GtkGestureClick *gesture,
                                  int n_press,
                                  double x,
                                  double y,
                                  gpointer user_data)
{
    CellCallBackData *data = user_data;
    Block *block = data->block;
    AppCtx *ctx = data->ctx;

    GameUI *ui = ctx->ui;
    Game *game = ctx->game;

    if(!block->active || !ctx->timer.timeout_id)
        return;

    // Set the flag and increase/decrease the number of mines

    if(!game->mines && !block->is_flag)
        return;

    block->is_flag = !block->is_flag;
    if(block->is_flag) game->mines--;
    if(!block->is_flag) game->mines++;

    /* Set the mines label */
    char buf[32];
    snprintf(buf,sizeof(buf),"💣: %d", game->mines);
    
    update_label_text(ui->mines.label,buf);
    gtk_label_set_text(GTK_LABEL(ctx->ui->mines.label), buf);

    gtk_stack_set_visible_child_name(GTK_STACK(block->stack),
                                     block->is_flag ? "flag_label" : "button");

    if(check_win_status(game->grid)) { 
	    run_win_events(ctx);
    }
}

void reset_visibility(Game *game){

    int rows = game->grid_size[0];
    int cols = game->grid_size[1];
    
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {

            Block *block = &game->grid->rows[row].cols[col];

            block->active = TRUE;
            gtk_stack_set_visible_child_name(GTK_STACK(block->stack), "button");
        }
    }
}


void generate_gtk_grid(GtkWidget *grid, AppCtx *ctx) {
    Game *game = ctx->game;
	
	Level level = game->level;

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
                snprintf(buf, sizeof(buf), " ");
            else
                snprintf(buf, sizeof(buf), "%d", block->val);

            GtkWidget *label = create_block_widget(label_factory,buf);
            block->label = label;

            // // --- Create flag label ---
            GtkWidget *flag_label = create_block_widget(label_factory,"🚩");
            block->flag = flag_label;
			

            // --- CSS classes ---
            gtk_widget_add_css_class(btn, "cell");
           
		   set_widget_css_classes(label, flag_label, level);	

            // --- Minimum box size ---
            int min_box_size = select_minimum_box_size(game->level);
            gtk_widget_set_size_request(stack, min_box_size, min_box_size);

            // --- Add children to stack ---
            gtk_stack_add_named(GTK_STACK(stack), btn, "button");
            gtk_stack_add_named(GTK_STACK(stack), label, "label");
            gtk_stack_add_named(GTK_STACK(stack), flag_label, "flag_label");

            gtk_stack_set_visible_child_name(GTK_STACK(stack), "button");

            // --- Attach stack to the grid ---
            gtk_grid_attach(GTK_GRID(grid), stack, col, row, 1, 1);

            // // --- Prepare callback data ---
            CellCallBackData *data = g_new(CellCallBackData, 1);
            data->ctx = ctx;
            data->block = block;
            

            // // --- Left click using the button's native signal ---
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


void regenerate_gtk_grid(AppCtx *ctx){
	
	/* Clear the existing grid*/
	GtkWidget *child, *next;
	for (child = gtk_widget_get_first_child(GTK_WIDGET(ctx->ui->grid));
		child != NULL;
		child = next)
	{
		next = gtk_widget_get_next_sibling(child);
		gtk_widget_unparent(child);
	}

	// Reset the size of the grid based on the level
	reset_grid_size(ctx->game->level, ctx->game->grid_size);

	// Change the new grid size
	int rows = ctx->game->grid_size[0];
	int cols = ctx->game->grid_size[1];

	// De-allocate and then re-allocate the grid memory
	destroy_game_grid(ctx->game->grid);

	ctx->game->grid = create_game_grid(ctx->game->grid_size);
	
	generate_gtk_grid(ctx->ui->grid, ctx);
	reset_game_grid(ctx->game->grid, ctx->game->level);

	set_mines(ctx->game->mines, ctx->game->grid);
	update_mine_labels(ctx->game);
}
