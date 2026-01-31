#include <gtk/gtk.h>

// #include <timer/timer.h>

#include "app_ctx.h"
#include "actions.h"
#include "toolbar.h"


static int counter = 0;

static void activate(GtkApplication *app, gpointer user_data) {
    AppCtx *ctx = user_data;

    Game *game = ctx->game;
    GameUI *ui = ctx->ui;

    /* Create a CSS provider for styling */
    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider,
        "label.timer { font-size: 24pt; font-weight: bold; color: white; }"
        "label.mines { font-size: 24pt; font-weight: bold; color: white; }"
        ".header { background-color: rgba(66, 66, 66, 1); }"
    ".cell {"
    "    background-color: #3a3a3aff; "  /* default white */
    "    border-radius: 0px;"
    "    border: 1px solid rgba(122, 122, 122, 1);"
    "}"
    ".cell-label-easy {"
    "   font-size: 5em;"
    "   font-weight: bold;"
    "   border: 1px solid rgba(122, 122, 122, 1);"
    "}"
    ".cell-label-medium {"
    "   font-size: 3em;"
    "   font-weight: bold;"
    "   border: 1px solid rgba(122, 122, 122, 1);"
    "}"
    ".cell-label-hard {"
    "   font-size: 1.5em;"
    "   font-weight: bold;"
    "   border: 1px solid rgba(122, 122, 122, 1);"
    "}"
    ".flag-label-easy {"
    "   font-size: 5em;"
    "   border: 1px solid rgba(122, 122, 122, 1);"
    "}"
    ".flag-label-medium {"
    "   font-size: 2em;"
    "   border: 1px solid rgba(122, 122, 122, 1);"
    "}"
    ".flag-label-hard {"
    "   font-size: 1.5em;"
    "   border: 1px solid rgba(122, 122, 122, 1);"
    "}"
);

    GtkWidget *window = gtk_application_window_new(app);
    ui->window = window;
    gtk_window_set_title(GTK_WINDOW(window), "MineSweeper v1.0");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);

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
    ui->mines = create_header_panel("💣 0","mines");

    gtk_box_append(GTK_BOX(ui->header_box), ui->timer.box);
    gtk_box_append(GTK_BOX(ui->header_box), ui->mines.box); 

    // Initialize the timer here
    timer_init(&ctx->timer, GTK_LABEL(ui->timer.label));

    /* Generate the grid */
    ui->grid = create_empty_gui_grid();
    game->grid = create_game_grid(game->grid_size);
    set_mines(game->mines, game->grid);
    generate_gtk_grid(ui->grid,game);


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

    Game game = { .difficulty = "easy", .mines = 10, .grid_size={10,10}};
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