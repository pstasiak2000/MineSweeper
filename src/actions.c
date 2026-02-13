#include "actions.h"


static void action_new(GSimpleAction *action, GVariant *p, gpointer user_data) {
    AppCtx *ctx = user_data;

    GameUI *ui = ctx->ui;
    Game *game = ctx->game;

    timer_stop(&ctx->timer);
    timer_reset(&ctx->timer);

    ctx->game->mines = reset_mine_number(ctx->game->difficulty);

    char buf[32];
    snprintf(buf,sizeof(buf),"💣 %d", ctx->game->mines);
    update_label_text(ui->mines.label,buf);


    printf("New game triggered. Current difficulty is: %s\n", ctx->game->difficulty);
    
    // reset your game state here
    reset_game_grid(game->grid);
    set_mines(game->mines, game->grid);

    // Update the mine labels in the new game
    update_mine_labels(game);

    // print_board(game->grid);
}

static void action_solve(GSimpleAction *action, GVariant *p, gpointer user_data) {
    AppCtx *ctx = user_data;
    Game *game = ctx->game;

    reveal_all_blocks(game->grid);
    timer_pause(&ctx->timer);

    // Print out the victory message
    victory_message(
        GTK_WINDOW(ctx->ui->window),
        &ctx->timer);
}

static void action_quit(GSimpleAction *action, GVariant *p, gpointer user_data) {
    AppCtx *ctx = user_data;  
    g_application_quit(G_APPLICATION(ctx->app));
}


static void
action_toggle_developer_tools(GSimpleAction *action,
                              GVariant      *parameter,
                              gpointer      user_data)
{
    AppCtx *ctx = user_data;

    gboolean state = 
        g_variant_get_boolean(g_action_get_state(G_ACTION(action)));

    state = !state;

    g_simple_action_set_state(action, g_variant_new_boolean(state));

    developer_tools = state;

    g_print("Developer tools: %s\n", state ? "ON" : "OFF");

    /* Enable/disable Solve action*/
    GAction *solve = 
        g_action_map_lookup_action(G_ACTION_MAP(ctx->app),"Solve");

    if (solve)
        g_simple_action_set_enabled(
            G_SIMPLE_ACTION(solve), 
            developer_tools);
}




static void action_about(GSimpleAction *action, GVariant *p, gpointer user_data) {
    printf("MineSweeper GTK4  demo\n");
}

/* --- Callback for difficulty action --- */
static void action_difficulty(GSimpleAction *action, GVariant *p, gpointer user_data) {
    AppCtx *ctx = user_data;

    GameUI *ui = ctx->ui;
    Game *game = ctx->game;

    // Firstly confirm the change
    char text[128] = "Are you sure you want to reset the board?";
    if (show_reset_confirm_dialog(GTK_WINDOW(ui->window),text)) {

        const char *level = g_variant_get_string(p, NULL);
        ctx->game->difficulty = level;

        /* Update action state so GTK shows the selected radio */
        g_simple_action_set_state(action, g_variant_new_string(level));

        g_print("Difficulty set to: %s\n", level);

        // Reset the timer
        timer_stop(&ctx->timer);

        // Update number of mines text
        ctx->game->mines = reset_mine_number(level);
        char buf[32];
        snprintf(buf,sizeof(buf),"💣 %d", ctx->game->mines);
        update_label_text(ui->mines.label,buf);

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
        reset_grid_size(level,game->grid_size);

        // Change the new grid size
        int rows = ctx->game->grid_size[0];
        int cols = ctx->game->grid_size[1];

        // De-allocate and then re-allocate the grid memory
        destroy_game_grid(ctx->game->grid);

        game->grid = create_game_grid(ctx->game->grid_size);
        
        generate_gtk_grid(ui->grid, ctx);
        reset_game_grid(game->grid);

        set_mines(game->mines,game->grid);
        update_mine_labels(game);
        // gtk_widget_set_visible(ui->grid, TRUE);    
    } else {
        return; // user clicked No
    }
}

/* --- Array of Toolbar actions --- */
static const GActionEntry actions[] = {
        { "New",   action_new, NULL, NULL, NULL }, 
        { "Quit",  action_quit,  NULL, NULL, NULL },
        { "About", action_about, NULL, NULL, NULL },
        { "Difficulty", action_difficulty, "s", "'easy'", NULL },
        { "Solve", action_solve, NULL, NULL, NULL },
        {"developer-tools",
         action_toggle_developer_tools,
         NULL,
         "false",
         NULL
        }
};

/* Getter for external use*/
const GActionEntry *get_actions(size_t *n) {
    *n = G_N_ELEMENTS(actions);
    return actions;
}

