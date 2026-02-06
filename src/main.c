#include <gtk/gtk.h>

// #include <timer/timer.h>

#include "version.h"

#include "app_ctx.h"
#include "actions.h"
#include "base.h"
#include "toolbar.h"


static int counter = 0;

static void activate(GtkApplication *app, gpointer user_data) {
    AppCtx *ctx = user_data;

    Game *game = ctx->game;
    GameUI *ui = ctx->ui;

    /* Create a CSS provider for styling */
    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(
        provider,
        "/com/github/pstasiak2000/minesweeper/style.css"
    );

    /* Title buffer for window name based on game version*/
    char buf[32];
    snprintf(buf,sizeof(buf),"MineSweeper v%s", MINESWEEPER_VERSION);

    GtkWidget *window = gtk_application_window_new(app);
    ui->window = window;
    gtk_window_set_title(GTK_WINDOW(window), buf);
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 900);

    GtkWidget *main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(main_vbox, GTK_ALIGN_FILL);
    gtk_window_set_child(GTK_WINDOW(window), main_vbox);

    /* Menu bar widget */
    GtkWidget *menubar =
        gtk_popover_menu_bar_new_from_model(create_menu());
    gtk_box_append(GTK_BOX(main_vbox), menubar);

    /* Header box for timer and number of mines */
    ui->header_box = create_header_box();
    gtk_widget_add_css_class(ui->header_box, "header");
    gtk_box_append(GTK_BOX(main_vbox), ui->header_box);

    /* Timer panel and mines panels */
    ui->timer = create_header_panel("00:00","timer");
    ui->mines = create_header_panel("💣 10","mines");

    gtk_box_append(GTK_BOX(ui->header_box), ui->timer.box);
    gtk_box_append(GTK_BOX(ui->header_box), ui->mines.box); 

    // Initialize the timer here
    timer_init(&ctx->timer, GTK_LABEL(ui->timer.label));

    /* Generate the grid */
    ui->grid = create_empty_gui_grid();
    game->grid = create_game_grid(game->grid_size);

    generate_gtk_grid(ui->grid,ctx);    

    reset_game_grid(game->grid);
    set_mines(game->mines, game->grid); 
    update_mine_labels(game);

    

    // gtk_widget_set_visible(ui->grid, TRUE);

    // Add the grid to your main vbox
    gtk_box_append(GTK_BOX(main_vbox), ctx->ui->grid);


    /* Attach CSS provider to the entire display */
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    gtk_init();

    srand(time(NULL));  // Seed RNG with current tim

    GtkApplication *app =
        gtk_application_new("com.example.menu", G_APPLICATION_DEFAULT_FLAGS);

    Game game = { .difficulty = "easy", .mines = 10, .grid_size={10,10}, .status=GAME_PLAYING};
    GameUI ui = {0};

    AppCtx ctx = {
        .app = app,
        .game = &game,
        .ui = &ui,
    };

    size_t n;
    const GActionEntry *actions = get_actions(&n);

    g_action_map_add_action_entries(G_ACTION_MAP(app), actions, n, &ctx);

    g_signal_connect(app, "activate", G_CALLBACK(activate), &ctx);

    return g_application_run(G_APPLICATION(app), argc, argv);
}